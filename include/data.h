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

typedef struct {
    void (*task_func)(void);
    uint16_t period_ms;
    uint32_t last_run_time;

} Task_t;

void ReadUART_func (void);
void Telemetry_func (void);
void Heartbeat_func (void);

#endif
