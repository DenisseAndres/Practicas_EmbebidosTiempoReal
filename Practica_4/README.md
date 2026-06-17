# integrantes del equipo
Maria Fernanda Serrano Rodríguez\
Denisse Andres Zúñiga

# preguntas 
1. ¿Cuál es la diferencia entre usar una variable global y una cola para comunicar datos?
Una variable global corre el riesgo de que muchas tareas simultáneas quieran escribir sobre esa variable cuando no está protegida por un mutex, y la cola es para hilos que gestiona el bloqueo y desbloqueo de las tareas,transfiriendo de manera ordenada los datos.
2. ¿Qué tarea queda bloqueada cuando espera datos de una cola?\
Queda bloqueada la tarea que recibe en la función xQueueReceive(). Espera datos hasta que lleguen nuevos si es que está vacío.\
3. ¿Por qué TaskManager debe concentrar las decisiones del sistema?\
Para evitar condiciones de carrera y garantizar que el servo y el sensor trabajen con sincronía.
4. ¿Por qué pvParameters es más flexible que crear una función distinta por tarea?\
Porque se reutiliza el código, es como diseñar una plantilla y solamente la instancias para asignarle los valores o parámetros de los componentes similares que ocupas.\
5. ¿Qué diferencia existe entre suspender una tarea y bloquearla esperando una cola?
Al suspender la tarea sale completamente de la planificación y no consumirá recursos, mientras que al bloquear la tarea entrará en pausa temporal esperando a que un evento suceda y el sistema operativo la despertará en cuanto el dato este disponible o si el tiempo expira.
6. ¿Qué efecto tiene aumentar el tamaño de la ventana del filtro de mediana?
   Nos ayuda aumentando la robustez del sistema, para que cualquier ruido no interfiera con las variaciones en la luz.
7. ¿Por qué un filtro de mediana rechaza picos mejor que un promedio simple?
   Por que al sumar todos los valores recibidos, con el promedio simple, habrá valores extremadamente altos o bajos, que alteren el resultado final. Mientras que en el filtro de mediana va a ordenar los valores de menor a mayor y tomará el valor central, y esto hace que los valores atípicos que nos arroje, no sean elegidos.
8. ¿Qué ocurre si se presiona Start-operation mientras el servo está en movimiento?
   El sistema la va a ignorar completamente, ya que la bloquea mientras el servo esté en movimiento, luego ya que alcance su ángulo objetivo y finalmente cuando pasen los 8 segundos, se inicia nuevamente la operación.
9. ¿Cómo se garantiza que el botón de velocidad solo funcione durante la operación?
    Porque la tarea del botón de velocidad permanece suspendida,  y sólo se reauda una vez que se haya presionado el botón que inicia la operación, y se vuelve a suspender en después del ciclo de los 8 segundos.
10. ¿Por qué un constexpr en C++ es preferible a #define para constantes tipadas?
    Porque en el define es sólo la sustitución de texto, mientras que en constexpr se va a evaluar el tiempo de compilación y se integrará completamente al lenguaje, siendo más segura, respetando los espcaios en los nombres y facilitando la detección de errores. 

