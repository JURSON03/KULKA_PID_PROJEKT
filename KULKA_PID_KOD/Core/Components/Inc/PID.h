/*
 * PID.h
 *
 *  Created on: Dec 1, 2025
 *      Author: juras
 */

#ifndef COMPONENTS_INC_PID_H_
#define COMPONENTS_INC_PID_H_

#include "main.h"
#include "SG90.h"
#include "HCSR04.h"

#define Tp 0.1			// próbkowanie
#define PWM_OFFSET 500
#define RAD_TO_PWM 50



typedef struct {

	float kp;             // Wzmocnienie proporcjonalne
	float ki;             // Wzmocnienie całkujące
	float kd;             // Wzmocnienie różniczkujące
	float N;              //  Współczynnik filtru członu D


	float error;		  // blad
	float prev_error;     // Poprzedni błąd (do obliczenia różniczki)
	float prev_derivative_output;	// poprzednia wartosc rozniczki

	float PID_p;
	float PID_i;
	float PID_d;
	float PID_total;

	float output_limit_min;
	float output_limit_max;

	float distance_setpoint;

} PID_HandleType;


extern PID_HandleType PID_Controller;



void PID_Controller_Init();

void PID_Controller_Calculate(HCSR04_Sensor_HandleType* hcsr04_sensor,
							  SG90_Servo_HandleType* servo,
							  PID_HandleType* pid_controller);





#endif /* COMPONENTS_INC_PID_H_ */
