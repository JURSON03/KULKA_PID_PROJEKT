/*
 * Enkoder.c
 *
 *  Created on: Dec 25, 2025
 *      Author: juras
 */

#include "Enkoder.h"
#include "HCSR04.h"
#include "PID.h"



void Encoder_Init(Encoder_HandleType* encoder ,TIM_HandleTypeDef* encoder_timer,  uint32_t encoder_channel){

	encoder -> Encoder_Timer = encoder_timer;
	encoder -> Encoder_Channel = encoder_channel;


	encoder -> value = __HAL_TIM_GET_COUNTER(encoder -> Encoder_Timer);
	encoder -> last_value = encoder -> value;

	encoder -> min_value = 0;
	encoder -> max_value = 20;

	encoder -> Encoder_Change = 0;
}


int Encoder_GetValue(Encoder_HandleType* encoder){

	int current = __HAL_TIM_GET_COUNTER(encoder->Encoder_Timer);

	if(current != encoder->value){

	encoder->last_value = encoder->value;

	encoder->value = current;
	encoder -> Encoder_Change = 1;
	}
	else{
		encoder -> Encoder_Change = 0;
	}

	return encoder->value;

}

