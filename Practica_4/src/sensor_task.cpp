#include "sensor_task.hpp"
#include "app_context.hpp"
#include "app_config.hpp"
#include "messages.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include <vector>
#include <algorithm>
#include "esp_log.h"

namespace App
{
    void SensorTask::run(void *pvParameters)
    {
        
        SensorTaskConfig *cfg = static_cast<SensorTaskConfig *>(pvParameters);

        //configuración del adc
        adc_oneshot_unit_handle_t adc_handle;
        adc_oneshot_unit_init_cfg_t init_config = {};
        init_config.unit_id = cfg->unit_id;
        adc_oneshot_new_unit(&init_config, &adc_handle);

        adc_oneshot_chan_cfg_t config = {};
        config.bitwidth = ADC_BITWIDTH_DEFAULT;
        config.atten = ADC_ATTEN_DB_12;
        adc_oneshot_config_channel(adc_handle, cfg->channel, &config);

        std::vector<int> samples(cfg->filter_window);

        while (true)
        {
            for (uint8_t i = 0; i < cfg->filter_window; ++i)
            {
                adc_oneshot_read(adc_handle, cfg->channel, &samples[i]);
                vTaskDelay(pdMS_TO_TICKS(5)); // Pequeña pausa entre muestras del mismo bloque
            }

            std::vector<int> sorted_samples = samples;
            std::sort(sorted_samples.begin(), sorted_samples.end());
            int median_val = sorted_samples[cfg->filter_window / 2];
                
            // poca luz -> 0 grados, luz suficiente -> 180 grados
            // con poca luz era lectura de 1500, con mucha luz era de 3000 aproximadamente
            uint8_t target = (median_val > 1527) ? AppConfig::SERVO_ANGLE_DARK : AppConfig::SERVO_ANGLE_LIGHT;

            SensorMsg msg = {};
            msg.raw = samples[0]; 
            msg.filtered = median_val;
            msg.target_angle = target;
            msg.tick = xTaskGetTickCount();

            xQueueSend(g_queues.sensor, &msg, 0);
            vTaskDelay(pdMS_TO_TICKS(cfg->period_ms));
        }

        adc_oneshot_del_unit(adc_handle);
        vTaskDelete(nullptr);
    }
}