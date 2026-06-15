# Integrantes del equipo  
María Fernanda Serrano Rodríguez \
Denisse Andrés Zúñiga 

# Descripción de proyecto 
En esta primera práctica se implemenró FreeRTOS en una ESP32 para realizar el encendido de un led a distintas frecuencias.
Se planteron 3 estados principales:
- Rápido:
  El LED parpadea a 100ms
- Evento:
  Se ejecuta al presionar un botón (BOOT integrado en la tarjeta)
- Lento:
  El LED parpadea a 500ms y al mismo tiempo se imprime en pantalla la lectura de ADC con valores crudos y en voltaje.

Estando en el estado LENTO puede suceder una de estas dos cosas, al presionar o no presionar el botón:
Si se presiona dentro de los próximos 5 segundos, se regresa inmediatamente al estado rápido.
Si NO se presiona dentro de los próximos 5 segundos, las tareas pasan al estado IDLE e inmediatamente al estado rápido.

# Preguntas de análisis
- ¿Por que la variable g_ledRapido debe declararse como volatile? ¿Que ocurre si se omite esa
palabra clave?
- ¿En que momento exacto aparece el mensaje [IDLE] en la terminal? Describe el estado de las
cuatro tareas en ese instante. 
- ¿Que diferencia existe entre vTaskDelay() y vTaskDelayUntil()? ¿En cual de las tareas de esta
practica seria mas apropiado usar vTaskDelayUntil?
- ¿Por que vTaskLedRapido tiene prioridad menor que vTaskMonitor? Describe que ocurriria si
se invirtieran esas prioridades.
- ¿Que riesgo existe al leer una variable volatile desde dos tareas distintas sin proteccion?
Investiga el concepto de seccion critica.
