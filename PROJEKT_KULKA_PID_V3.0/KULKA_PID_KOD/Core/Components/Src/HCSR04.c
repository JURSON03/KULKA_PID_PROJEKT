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





void bubbleSort(float* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                float temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

float HCSR04_Get_Distance(HCSR04_Sensor_HandleType* sensor){

    float raw_difference = 0;
    float raw_distance = 0;

    // 1. Pobierz surowe dane
    sensor->CaptureValue1 = HAL_TIM_ReadCapturedValue(sensor->EchoRisingEdgeTimer, sensor->EchoRisingEdgeChannel);
    sensor->CaptureValue2 = HAL_TIM_ReadCapturedValue(sensor->EchoFallingEdgeTimer, sensor->EchoFallingEdgeChannel);

    // Obsługa przepełnienia timera (jeśli timer liczy w kółko)
    if (sensor->CaptureValue2 >= sensor->CaptureValue1) {
        raw_difference = sensor->CaptureValue2 - sensor->CaptureValue1;
    } else {
        // Zakładając, że ARR timera to np. 0xFFFF lub inna znana wartość
        // raw_difference = (0xFFFF - sensor->CaptureValue1) + sensor->CaptureValue2;
        // Jeśli ARR jest duże i rzadko się przepełnia przy 40ms, można to pominąć lub obsłużyć prościej:
        return sensor->last_distance; // Odrzuć pomiar w cyklu przepełnienia
    }

    raw_distance = (float)raw_difference / 5800.0f; // Wynik w [m] jeśli timer ma 1MHz

    // 2. Proste zabezpieczenie zakresu (Hardware limits)
    if(raw_distance > 1.0f) raw_distance = 1.0f; // Jeśli > 1m, to na pewno błąd w Ball&Beam
    if(raw_distance < 0.0f) raw_distance = 0.0f;

    // --- FILTR MEDIANOWY START ---

    // Przesuń bufor (FIFO) - wyrzuć najstarszy, zrób miejsce na nowy
    // Zakładamy, że w strukturze masz: float history[MEDIAN_WINDOW_SIZE];
    for(int i = 0; i < MEDIAN_WINDOW_SIZE - 1; i++) {
        sensor->history[i] = sensor->history[i+1];
    }
    // Dodaj nowy pomiar na koniec
    sensor->history[MEDIAN_WINDOW_SIZE - 1] = raw_distance;

    // Kopiuj do tymczasowej tablicy żeby posortować (nie psując kolejności historycznej)
    float sorted_buf[MEDIAN_WINDOW_SIZE];
    for(int i = 0; i < MEDIAN_WINDOW_SIZE; i++) {
        sorted_buf[i] = sensor->history[i];
    }

    // Sortuj
    bubbleSort(sorted_buf, MEDIAN_WINDOW_SIZE);

    // Wybierz środek
    float median_value = sorted_buf[MEDIAN_WINDOW_SIZE / 2];

    // --- FILTR MEDIANOWY STOP ---


    // 3. Opcjonalnie: Delikatne wygładzanie (EMA) na wyniku mediany
    // To usunie "schodki" wynikające z cyfrowej natury czujnika, ale mediana już usunęła "szpilki"
    // Możesz użyć mniejszej wagi dla starej wartości, bo mediana jest już stabilna.

    if(sensor->last_distance == 0.0f) sensor->Distance = median_value;
    else sensor->Distance = median_value * 0.7f + sensor->last_distance * 0.3f;

    // Ostateczne przycięcie do zakresu belki (jak u Ciebie)
    if(sensor->Distance >= 0.20f) sensor->Distance = 0.20f;
    if(sensor->Distance <= 0.02f) sensor->Distance = 0.02f;

    sensor->last_distance = sensor->Distance;

    return sensor->Distance;
}









/*

float HCSR04_Get_Distance(HCSR04_Sensor_HandleType* sensor){


	float difference = 0;


	sensor->CaptureValue1 = HAL_TIM_ReadCapturedValue(sensor->EchoRisingEdgeTimer, sensor->EchoRisingEdgeChannel);
	sensor->CaptureValue2 = HAL_TIM_ReadCapturedValue(sensor->EchoFallingEdgeTimer, sensor->EchoFallingEdgeChannel);

	difference = sensor->CaptureValue2 - sensor->CaptureValue1;

	sensor->Distance = (float)difference / 5800.0f;

    if(sensor->Distance >= 0.20f) sensor->Distance = 0.20f;
    if(sensor->Distance <= 0.02f) sensor->Distance = 0.02f;


    if(sensor->last_distance == 0.0f) sensor->Distance = sensor->Distance;
    else sensor->Distance = sensor->Distance *0.53  + sensor->last_distance * 0.47;

	sensor->last_distance = sensor->Distance;

    return sensor->Distance;
}

*/

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
