#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "pico/stdlib.h"
// #include "infrared_sensor.c"
#include "Led/LED.c"
#include "ultraSonic/ultrasonic_sensor.c"
#include "motor/motor_control.c"
#include "LDR/LDR.c"
#include "Buzzer/buzzer.c"
#include "Infrared/infrared_sensor.c"

#define LED_PIN 20
void motor_control_task(void *pvParameters);
void ultrasonic_sensor_task(void *pvParameters);
void ldr_task(void *pvParameters);