#include "system_state.h"
#include "app_config.h"

//estado del sistema
SystemState_t g_system;

//funcion para inicializar las variables de estado 
void system_state_init(void)
{
	/*TODO: 
		1. Establecer el valor de value a cero, 
		2. Establecer periodo_ms a una velocidad,
		3. Establecer dirección,
		4. Establecer modo,
		5. Establecer el estado del manager. :)*/
	g_system.value = 0; //contador BCD
	g_system.period_ms = SPEED_SLOW_MS; //velocidad inicial
	g_system.direction = COUNT_UP; // inicia incrementando
    g_system.mode = SYSTEM_PAUSED; 
    g_system.pending_event = MANAGER_EVENT_NONE; //Manager inicia sin eventos
}