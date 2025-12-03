/*
 * SG90.h
 *
 *  Created on: Dec 1, 2025
 *      Author: juras
 */

#ifndef COMPONENTS_INC_SG90_H_
#define COMPONENTS_INC_SG90_H_

#include "main.h"


#define SG90_PWM_MIN 350
#define SG90_PWM_MAX 1000

typedef struct {

	TIM_HandleTypeDef* PWM_Timer;
	uint32_t PWM_Channel;

	float angle;


} SG90_Servo_HandleType;


// Definicja

extern SG90_Servo_HandleType SG90_Servo;


/**
 * Funkcja inicjalizująca parametry Serwonapędu SG90
 */

void SG90_Init(SG90_Servo_HandleType* servo,
			   TIM_HandleTypeDef* PWM_Timer,
			   uint32_t PWM_Channel);



/**
 *  Funkcja zmieniająca kąt Serwonapędu SG90
 */


void SG90_Set_Angle(SG90_Servo_HandleType* servo);




#endif /* COMPONENTS_INC_SG90_H_ */
