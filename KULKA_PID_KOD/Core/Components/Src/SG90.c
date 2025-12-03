/*
 * SG90.c
 *
 *  Created on: Dec 1, 2025
 *      Author: juras
 */

#include "SG90.h"


void SG90_Init(SG90_Servo_HandleType* servo,
				TIM_HandleTypeDef* pwm_timer,
				 uint32_t pwm_channel){

	servo -> PWM_Timer = pwm_timer;
	servo -> PWM_Channel = pwm_channel;

}





void SG90_Set_Angle(SG90_Servo_HandleType* servo){

__HAL_TIM_SET_COMPARE(servo->PWM_Timer, servo->PWM_Channel, servo->angle);

}

