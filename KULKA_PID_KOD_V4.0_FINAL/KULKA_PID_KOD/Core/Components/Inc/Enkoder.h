/*
 * Enkoder.h
 *
 *  Created on: Dec 25, 2025
 *      Author: juras
 */

#ifndef COMPONENTS_INC_ENKODER_H_
#define COMPONENTS_INC_ENKODER_H_

#include "main.h"

typedef struct{

	int value;
	int last_value;

	int min_value;
	int max_value;

	TIM_HandleTypeDef* Encoder_Timer;
	uint32_t Encoder_Channel;

	_Bool Encoder_Change;


} Encoder_HandleType;

extern Encoder_HandleType Enkoder;

void Encoder_Init(Encoder_HandleType* encoder ,TIM_HandleTypeDef* encoder_timer,  uint32_t encoder_channel);

int Encoder_GetValue(Encoder_HandleType* encoder);


#endif /* COMPONENTS_INC_ENKODER_H_ */
