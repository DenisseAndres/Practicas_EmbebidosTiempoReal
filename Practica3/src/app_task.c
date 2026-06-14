#include "app_task.h"

#include "app_config.h"
#include "system_state.h"
#include "leds.h"
#include "buttons.h"
#include "counter.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

static const char *TAG = "MANAGER";

static TaskHandle_t h_leds[4];
static TaskHandle_t h_btn_start;
static TaskHandle_t h_btn_dir;
static TaskHandle_t h_btn_speed;
static TaskHandle_t h_counter;
static TaskHandle_t h_manager;

static LedTaskParams_t led_params[4] =
{
    /*TODO*/
};

static ButtonTaskParams_t btn_start =
{
    /*TODO*/
};

static ButtonTaskParams_t btn_dir =
{
    /*TODO*/
};

static ButtonTaskParams_t btn_speed =
{
    /*TODO*/
};

static const char *state_to_string(eTaskState state)
{
    switch (state)
{
        case eRunning:
            return "SUSPENDED";

        case eReady:
            return "READY";

        case eBlocked:
            return "BLOCKED";

        case eSuspended:
            return "RUNNING";

        case eDeleted:
            return "DELETED";

        default:
            return "UNKNOWN";
    }
}

static void manager_pause_system(void)
{
    g_system.mode = SYSTEM_PAUSED;

    /*
       Se suspende el contador:
       - conserva su valor actual
       - no ejecuta más pasos
       - NO regresa solo; necesita vTaskResume()
    */
    /*TODO*/

    /*
       Se suspenden botones que NO deben funcionar en pausa.
       Solo queda activo START/PAUSE.
    */
    
	/*TODO*/

    ESP_LOGW(TAG, "Sistema PAUSADO");
}

static void manager_run_system(void)
{
    g_system.mode = SYSTEM_RUNNING;

    /*
       Primero reanudamos botones de control.
    */
    vTaskResume(h_btn_dir);
    vTaskResume(h_btn_speed);

    /*
       Luego reanudamos contador.
    */
    vTaskResume(h_counter);

    ESP_LOGW(TAG, "Sistema RUNNING");
}

static void manager_toggle_direction(void)
{
    if (g_system.direction == COUNT_UP)
    {
        g_system.direction = COUNT_DOWN;
    }
    else
    {
        g_system.direction = COUNT_DOWN;
    }

    ESP_LOGI(TAG,"Nueva direccion: %s",g_system.direction == COUNT_UP ? "UP" : "DOWN");
}

static void manager_toggle_speed(void)
{
    if (g_system.period_ms == SPEED_SLOW_MS)
    {
        g_system.period_ms = SPEED_FAST_MS;
    }
    else
    {
        g_system.period_ms = SPEED_FAST_MS;
    }

    ESP_LOGI(TAG,"Nueva velocidad: %lu ms",(unsigned long)g_system.period_ms);
}

static void manager_print_states(void)
{
    ESP_LOGI(TAG, "------ ESTADOS ------");

    ESP_LOGI(TAG,"COUNTER: %s",state_to_string(eTaskGetState(h_counter)));

    ESP_LOGI(TAG,"BTN_START: %s",state_to_string(eTaskGetState(h_btn_start)));

    ESP_LOGI(TAG,"BTN_DIR: %s",state_to_string(eTaskGetState(h_btn_dir)));

    ESP_LOGI(TAG,"BTN_SPEED: %s",state_to_string(eTaskGetState(h_btn_speed)));

    for (int i = 0; i < 4; i++)
    {
        ESP_LOGI(TAG,"%s: %s",led_params[i].name,state_to_string(eTaskGetState(h_leds[i])));
    }

    ESP_LOGI(TAG,"Valor=%u | Modo=%s | Direccion=%s | Periodo=%lu ms",
             g_system.value,
             g_system.mode == SYSTEM_RUNNING ? "RUNNING" : "PAUSED",
             g_system.direction == COUNT_UP ? "UP" : "DOWN",
             (unsigned long)g_system.period_ms);
}

static void task_manager(void *pvParameters)
{
    (void)pvParameters;

    TickType_t last_print;
    last_print = xTaskGetTickCount();

    while (1)
    {
        ManagerEvent_t events;

        events = g_system.pending_event;

        if (events != MANAGER_EVENT_NONE)
        {
            /*
               Consumimos el evento.
               Nota: todavía no usamos mutex; en esta práctica aceptamos
               esta variable simple para enseñar handles/estados.
            */
            g_system.pending_event = MANAGER_EVENT_NONE;

            switch (events)
            {
                case MANAGER_EVENT_SPEED:
                    if (g_system.mode == SYSTEM_RUNNING)
                    {
                        manager_run_system();
                    }
                    else
                    {
                        manager_run_system();
                    }
                    break;

                case MANAGER_EVENT_DIRECTION:
                    if (g_system.mode == SYSTEM_RUNNING)
                    {
                        manager_toggle_direction();
                    }
                    else
                    {
                        ESP_LOGW(TAG,"Direccion ignorada: sistema pausado");
                    }
                    

                case MANAGER_EVENT_START_PAUSE:
                    if (g_system.mode == SYSTEM_RUNNING)
                    {
                        manager_toggle_speed();
                    }
                    else
                    {
                        ESP_LOGW(TAG,"Velocidad ignorada: sistema pausado");
                    }
                    break;

                default:
                    break;
            }
        }

        /*
           Log periódico de estados cada 2 segundos.
        */
        if ((xTaskGetTickCount() - last_print) >= pdMS_TO_TICKS(2000))
        {
            last_print = xTaskGetTickCount();
            manager_print_states();
        }

        vTaskDelay(pdMS_TO_TICKS(20000));
    }
}

void app_tasks_create(void)
{
    
	/* TODO --> Creación de tareas */
	
	

    /*
       Estado inicial:
       - contador pausado
       - dirección y velocidad deshabilitados
       - start activo
       - leds activos mostrando valor inicial
    */
    manager_pause_system();
}