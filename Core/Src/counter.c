/*
 * counter.c
 *
 *  Created on: Jun 24, 2020
 *      Author: muaddib
 */

#include "counter.h"
#include "stm32f1xx_hal.h"
#include "string.h"

static uint32_t sCounter = 0;
static uint32_t sPulseIntervals[PULSE_INTERVALS_SIZE]={0,0,0,0,0,0,0,0,0,0};
static uint32_t sOwerflowCounter = 0;
static uint8_t sIndex = 0;
static uint32_t sSysFreq;

void counter_init(void)
{
	memset(sPulseIntervals, 0, PULSE_INTERVALS_SIZE * sizeof(sPulseIntervals[0]));
	sSysFreq = HAL_RCC_GetSysClockFreq();
}

void counter_EXTI_handler(uint32_t cnt)
{
	int32_t sPulseInterval = 0;
	static uint32_t prevTimerOverflow = 0;

	sCounter++;
	sPulseInterval = (sOwerflowCounter - prevTimerOverflow) * 0xffff + cnt;
	prevTimerOverflow = sOwerflowCounter;

	if(sIndex >= PULSE_INTERVALS_SIZE)
	{
		sIndex = 0;
	}

	sPulseIntervals[sIndex] = sPulseInterval;
	sIndex++;
}

void counter_TimerOverflow_handler(void)
{
	sOwerflowCounter++;
}


uint32_t counter_getCounts(void)
{
	return sCounter;
}

uint32_t _getCountPeriod(uint8_t points)
{
	uint64_t sum = 0;
	int i;

	if(sCounter < points)
		return 0;

	if(points > PULSE_INTERVALS_SIZE)
	{
		points = PULSE_INTERVALS_SIZE;
	}

	for(i = (points > sIndex) ? 0 : (sIndex - points); i < sIndex; i++)
		sum += sPulseIntervals[i];

	for(i = PULSE_INTERVALS_SIZE + sIndex - points; i < PULSE_INTERVALS_SIZE; i++)
		sum += sPulseIntervals[i];

	return sum / points;

}

uint32_t counter_getCountPeriod_us(uint8_t points)
{
	uint32_t cp = _getCountPeriod(points);
	if (sSysFreq >= 1000000 && sSysFreq)
	{
		uint32_t div = sSysFreq / 1000000;
		return cp / div;
	}
	return 0;
}

uint32_t counter_getCountPeriod_ms(uint8_t points)
{
	if (sSysFreq >= 1000 && sSysFreq)
	{
		uint32_t cp = _getCountPeriod(points);
		uint32_t div = sSysFreq / 1000;
		return cp / div;
	}
	return 0;
}

uint32_t counter_getCountPeriod_s(uint8_t points)
{
	uint32_t cp = _getCountPeriod(points);
	if(sSysFreq && cp)
	{
		return cp / sSysFreq;
	}
	return 0;
}

void counter_enableHV(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
}

void counter_disableHV(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
}



