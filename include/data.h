//
// Created by kumpaan on 9/19/26.
//

#ifndef SIMULATED_AUTONOMOUS_SENSOR_HUB_DATA_H

#define SIMULATED_AUTONOMOUS_SENSOR_HUB_DATA_H

#include <stdint.h>

typedef struct {
    volatile uint32_t CTRL_REG;
    volatile uint32_t STATUS_REG;
    volatile uint32_t DATA_REG;
} SimulatedUART_t;

extern SimulatedUART_t uart_peripheral;

#endif
