/*
 * HCSR04.c
 *
 *  Created on: Dec 1, 2025
 *      Author: juras
 */


#include "HCSR04.h"
#include <math.h>


void HCSR04_Init(HCSR04_Sensor_HandleType* sensor,
				TIM_HandleTypeDef* trig_timer,
				uint32_t trig_channel,
				TIM_HandleTypeDef* echo_rising_timer,
				uint32_t echo_rising_channel,
				TIM_HandleTypeDef* echo_falling_timer,
				uint32_t echo_falling_channel){


	sensor -> TrigTimer = trig_timer;
	sensor -> TrigChannel = trig_channel;

	sensor -> EchoRisingEdgeTimer = echo_rising_timer;
	sensor -> EchoRisingEdgeChannel = echo_rising_channel;

	sensor -> EchoFallingEdgeTimer = echo_falling_timer;
	sensor -> EchoFallingEdgeChannel = echo_falling_channel;

}


/**
 * Zwraca wartosc dystansu z czujnika
 */




float HCSR04_Get_Distance(HCSR04_Sensor_HandleType* sensor){


	float difference = 0;


	sensor->CaptureValue1 = HAL_TIM_ReadCapturedValue(sensor->EchoRisingEdgeTimer, sensor->EchoRisingEdgeChannel);
	sensor->CaptureValue2 = HAL_TIM_ReadCapturedValue(sensor->EchoFallingEdgeTimer, sensor->EchoFallingEdgeChannel);

	difference = sensor->CaptureValue2 - sensor->CaptureValue1;

	sensor->Distance = (float)difference / 5800.0f;

    if(sensor->Distance > 0.25f) sensor->Distance = 0.25f;
    if(sensor->Distance < 0.02f) sensor->Distance = 0.02f;


        if(sensor->last_distance == 0.0f) sensor->Distance = sensor->Distance;
        else sensor->Distance = sensor->Distance *0.53  + sensor->last_distance * 0.47;

	sensor->last_distance = sensor->Distance;

    return sensor->Distance;
}



/*

// Zmienne statyczne do pamiętania historii (poza funkcją lub static wewnątrz)
// Warto, żeby były w strukturze czujnika, ale dla uproszczenia tutaj static w funkcji:

float HCSR04_Get_Distance(HCSR04_Sensor_HandleType* sensor){

    // Zmienne pamiętające stan poprzedni (static = nie giną po wyjściu z funkcji)
    static float last_valid_distance = 0.12f; // Startujemy od środka
    static int error_count = 0;               // Licznik błędów pod rząd

    // 1. Pobranie SUROWEGO pomiaru
    uint32_t val1 = HAL_TIM_ReadCapturedValue(sensor->EchoRisingEdgeTimer, sensor->EchoRisingEdgeChannel);
    uint32_t val2 = HAL_TIM_ReadCapturedValue(sensor->EchoFallingEdgeTimer, sensor->EchoFallingEdgeChannel);
    uint32_t raw_diff;

    if (val2 >= val1) raw_diff = val2 - val1;
    else raw_diff = (0xFFFF - val1) + val2;

    float raw_measurement = (float)raw_diff / 5800.0f;

    // ---------------------------------------------------------
    // 2. FILTR "ANTY-SZPILKOWY" (Rate Limiter)
    // ---------------------------------------------------------

    // Maksymalny skok, jaki uznajemy za fizycznie możliwy w 20ms
    // Np. 0.04m (4 cm). Jeśli skoczy więcej - to błąd.
    float max_jump = 0.03f;

    float jump = raw_measurement - last_valid_distance;
    if(jump < 0) jump = -jump; // Wartość bezwzględna (fabs)

    if (jump > max_jump) {
        // Wykryto nienaturalny skok (np. z 16cm na 25cm)
        error_count++;

        if(error_count < 3) {
            // Jeśli to 1 lub 2 błąd z rzędu -> IGNORUJEMY GO.
            // Udajemy, że kulka jest tam, gdzie była ostatnio.
            raw_measurement = last_valid_distance;
        } else {
            // Jeśli to już 3 błąd z rzędu -> może kulka faktycznie szybko poleciała?
            // Akceptujemy nową wartość, żeby system się nie zaciął.
            last_valid_distance = raw_measurement;
            error_count = 0;
        }
    } else {
        // Skok jest mały (normalny ruch) -> akceptujemy
        last_valid_distance = raw_measurement;
        error_count = 0;
    }

    // ---------------------------------------------------------

    // 3. Clamping (Fizyczne limity belki)
    if(raw_measurement > 0.26f) raw_measurement = 0.26f;
    if(raw_measurement < 0.02f) raw_measurement = 0.02f;

    // 4. Filtr EMA (Wygładzanie)
    float alpha = 0.15f;
    if(sensor->Distance == 0.0f) sensor->Distance = raw_measurement;
    else sensor->Distance = (sensor->Distance * (1.0f - alpha)) + (raw_measurement * alpha);

    return sensor->Distance;
}


*/
