#include "main.h"

// Function prototypes for FreeRTOS tasks
void motor_control_task(void *pvParameters);
void ultrasonic_sensor_task(void *pvParameters);
void ldr_sensor_task(void *pvParameters);
void led_buzzer_task(void *pvParameters);

// Global variables to share data between tasks

volatile float distance1 = 0, distance2 = 0;
volatile float light_intensity = 0;

int main() {
    // Initialize stdio and peripherals
    stdio_init_all();
    setup_ldr_gl5528();
    setup_ultrasonic();
    infrared_sensor_init();
    motor_init();
    buzzer_init();
    pico_led_init(20);

    // Create FreeRTOS tasks
    xTaskCreate(motor_control_task, "Motor Control", 1024, NULL, 1, NULL);
    xTaskCreate(ultrasonic_sensor_task, "Ultrasonic Sensor", 1024, NULL, 1, NULL);
    xTaskCreate(ldr_sensor_task, "LDR Sensor", 1024, NULL, 1, NULL);
    xTaskCreate(led_buzzer_task, "LED and Buzzer", 1024, NULL, 1, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // The program should never reach here
    while (1) {
        sleep_ms(1000);
    }
    return 0;
}

// Task to control motors based on the infrared sensor
void motor_control_task(void *pvParameters) {
    while (1) {
        if (!infrared_sensor_is_triggered()) {
            motor_stop(FRONT_LEFT);
            motor_stop(BACK_LEFT);
            motor_stop(FRONT_RIGHT);
            motor_stop(BACK_RIGHT);

        } else {
            motor_control(100, true, FRONT_LEFT);
            motor_control(100, true, BACK_LEFT);
            motor_control(100, true, FRONT_RIGHT);
            motor_control(100, true, BACK_RIGHT);

        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Run every 100ms
    }
}

// Task to measure distances using ultrasonic sensors
void ultrasonic_sensor_task(void *pvParameters) {
    while (1) {
        send_trigger_pulse();
        distance1 = calculate_distance(measure_echo_time1());
        distance2 = calculate_distance(measure_echo_time2());
        printf("Distance1: %.2f cm, Distance2: %.2f cm\n", distance1, distance2);
        vTaskDelay(200 / portTICK_PERIOD_MS); // Run every 200ms
    }
}

// Task to read light intensity from the LDR sensor
void ldr_sensor_task(void *pvParameters) {
    while (1) {
        uint16_t raw_value = read_ldr_gl5528_raw();
        float voltage = ldr_gl5528_to_voltage(raw_value);
        float resistance = ldr_gl5528_to_resistance(voltage);
        light_intensity = ldr_gl5528_to_light_intensity(resistance);
        printf("Light Intensity: %.2f\n", light_intensity);
        vTaskDelay(300 / portTICK_PERIOD_MS); // Run every 300ms
    }
}

// Task to control the LED and buzzer based on sensor data
void led_buzzer_task(void *pvParameters) {
    while (1) {
        if (light_intensity >= 50 && (distance1 < 10 || distance2 < 10)) {
            buzzer_beep(5000, 2000);
        } else if (light_intensity < 50 && (distance1 > 10 || distance2 > 10)) {
            pico_set_led(20, true);
        } else {
            pico_set_led(20, false);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Run every 100ms
    }
}