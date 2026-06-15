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
- ¿Por que la variable g_ledRapido debe declararse como volatile? ¿Que ocurre si se omite esa palabra clave?\
Se declara de esa manera porque es una variable que se modifica por otras tareas. Se utiliza para marcar el estado activo o inactivo del parpadeo rápido, entonces ese cambio se hace en TaskMonitor y en TaskLedLento. Si se omite esa palabra clave podría ocasionar que cuando otras tareas lean el valor de g_ledRapido sea un valor desactualizado.

- ¿En que momento exacto aparece el mensaje [IDLE] en la terminal? Describe el estado de las cuatro tareas en ese instante.\
Aparece cuando las cuatro tareas están bloqueadas o suspendidas, que es cuando terminan los 5 segundos del Led lento:
TaskLedRapido se encuentra suspendida porque se presionó el botón, TaskLedLento, TaskSensor y TaskMonitor están bloqueadas porque están verificando lecturas cada cierto tiempo definido. 

- ¿Que diferencia existe entre vTaskDelay() y vTaskDelayUntil()? ¿En cual de las tareas de esta practica seria mas apropiado usar vTaskDelayUntil? \
La diferencia es que vTaskDelay() es una función que hace que la tarea espere cierto tiempo a partir del momento en que se llama la función, por lo que completar las instrucciones de la tarea pueden tomar distintas cantidades de tiempo, y un vTaskDelayUnit() hace un ajuste para que cada iteración tenga la misma duración. \
vTaskDelayUnit() se podría utilizar en la tarea del sensor porque adquirimos datos de un ADC a una frecuencia definida, así no habrían modificaciones de muestreo si un printf o la misma lectura toma más tiempo que alguna otra iteración.

- ¿Por que vTaskLedRapido tiene prioridad menor que vTaskMonitor? Describe que ocurriria si
se invirtieran esas prioridades.\
Porque como TaskMonitor se encarga de leer el estado del botón, tiene que hacerlo como prioridad por si el usuario sí presiona el botón, 


- ¿Que riesgo existe al leer una variable volatile desde dos tareas distintas sin proteccion?
Investiga el concepto de seccion critica.
