#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "data.h"

SimulatedUART_t uart_peripheral = {0};

static Task_t Task_ReadUART;
static Task_t Task_Telemetry;
static Task_t Task_Heartbeat;

static uint8_t rx_buffer[256];

static uint16_t rx_head = 0;
static uint16_t rx_tail = 0;

static uint32_t bytes_processed = 0;

// static void UART_enable();

// static int UART_SetBaudRate(uint32_t baud_rate);

// static void UART_interrupt();

// static void UART_SWReset();

static void *HWSimulator();

static void SetLastRunTime();

static void RunTasks();

static void BufferFill();

int main(void) {
    printf("Hello, World!\n");

    Task_ReadUART.period_ms = 5;
    Task_Telemetry.period_ms = 100;
    Task_Heartbeat.period_ms = 500;

    pthread_t HWSimulatorThread;
    pthread_create(&HWSimulatorThread,NULL, HWSimulator, NULL);

    SetLastRunTime();

    Task_ReadUART.task_func = ReadUART_func;
    Task_Telemetry.task_func = Telemetry_func;
    Task_Heartbeat.task_func = Heartbeat_func;

    BufferFill();

    return EXIT_SUCCESS;
}

// static void UART_enable() {
//     uart_peripheral.CTRL_REG |= 1;
// }

// static int UART_SetBaudRate(const uint32_t baud_rate) {
//     uart_peripheral.CTRL_REG &= ~(3 << 1);
//     switch (baud_rate) {
//         case 9600:
//             break;
//         case 115200:
//             uart_peripheral.CTRL_REG |= 1 << 1;
//             break;
//         case 1000000:
//             uart_peripheral.CTRL_REG |= 2 << 1;
//             break;
//         default:
//             return 1;
//     }
//     return 0;
// }

// static void UART_interrupt() {
//     uart_peripheral.CTRL_REG |= 1 << 3;
// }

// static void UART_SWReset() {
//     uart_peripheral.CTRL_REG |= 1 << 7;
//}

void *HWSimulator() {
    printf("HW Simulator initiated.");

    srand(time(NULL));

    while (1) {
        usleep(50000);
        uart_peripheral.DATA_REG = rand() % UINT32_MAX;
        uart_peripheral.STATUS_REG |= 1;
    }

    return NULL;
}

static void BufferFill() {
    while (1) {
        if ((uart_peripheral.STATUS_REG & 1) == 1) {
            const uint16_t next_head = (rx_head + 1) & 255;
            if (next_head == rx_tail) {
                uart_peripheral.STATUS_REG |= 1 << 1;
            } else {
                rx_buffer[rx_head] = uart_peripheral.DATA_REG;
                rx_head = next_head;
            }
            uart_peripheral.STATUS_REG &= ~1;
        }

        RunTasks();
    }
}

static void SetLastRunTime() {
    struct timespec last_run_time;
    clock_gettime(CLOCK_MONOTONIC, &last_run_time);
    const uint32_t last_run_time_ms = last_run_time.tv_sec * 1000 + last_run_time.tv_nsec / 1000000;
    Task_ReadUART.last_run_time = last_run_time_ms;
    Task_Heartbeat.last_run_time = last_run_time_ms;
    Task_Telemetry.last_run_time = last_run_time_ms;
}

static void RunTasks() {
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    const uint32_t current_time_ms = current_time.tv_sec * 1000 + current_time.tv_nsec / 1000000;
    if (current_time_ms - Task_ReadUART.last_run_time >= Task_ReadUART.period_ms) {
        Task_ReadUART.last_run_time = current_time_ms;
        Task_ReadUART.task_func();
    }
    if (current_time_ms - Task_Telemetry.last_run_time >= Task_Telemetry.period_ms) {
        Task_Telemetry.last_run_time = current_time_ms;
        Task_Telemetry.task_func();
    }
    if (current_time_ms - Task_Heartbeat.last_run_time >= Task_Heartbeat.period_ms) {
        Task_Heartbeat.last_run_time = current_time_ms;
        Task_Heartbeat.task_func();
    }
}

void ReadUART_func(void) {
    while (rx_tail != rx_head) {
        const uint8_t received_data = rx_buffer[rx_tail];
        rx_tail = (rx_tail + 1) & 255;
        bytes_processed++;
        printf("Received Data: %02X\n", received_data);
    }
}

void Telemetry_func(void) {
    printf("Bytes processed in the last 100 ms: %d\n", bytes_processed);
    bytes_processed = 0;
}

void Heartbeat_func(void) {
    printf("System OK\n");
}
