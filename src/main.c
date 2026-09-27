#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "data.h"

SimulatedUART_t uart_peripheral = {0};

static uint8_t rx_buffer[256];

// static void UART_enable();

// static int UART_SetBaudRate(uint32_t baud_rate);

// static void UART_interrupt();

// static void UART_SWReset();

static void* HWSimulator();

int main(void) {
    printf("Hello, World!\n");

    pthread_t HWSimulatorThread;
    pthread_create(&HWSimulatorThread,NULL, HWSimulator, NULL);

    uint16_t head = 0;
    uint16_t tail = 0;

    while (1) {
        if ((uart_peripheral.STATUS_REG & 1) == 1) {
            const uint16_t next_head = (head + 1) & 255;
            if (next_head == tail) {
                uart_peripheral.STATUS_REG |= 1 << 1;
            }
            else {
                rx_buffer[head] = uart_peripheral.DATA_REG;
                head = next_head;
            }
            uart_peripheral.STATUS_REG &= ~1;
        }
    }

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

void* HWSimulator() {
    printf("HW Simulator initiated.");

    srand(time(NULL));

    while (1) {
        usleep(50000);
        uart_peripheral.DATA_REG = rand() % UINT32_MAX;
        uart_peripheral.STATUS_REG |= 1;
    }

    return NULL;
}
