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

#define Tp 0.04			// próbkowanie
#define PWM_OFFSET 500	// OFF_SET dla którego belka jest prosto, niepochylona (dla serwa 0 stopni)
#define RAD_TO_PWM 477

#define radianMin -0,523
#define radianMax 0,523
#define angleMin -30
#define angleMax 30


/**
 * 			Moje wyliczenie RAD_TO_PWM (moja logika)
 *
 * 			skoro silnik obraca się od środkowej pozycji o około +/- 30 stopni to jego zakres całkowity to 60 stopni
 * 			60 stopni to jest 1,0472 radiana
 * 			aby uzyskać te kąty w serwonapędzie to należy wysłać sygnał między 350 (dół), (środek to 600) oraz 850 (max, góra)
 * 			różnica zakresu to 500
 * 			teraz jak podzielimy 500 / 1,0472 = 477,46
 * 			czyli o tyle trzeba przemnożyć sygnał z regulatora aby zamienić radiany na sygnał PWM oraz dodać OFF_SET w przypadku gdy uchyb będzie
 * 			bliski zera to belka będzie w środkowej niepochylonej pozycji
 *
 */



typedef struct {

	float kp;             // Wzmocnienie proporcjonalne
	float ki;             // Wzmocnienie całkujące
	float kd;             // Wzmocnienie różniczkujące
	float N;              //  Współczynnik filtru członu D


	float error;		  // blad
	float prev_error;     // Poprzedni błąd (do obliczenia różniczki)
	float prev_derivative_output;	// poprzednia wartosc rozniczki

	float integral_sum;

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
