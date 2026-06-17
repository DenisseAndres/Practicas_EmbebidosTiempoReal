
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "task_manager.hpp"
#include "app_context.hpp"


extern "C" void app_main()
{
    App::app_tasks_create();
}