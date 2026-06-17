# Integrantes del equipo
María Fernanda Serrano Rodríguez\
Denisse Andrés Zúñiga
# Descripción de la práctica
La práctica consiste en un contador BCD del 0 al 9, un botón de dirección que indica si el contador va en forma ascendente o descendente, y un botón que alterna la velocidad entre 500ms y 250ms. Además se utiliza un botón para iniciar y pausar el contador.
Cuando el sistema está en pausa, no pueden haber cambios de velocidad ni de dirección, y cuando se reanude el sistema, el contador debe retomar donde se quedó.


## preguntas guía ##
1. ¿Qué diferencia existe entre BLOCKED y SUSPENDED?\
En blocked, la tarea está esperando un evento o tiempo determinado, mientras que Suspended es cuando entra a ese estado solamente cuando se le indica mediante la función vTaskSuspend(), haciendo que se congele por completo y el scheduler la ignora hasta que se llame a vTaskResume() para que salga de ese estado.\
2. ¿Por qué vTaskDelay() coloca una tarea en estado BLOCKED? \
   Porque como se mencionó anteriormente, la tarea está esperando un evento o tiempo determinado, liberando esas milésimas de segundo para dedicarlas a otra tarea si es necesario.
3. ¿Qué diferencia existe entre vTaskDelay() y un Software Timer?\
que vTaskDelay() es una función bloqueante, y el software timer es asíncrono, o sea que el tiempo corre en segundo plano y no afecta a la tarea que lo utiliza o que lo inicia.\
4. ¿Qué función cumple el Idle Task?\
Que cuando todas las tareas están suspendidas o bloqueadas, se ejecuta el idle task, que es para garantizar que haya una instrucción para validar y librerar memoria.\
5. ¿Cómo decide FreeRTOS cuál tarea ejecutar cuando varias tienen la misma prioridad?\
Cuando tienen la misma prioridad se recurre al scheduler Round-Robin que consiste en repartir el tiempo del CPU de forma equitativa.\
6. ¿Qué ventajas aporta pvParameters?\
Nos permite establecer una "plantilla" para solamente pasarle las configuraciones deseadas si ocupamos funciones iguales varias veces, como encender leds o tilizar botones. \
7. ¿Qué ventajas aporta TaskHandle_t?\
  Nos permite trabajar con una tarea desde cualquier parte del programa porque es un apuntador, entonces se puede controlar qué hace una tarea si presiono un botón que especifiqué en otra parte del código. \
9. ¿Qué ocurriría si el contador se implementara con variables globales únicamente?\
Se corre el riesgo que dos tareas o más se peleen por actualizar una misma variable, y no se tiene el control exacto de cuándo ocurren ciertos eventos.
