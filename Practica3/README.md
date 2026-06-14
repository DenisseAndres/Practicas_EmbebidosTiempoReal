Descripción de la práctica
La práctica consiste en un contador BCD del 0 al 9, un botón de dirección que indica si el contador va en forma ascendente o descendente, y un botón que alterna la velocidad entre 500ms y 250ms. Además se utiliza un botón para iniciar y pausar el contador.
Cuando el sistema está en pausa, no pueden haber cambios de velocidad ni de dirección, y cuando se reanude el sistema, el contador debe retomar donde se quedó.


## preguntas guía ##
1. ¿Qué diferencia existe entre BLOCKED y SUSPENDED?
2. ¿Por qué vTaskDelay() coloca una tarea en estado BLOCKED?
3. ¿Qué diferencia existe entre vTaskDelay() y un Software Timer?
4. ¿Qué función cumple el Idle Task?
5. ¿Cómo decide FreeRTOS cuál tarea ejecutar cuando varias tienen la misma prioridad?
6. ¿Qué ventajas aporta pvParameters?
7. ¿Qué ventajas aporta TaskHandle_t?
8. ¿Qué ocurriría si el contador se implementara con variables globales únicamente?
