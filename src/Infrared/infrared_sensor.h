#ifndef INFRARED_SENSOR_H
#define INFRARED_SENSOR_H
#include "pico/stdlib.h"

#define IR_SENSOR_PIN 18
// Function to initialize the infrared sensor
void infrared_sensor_init();
// Function to read the state of the infrared sensor
bool infrared_sensor_is_triggered();
#endif
// INFRARED_SENSOR_H