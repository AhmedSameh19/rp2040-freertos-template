#include "main.h"

int main() {
    stdio_init_all();  // Initialize stdio for UART or USB

    // Initialize hardware components
    setup_ldr_gl5528();
    setup_ultrasonic();
    infrared_sensor_init();
    motor_init();
    buzzer_init();
    pico_led_init(LED_PIN);

    // Create FreeRTOS tasks
    xTaskCreate(motor_control_task, "Motor Control", 1024, NULL, 1, NULL);
    xTaskCreate(ultrasonic_sensor_task, "Ultrasonic Sensor", 1024, NULL, 1, NULL);
    xTaskCreate(ldr_task, "LDR Task", 1024, NULL, 1, NULL);

    // Start the scheduler to run the tasks
    vTaskStartScheduler();

    // The program should never reach here
    while (1) {
        sleep_ms(1000);
    }

    return 0;
}

// Task to handle motor control based on infrared sensor input
void motor_control_task(void *pvParameters) {
    while (1) {
        if (!infrared_sensor_is_triggered()) {
            motor_stop(FRONT_LEFT);
            motor_stop(BACK_LEFT);
            motor_stop(FRONT_RIGHT);
            motor_stop(BACK_RIGHT);
        } else {
            motor_control(100, false, FRONT_LEFT);
            motor_control(100, false, BACK_LEFT);
            motor_control(100, false, FRONT_RIGHT);
            motor_control(100, false, BACK_RIGHT);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);  // Task delay (100ms)
    }
}

// Task to handle ultrasonic sensor measurements
void ultrasonic_sensor_task(void *pvParameters) {
    while (1) {
        send_trigger_pulse();
        uint32_t echo_time1 = measure_echo_time1();
        uint32_t echo_time2 = measure_echo_time2();
        float distance1 = calculate_distance(echo_time1);
        float distance2 = calculate_distance(echo_time2);
        printf("Distance1: %.2f cm\n", distance1);
        printf("Distance2: %.2f cm\n", distance2);

        vTaskDelay(500 / portTICK_PERIOD_MS);  // Task delay (500ms)
    }
}

// Task to handle LDR sensor reading and light intensity
void ldr_task(void *pvParameters) {
    while (1) {
        uint16_t raw_value = read_ldr_gl5528_raw();
        float voltage = ldr_gl5528_to_voltage(raw_value);
        float resistance = ldr_gl5528_to_resistance(voltage);
        float intensity = ldr_gl5528_to_light_intensity(resistance);
        printf("Light Intensity: %.2f\n", intensity);

        vTaskDelay(200 / portTICK_PERIOD_MS);  // Task delay (200ms)
    }
}