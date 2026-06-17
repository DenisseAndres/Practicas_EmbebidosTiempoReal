#include "leds.h"
#include "system_state.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

//tarea para un solo led y despues se instancia por cada led que se necesita
void led_task(void *pvParameters)
{
    LedTaskParams_t *cfg = (LedTaskParams_t *)pvParameters;

    gpio_reset_pin((gpio_num_t)cfg->gpio);
    gpio_set_direction((gpio_num_t)cfg->gpio, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)cfg->gpio, 0);

    while (1)
    {
        uint8_t bit_value;

		//para extraer el bit correspondiende, g_system value contiene el numero
        /*decimal, entonces los bits se pasan a la derecha cierta cantidad de posiciones
        poniendo el bit de interés como menos significativo*/
        bit_value = (g_system.value >> cfg->bit_position) & 0x01;

		/*TODO --> Después de actualizar el valor de bit_value, enviar a gpio*/
		gpio_set_level((gpio_num_t)cfg ->gpio, bit_value);
        

        /*
           Tarea ligera: actualiza LED periódicamente.
           Cuando no hay cambios, queda BLOCKED.
        */
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}