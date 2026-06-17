#include "button_task.hpp"
#include "app_context.hpp"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace App
{
    void ButtonTask::run(void *pvParameters)
    {
        ButtonTaskConfig *cfg = static_cast<ButtonTaskConfig *>(pvParameters);

        gpio_config_t io_conf = {};
        io_conf.intr_type = GPIO_INTR_DISABLE;      
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = (1ULL << cfg->gpio);
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        gpio_config(&io_conf);

        bool last_state = true; 

        while (true)
        {
            // leer el nivel actual del pin
            bool current_state = (gpio_get_level(static_cast<gpio_num_t>(cfg->gpio)) == 1);
            
            // Para lógica negativa
            bool is_pressed = !current_state; 

            // el estado ha cambiado?
            if (current_state != last_state)
            {
            
                if (cfg->send_on_change_only)
                {
                    
                    if (cfg->event_type == ButtonEventType::SpeedState || 
                       (cfg->event_type == ButtonEventType::Start && is_pressed))
                    {
                        ButtonMsg msg = {};
                        msg.type = cfg->event_type;
                        msg.pressed = is_pressed;
                        msg.tick = xTaskGetTickCount();


                        xQueueSend(g_queues.buttons, &msg, 0);
                    }
                }
            }

            // actualizar el estado anterior para la siguiente lectura
            last_state = current_state;

            vTaskDelay(pdMS_TO_TICKS(cfg->poll_ms));
        }
    }
}