/*
 * HCSR04.h
 *
 *  Created on: Dec 1, 2025
 *      Author: juras
 */

#ifndef COMPONENTS_INC_HCSR04_H_
#define COMPONENTS_INC_HCSR04_H_

#include "main.h"


/**
 *  Struktura czujnika zawierająca
 *  TrigPort - wskaźnik na port GPIO (A lub B)
 *  TrigPin - numer pinu trig czujnika
 *  EchoPort -  wskaźnik na port GPIO (A lub B)
 *  EchoPin - Numer pinu echo
 *  TimerHandle - Uchwyt do timera,
 *  CaptureValue1 -  Pomiar zbocza narastającego
 *	CaptureValue2 -	 Pomiar zbocza opadającego
 *	Distance - pomiar dystansu w [cm]
 *
 */


typedef struct {

	TIM_HandleTypeDef* TrigTimer;
	uint32_t TrigChannel;

	TIM_HandleTypeDef* EchoRisingEdgeTimer;
	uint32_t EchoRisingEdgeChannel;
	uint32_t CaptureValue1;

	TIM_HandleTypeDef* EchoFallingEdgeTimer;
	uint32_t EchoFallingEdgeChannel;
	uint32_t CaptureValue2;

	float Distance;
	float last_distance;

} HCSR04_Sensor_HandleType;




// Definicja

extern HCSR04_Sensor_HandleType HCSR04_Sensor;




/**
 * Funkcja inicjalizująca parametry czujnika HCSR-04
 */

void HCSR04_Init(HCSR04_Sensor_HandleType* sensor,
				TIM_HandleTypeDef* trig_timer,
				uint32_t trig_channel,
				TIM_HandleTypeDef* echo_rising_timer,
				uint32_t echo_rising_channel,
				TIM_HandleTypeDef* echo_falling_timer,
				uint32_t echo_falling_channel);




/**
 *  Funkcja zwracająca dystans pomiędzy kulką a czujnikiem
 */

float HCSR04_Get_Distance(HCSR04_Sensor_HandleType* sensor);






#endif /* COMPONENTS_INC_HCSR04_H_ */
