#include <stdint.h>
#include <stdio.h>

#include "data.h"

SimulatedUART_t uart_peripheral = {0};

static void UART_enable();

static int UART_SetBaudRate(uint32_t baud_rate);

static void UART_interrupt();

static void UART_SWReset();

int main(void) {
    printf("Hello, World!\n");
    return 0;
}

static void UART_enable() {
    uart_peripheral.CTRL_REG |= 1;
}

static int UART_SetBaudRate(const uint32_t baud_rate) {
    uart_peripheral.CTRL_REG &= ~(3 << 1);
    switch (baud_rate) {
        case 9600:
            break;
        case 115200:
            uart_peripheral.CTRL_REG |= 1 << 1;
            break;
        case 1000000:
            uart_peripheral.CTRL_REG |= 2 << 1;
            break;
        default:
            return 1;
    }
    return 0;
}

static void UART_interrupt() {
    uart_peripheral.CTRL_REG |= 1 << 3;
}

static void UART_SWReset() {
    uart_peripheral.CTRL_REG |= 1 << 7;
}
