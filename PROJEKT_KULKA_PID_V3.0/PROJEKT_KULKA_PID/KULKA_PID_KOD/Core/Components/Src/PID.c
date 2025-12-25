/*
 * PID.c
 *
 *  Created on: Dec 1, 2025
 *      Author: juras
 */


#include "PID.h"
#include "usart.h"
#include "math.h"

void PID_Controller_Init(PID_HandleType* pid_controller){




	pid_controller->kp = 750;
	pid_controller->ki = 350;
	pid_controller->kd = 600;


	/*

	pid_controller->kp = 750;
	pid_controller->ki = 350;
	pid_controller->kd = 600;

	 */


	pid_controller->error = 0;
	pid_controller->prev_error = 0;
//	pid_controller->prev_derivative_output = 0;

	pid_controller->integral_sum = 0;

	pid_controller->PID_p = 0;
	pid_controller->PID_i = 0;
	pid_controller->PID_d = 0;
	pid_controller->PID_total = 0;

	pid_controller->output_limit_min = 750;			// 650
	pid_controller->output_limit_max = 1050;		// 1050

	pid_controller->distance_setpoint = 0.11;		// wymuszenie 0.12 [m] = 12 [cm]
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


	pid_controller -> error = pid_controller->distance_setpoint - hcsr04_sensor->Distance;

	pid_controller->PID_p = pid_controller->kp * pid_controller->error;

	pid_controller->PID_i +=  pid_controller->ki * pid_controller->error * Tp;


	if(pid_controller->PID_i >= 200) pid_controller->PID_i  = 200;
	else if(pid_controller->PID_i <= - 200) pid_controller->PID_i = - 200;


	pid_controller->PID_d = ( pid_controller->kd *(pid_controller->error - pid_controller->prev_error) ) / Tp;


	pid_controller->PID_total =  PWM_OFFSET - (pid_controller->PID_p + pid_controller->PID_i + pid_controller->PID_d);

	if(pid_controller->PID_total < pid_controller->output_limit_min){
	        pid_controller->PID_total =  pid_controller->output_limit_min;

	    }
	    else if(pid_controller->PID_total > pid_controller->output_limit_max){
	        pid_controller->PID_total = pid_controller->output_limit_max;

	    }



	servo->angle =  pid_controller->PID_total;

	pid_controller-> prev_error = pid_controller-> error;


}






void PID_Set_DistanceSetPoint( PID_HandleType* pid_controller, int new_setpoint){

	float converted_val = (float)new_setpoint / 100.0f;
	pid_controller->distance_setpoint = converted_val;

}






