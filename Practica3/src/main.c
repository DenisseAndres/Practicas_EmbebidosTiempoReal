#include "system_state.h"
#include "app_task.h"

void app_main(void)
{
	
	/*TODO --> Inicializar el sistema y crear tareas*/
	system_state_init(); //inicializar la memoria de los estados
	app_tasks_create(); //poner las tareas en el scheduler
}