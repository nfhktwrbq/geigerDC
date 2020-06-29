/*
 * counter.h
 *
 *  Created on: Jun 24, 2020
 *      Author: muaddib
 */

#ifndef INC_COUNTER_H_
#define INC_COUNTER_H_

#include "inttypes.h"

#define PULSE_INTERVALS_SIZE 10


void counter_EXTI_handler(uint32_t cnt);
void counter_TimerOverflow_handler(void);
uint32_t counter_getCounts(void);
uint32_t counter_getCountPeriod_us(uint8_t points);
uint32_t counter_getCountPeriod_ms(uint8_t points);
uint32_t counter_getCountPeriod_s(uint8_t points);
void counter_enableHV(void);
void counter_disableHV(void);

#endif /* INC_COUNTER_H_ */
