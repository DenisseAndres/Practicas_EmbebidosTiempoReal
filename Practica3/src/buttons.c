#include "buttons.h"
#include "app_config.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "BUTTON";

typedef struct
{
    int stable_state;
    int last_raw;
    int count;

} Debounce_t;

//numero que define las lecturas necesarias para que el estado sea estable
#define DEBOUNCE_COUNT 3

//función para leer el estado del pin y evitar falsos positivos
static bool debounce_update(uint8_t gpio, Debounce_t *db)
{
    int raw;
    bool pressed_event;

    raw = gpio_get_level((gpio_num_t)gpio);
    pressed_event = false;

    //compara lectura actual con la anterior
    if (raw == db->last_raw)
    {
        //incrementar contador para confirmar que es estable
        if (db->count < DEBOUNCE_COUNT)
        {
            db->count++;
        }
    }
    else
    {
        //se reinicia el contador si el valor sí cambió
        db->count = 0;
        db->last_raw = raw;
    }
    //la señal se ha mantenido estable el tiempo determinado?
    if (db->count >= DEBOUNCE_COUNT)
    {
        //es diferente al estado estable anterior?
        if (raw != db->stable_state)
        {
            //actualiza el estado estable
            db->stable_state = raw;

            /*
               Botón normalmente abierto con pull-up:
               no presionado = 1
               presionado    = 0
            */
            if (db->stable_state == 0)
            {
                pressed_event = true;
            } 
            else 
            {
                pressed_event = false;
            }
        }
    }

    return pressed_event;
}

//función para monitorear los botones
void button_task(void *pvParameters)
{
    ButtonTaskParams_t *cfg = (ButtonTaskParams_t *)pvParameters;

    Debounce_t db =
    {
        .stable_state = 1,
        .last_raw = 1,
        .count = 0
    };
    //numero de pin para el boton
    gpio_reset_pin((gpio_num_t)cfg->gpio);
    gpio_set_direction((gpio_num_t)cfg->gpio, GPIO_MODE_INPUT);
    gpio_set_pull_mode((gpio_num_t)cfg->gpio, GPIO_PULLUP_ONLY);

    while (1)
    {
        //actualiza el estado del botón y verificar si se ha presionado
        if (debounce_update(cfg->gpio, &db))
        {
            /*
               NO modificamos directamente el contador aquí.
               Solo pedimos al Task_Manager que procese el evento.
            */
            switch (cfg->type)
            {
                case BUTTON_START_PAUSE:
					/*TODO : El manager cambia a MANAGER_EVENT_START_PAUSE*/
                    g_system.pending_event = MANAGER_EVENT_START_PAUSE; 
                    ESP_LOGI(TAG, "%c presionado", cfg->name);
                    break;

                case BUTTON_DIRECTION:
                    /*TODO : El manager cambia a MANAGER_EVENT_DIRECTION */
					g_system.pending_event = MANAGER_EVENT_DIRECTION;
                    ESP_LOGI(TAG, "%c presionado", cfg->name);
                    break;

                case BUTTON_SPEED:
					/*TODO : El manager cambia a MANAGER_EVENT_SPEED */
                    g_system.pending_event = MANAGER_EVENT_SPEED;
                    ESP_LOGI(TAG, "%c presionado", cfg->name);
                    break;

                default:
                    break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(BUTTON_POLL_MS));
    }
}