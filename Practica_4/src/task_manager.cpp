#include "task_manager.hpp"
#include "app_context.hpp"
#include "app_config.hpp"
#include "messages.hpp"
#include "sensor_task.hpp"
#include "button_task.hpp"
#include "servo_task.hpp"
#include "ready_led_task.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

static const char *TAG = "TaskManager";

namespace App
{
    
    static SensorTaskConfig sensor_cfg = {
        AppConfig::LDR_ADC_UNIT, AppConfig::LDR_ADC_CHANNEL, 
        AppConfig::SENSOR_PERIOD_MS, AppConfig::FILTER_WINDOW_SIZE, "Task_Sensor"
    };

    static ServoTaskConfig servo_cfg = {
        AppConfig::SERVO_GPIO, AppConfig::SERVO_PWM_CHANNEL, AppConfig::SERVO_PWM_TIMER,
        AppConfig::SERVO_PWM_MODE, AppConfig::SERVO_PWM_RES_BITS, AppConfig::SERVO_PWM_FREQ_HZ,
        AppConfig::SERVO_MIN_US, AppConfig::SERVO_MAX_US, "Task_Servo"
    };

    static ButtonTaskConfig start_btn_cfg = {
        AppConfig::START_BUTTON_GPIO, "Task_BtnStart", ButtonEventType::Start, 
        AppConfig::BUTTON_POLL_MS, true
    };

    static ButtonTaskConfig speed_btn_cfg = {
        AppConfig::SPEED_BUTTON_GPIO, "Task_BtnSpeed", ButtonEventType::SpeedState, 
        AppConfig::BUTTON_POLL_MS, true
    };

    static ReadyLedTaskConfig ready_led_cfg = {
        AppConfig::READY_LED_GPIO, AppConfig::READY_LED_ON_MS, AppConfig::READY_LED_OFF_MS, "Task_ReadyLed"
    };

    static ManagerTaskConfig manager_cfg = {
        AppConfig::HOLD_TARGET_MS, AppConfig::SERVO_TOLERANCE_DEG, 
        AppConfig::SERVO_DELAY_SLOW_MS, AppConfig::SERVO_DELAY_FAST_MS, 
        AppConfig::SERVO_STEP_DEG, "TaskManager"
    };

    void app_tasks_create()
    {
        
        g_queues.sensor = xQueueCreate(5, sizeof(SensorMsg));
        g_queues.buttons = xQueueCreate(AppConfig::BUTTON_QUEUE_LEN, sizeof(ButtonMsg));
        g_queues.servo_cmd = xQueueCreate(5, sizeof(ServoCmd));
        g_queues.servo_status = xQueueCreate(5, sizeof(ServoStatusMsg));

        
        xTaskCreate(SensorTask::run, sensor_cfg.name, 4096, &sensor_cfg, 3, &g_handles.sensor);
        xTaskCreate(ServoTask::run, servo_cfg.name, 4096, &servo_cfg, 3, &g_handles.servo);
        xTaskCreate(ButtonTask::run, start_btn_cfg.name, 2048, &start_btn_cfg, 4, &g_handles.start_button);
        xTaskCreate(ButtonTask::run, speed_btn_cfg.name, 2048, &speed_btn_cfg, 4, &g_handles.speed_button);
        xTaskCreate(ReadyLedTask::run, ready_led_cfg.name, 2048, &ready_led_cfg, 1, &g_handles.ready_led);
        xTaskCreate(TaskManager::run, manager_cfg.name, 4096, &manager_cfg, 5, &g_handles.manager);

        
        vTaskSuspend(g_handles.sensor);
        vTaskSuspend(g_handles.servo);
        vTaskSuspend(g_handles.speed_button);
    }

    
    enum class SystemState { READY, MOVING, HOLDING };

    void TaskManager::run(void *pvParameters)
    {
        ManagerTaskConfig *cfg = static_cast<ManagerTaskConfig *>(pvParameters);
        SystemState current_state = SystemState::READY;
        
        uint32_t hold_start_tick = 0;
        uint32_t last_log_tick = xTaskGetTickCount();
        bool is_fast_mode = false;
        uint8_t current_target_angle = 255; // Inválido para forzar la primera actualización

        while (true)
        {
            
            ButtonMsg btn_msg;
            if (xQueueReceive(g_queues.buttons, &btn_msg, 0) == pdTRUE)
            {
                if (btn_msg.type == ButtonEventType::Start && current_state == SystemState::READY)
                {
                    ESP_LOGI(TAG, "Iniciando operacion...");
                    current_state = SystemState::MOVING;
                    
                    // Transición de estado: Administrar tareas mediante sus handles
                    vTaskSuspend(g_handles.ready_led);
                    vTaskSuspend(g_handles.start_button);
                    
                    // Asegurar que el servo no tenga basura en su cola y reanudar tareas
                    xQueueReset(g_queues.servo_cmd);
                    xQueueReset(g_queues.sensor);
                    
                    vTaskResume(g_handles.sensor);
                    vTaskResume(g_handles.servo);
                    vTaskResume(g_handles.speed_button);
                    
                    is_fast_mode = false;
                    current_target_angle = 255; 
                }
                else if (btn_msg.type == ButtonEventType::SpeedState && current_state == SystemState::MOVING)
                {
                    is_fast_mode = btn_msg.pressed;
                    ESP_LOGI(TAG, "Velocidad cambiada a: %s", is_fast_mode ? "RAPIDA" : "LENTA");
                    
                    
                    if (current_target_angle != 255)
                    {
                        ServoCmd cmd = {
                            current_target_angle, cfg->tolerance_deg, 
                            (uint16_t)(is_fast_mode ? cfg->fast_delay_ms : cfg->slow_delay_ms), 
                            cfg->step_deg
                        };
                        xQueueSend(g_queues.servo_cmd, &cmd, 0);
                    }
                }
            }

            SensorMsg sensor_msg;
            if (xQueueReceive(g_queues.sensor, &sensor_msg, 0) == pdTRUE)
            {
                if (current_state == SystemState::MOVING)
                {
                    // si la luz cambia y el objetivo es diferente, actualizamos el servo
                    if (sensor_msg.target_angle != current_target_angle)
                    {
                        current_target_angle = sensor_msg.target_angle;
                        ESP_LOGI(TAG, "Nuevo objetivo detectado: %d grados (ADC: %d)", current_target_angle, sensor_msg.filtered);
                        
                        ServoCmd cmd = {
                            current_target_angle, cfg->tolerance_deg, 
                            (uint16_t)(is_fast_mode ? cfg->fast_delay_ms : cfg->slow_delay_ms), 
                            cfg->step_deg
                        };
                        xQueueSend(g_queues.servo_cmd, &cmd, 0);
                    }
                }
            }

            ServoStatusMsg status_msg;
            if (xQueueReceive(g_queues.servo_status, &status_msg, 0) == pdTRUE)
            {
                if (current_state == SystemState::MOVING && status_msg.status == ServoStatusType::Reached)
                {
                    ESP_LOGI(TAG, "Objetivo alcanzado. Iniciando espera de 8 segundos.");
                    current_state = SystemState::HOLDING;
                    hold_start_tick = xTaskGetTickCount();
                }
            }

            uint32_t current_tick = xTaskGetTickCount();

            //mantener posición 8 segundos
            if (current_state == SystemState::HOLDING)
            {
                if ((current_tick - hold_start_tick) >= pdMS_TO_TICKS(cfg->hold_target_ms))
                {
                    ESP_LOGI(TAG, "Operacion finalizada. Volviendo a estado READY.");
                    current_state = SystemState::READY;
                    
                    
                    vTaskSuspend(g_handles.sensor);
                    vTaskSuspend(g_handles.servo);
                    vTaskSuspend(g_handles.speed_button);
                    
                    xQueueReset(g_queues.buttons);
                    
                    vTaskResume(g_handles.start_button);
                    vTaskResume(g_handles.ready_led);
                }
            }

            if ((current_tick - last_log_tick) >= pdMS_TO_TICKS(3000))
            {
                const char* state_str = (current_state == SystemState::READY) ? "READY" : 
                                        (current_state == SystemState::MOVING) ? "MOVING" : "HOLDING";
                ESP_LOGI(TAG, "--- Estado del sistema: %s ---", state_str);
                last_log_tick = current_tick;
            }

            vTaskDelay(pdMS_TO_TICKS(20));
        }
    }
}