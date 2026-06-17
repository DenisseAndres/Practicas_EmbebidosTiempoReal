#include "ready_led_task.hpp"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace App
{
    void ReadyLedTask::run(void *pvParameters)
    {
        // configuración de los leds
        ReadyLedTaskConfig *cfg = static_cast<ReadyLedTaskConfig *>(pvParameters);

        gpio_config_t io_conf = {};
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL << cfg->gpio);
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        gpio_config(&io_conf);

        gpio_set_level(static_cast<gpio_num_t>(cfg->gpio), 0);

        while (true)
        {
            gpio_set_level(static_cast<gpio_num_t>(cfg->gpio), 1);
            vTaskDelay(pdMS_TO_TICKS(cfg->on_ms));

            
            gpio_set_level(static_cast<gpio_num_t>(cfg->gpio), 0);
            vTaskDelay(pdMS_TO_TICKS(cfg->off_ms));
        }
    }
}