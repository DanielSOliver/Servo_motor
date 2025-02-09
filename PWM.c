#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22 // GPIO do servo
#define LED_PIN 12   // GPIO do LED RGB
#define PWM_FREQ 50  // Frequência de 50Hz

// Função para configurar o PWM
void pwm_setup(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 100.0f); // Ajuste para 50Hz // Ajuste fino do clock
    pwm_config_set_wrap(&config, 25000); // Ajuste para período de 20ms // Para 50Hz com clock ajustado
    pwm_init(slice, &config, true);
}

// Função para definir a posição do servo
void set_servo_position(uint gpio, uint pulse_width_us) {
    uint slice = pwm_gpio_to_slice_num(gpio);
    uint level = (pulse_width_us * 25000) / 20000; // Correção do cálculo de nível PWM // Conversão para PWM
    pwm_set_gpio_level(gpio, level);
}

int main() {
    stdio_init_all();
    pwm_setup(SERVO_PIN);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        // Posição 180°
        set_servo_position(SERVO_PIN, 2400);
        gpio_put(LED_PIN, 1);
        sleep_ms(5000);

        // Posição 90°
        set_servo_position(SERVO_PIN, 1470);
        gpio_put(LED_PIN, 0);
        sleep_ms(5000);

        // Posição 0°
        set_servo_position(SERVO_PIN, 500);
        gpio_put(LED_PIN, 1);
        sleep_ms(5000);

        // Movimento suave de 0° a 180° e vice-versa
        for (int pos = 500; pos <= 2400; pos += 5) {
            set_servo_position(SERVO_PIN, pos);
            sleep_ms(10);
        }
        for (int pos = 2400; pos >= 500; pos -= 5) {
            set_servo_position(SERVO_PIN, pos);
            sleep_ms(10);
        }
    }
}
