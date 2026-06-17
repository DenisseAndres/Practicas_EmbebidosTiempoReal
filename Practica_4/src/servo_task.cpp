#include "servo_task.hpp"
#include "app_context.hpp"
#include "messages.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include <cmath>

namespace App
{
    static uint32_t angle_to_duty(uint8_t angle, const ServoTaskConfig *cfg)
    {
        // limitar el ángulo entre 0 y 180
        if (angle > 180) angle = 180;
        
        uint32_t us = cfg->min_us + ((cfg->max_us - cfg->min_us) * angle) / 180;
        
        uint32_t max_duty = (1 << cfg->resolution) - 1;
        return (us * max_duty) / (1000000 / cfg->freq_hz);
    }

    void ServoTask::run(void *pvParameters)
    {
        ServoTaskConfig *cfg = static_cast<ServoTaskConfig *>(pvParameters);

        ledc_timer_config_t timer_conf = {};
        timer_conf.speed_mode       = cfg->mode;
        timer_conf.duty_resolution  = cfg->resolution;
        timer_conf.timer_num        = cfg->timer;
        timer_conf.freq_hz          = cfg->freq_hz;
        timer_conf.clk_cfg          = LEDC_AUTO_CLK;
        ledc_timer_config(&timer_conf);

        ledc_channel_config_t channel_conf = {};
        channel_conf.gpio_num       = cfg->gpio;
        channel_conf.speed_mode     = cfg->mode;
        channel_conf.channel        = cfg->channel;
        channel_conf.intr_type      = LEDC_INTR_DISABLE;
        channel_conf.timer_sel      = cfg->timer;
        channel_conf.duty           = angle_to_duty(0, cfg); // Iniciar en 0 grados
        channel_conf.hpoint         = 0;
        ledc_channel_config(&channel_conf);

        uint8_t current_angle = 0;
        ServoCmd current_cmd = {0, 0, 100, 1}; // Valores por defecto seguros
        bool is_moving = false;

        while (true)
        {
            ServoCmd new_cmd;
            TickType_t wait_ticks = is_moving ? 0 : portMAX_DELAY;

            
            if (xQueueReceive(g_queues.servo_cmd, &new_cmd, wait_ticks) == pdTRUE)
            {
                current_cmd = new_cmd;
                is_moving = true;
            }

            if (is_moving)
            {
                int diff = current_cmd.target_angle - current_angle;
            
                if (std::abs(diff) > current_cmd.tolerance_deg)
                {
                    // determinar dirección del paso
                    if (diff > 0)
                    {
                        current_angle += current_cmd.step_deg;
                        if (current_angle > current_cmd.target_angle) current_angle = current_cmd.target_angle;
                    }
                    else
                    {
                        if (current_angle > current_cmd.step_deg) current_angle -= current_cmd.step_deg;
                        else current_angle = 0;
                    }

                    // Actualizar PWM 
                    ledc_set_duty(cfg->mode, cfg->channel, angle_to_duty(current_angle, cfg));
                    ledc_update_duty(cfg->mode, cfg->channel);
                    ServoStatusMsg status_msg = {ServoStatusType::Moving, current_angle, current_cmd.target_angle, xTaskGetTickCount()};
                    xQueueSend(g_queues.servo_status, &status_msg, 0);
                    vTaskDelay(pdMS_TO_TICKS(current_cmd.step_delay_ms));
                }
                else
                {
                    // se alcanzó el objetivo 
                    is_moving = false;

                    ServoStatusMsg status_msg = {ServoStatusType::Reached, current_angle, current_cmd.target_angle, xTaskGetTickCount()};
                    xQueueSend(g_queues.servo_status, &status_msg, 0);
                }
            }
        }
    }
}