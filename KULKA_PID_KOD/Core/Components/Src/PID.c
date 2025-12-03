/*
 * PID.c
 *
 *  Created on: Dec 1, 2025
 *      Author: juras
 */


#include "PID.h"
#include "usart.h"

void PID_Controller_Init(PID_HandleType* pid_controller){

	pid_controller->kp = 1.982;
	pid_controller->ki = 0;
	pid_controller->kd = 11.578;
	pid_controller->N = 4.913;


	/*
	pid_controller->kp = 1.98;
	pid_controller->ki = 0.083;
	pid_controller->kd = 11.58;
	pid_controller->N = 8.043;
	*/

	pid_controller->error = 0;;
	pid_controller->prev_error = 0;
	pid_controller->prev_derivative_output = 0;

	pid_controller->PID_p = 0;
	pid_controller->PID_i = 0;
	pid_controller->PID_d = 0;
	pid_controller->PID_total = 0;

	pid_controller->output_limit_min = 350;
	pid_controller->output_limit_max = 850;		// 1000

	pid_controller->distance_setpoint = 0.12;		// wymuszenie 0.12 [m] = 12 [cm]
}







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






void PID_Controller_Calculate(HCSR04_Sensor_HandleType* hcsr04_sensor,
							  SG90_Servo_HandleType* servo,
							  PID_HandleType* pid_controller){


	pid_controller -> error = (pid_controller->distance_setpoint - hcsr04_sensor->Distance) *(-1) ;		// obliczanie uchybu


	pid_controller->PID_p = pid_controller->kp * pid_controller->error;


	// pid_controller->PID_i +=  pid_controller->ki * pid_controller->error * Tp;

	pid_controller->PID_i =  (pid_controller->PID_i + pid_controller-> error) * pid_controller->ki * Tp;

	if(pid_controller->PID_i > 300.0f) pid_controller->PID_i = 300.0f;
	else if(pid_controller->PID_i < -300.0f) pid_controller->PID_i = -300.0f;


	if (pid_controller->prev_error == 0.0) {
	        pid_controller->PID_d = 0.0;
	    }
		else {
//		pid_controller->PID_d = (pid_controller->kd * pid_controller->N * (pid_controller->error - pid_controller->prev_error)) + ((1.0f - pid_controller->N * Tp) * pid_controller->prev_derivative_output);
		pid_controller->PID_d = (pid_controller->error - pid_controller->prev_error)*pid_controller->kd / Tp;
    }

//	pid_controller->prev_derivative_output = pid_controller->PID_d;

//	pid_controller->PID_total =  PWM_OFFSET + RAD_TO_PWM*(pid_controller->PID_p + pid_controller->PID_d);
	pid_controller->PID_total =  PWM_OFFSET + RAD_TO_PWM*(pid_controller->PID_p + pid_controller->PID_i + pid_controller->PID_d);

	if(pid_controller->PID_total < pid_controller->output_limit_min){
	        pid_controller->PID_total =  pid_controller->output_limit_min;

	    }
	    else if(pid_controller->PID_total > pid_controller->output_limit_max){
	        pid_controller->PID_total = pid_controller->output_limit_max;

	    }


	servo->angle =  pid_controller->PID_total;
	pid_controller-> prev_error = pid_controller-> error;

}













