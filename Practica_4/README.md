# integrantes del equipo
Maria Fernanda Serrano Rodríguez\
Denisse Andres Zúñiga

# preguntas 
1. ¿Cuál es la diferencia entre usar una variable global y una cola para comunicar datos?
Una variable global corre el riesgo de que muchas tareas simultáneas quieran escribir sobre esa variable cuando no está protegida por un mutex, y la cola es para hilos que gestiona el bloqueo y desbloqueo de las tareas,transfiriendo de manera ordenada los datos.
  
3. ¿Qué tarea queda bloqueada cuando espera datos de una cola?
4. ¿Por qué TaskManager debe concentrar las decisiones del sistema?
5. ¿Por qué pvParameters es más flexible que crear una función distinta por tarea?
6. ¿Qué diferencia existe entre suspender una tarea y bloquearla esperando una cola?
7. ¿Qué efecto tiene aumentar el tamaño de la ventana del filtro de mediana?
8. ¿Por qué un filtro de mediana rechaza picos mejor que un promedio simple?
9. ¿Qué ocurre si se presiona Start-operation mientras el servo está en movimiento?
10. ¿Cómo se garantiza que el botón de velocidad solo funcione durante la operación?
11. ¿Por qué un constexpr en C++ es preferible a #define para constantes tipadas? 
