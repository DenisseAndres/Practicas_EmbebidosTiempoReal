
This directory is intended for project header files.

A header file is a file containing C declarations and macro definitions
to be shared between several project source files. You request the use of a
header file in your project source file (C, C++, etc) located in `src` folder
by including it, with the C preprocessing directive `#include'.

```src/main.c

#include "header.h"

int main (void)
{
 ...
}
```
Preguntas guia:
1. ¿Por qué es preferible pasar la configuración de canal mediante pvParameters en lugar de usar variables globales o funciones de tarea distintas para cada canal?
   porque al usar variables globales podemos tender a causar condiciones de carrea o sobrescritura accidental de los datos. Usar pvParameters nos previene de la duplicación de nuestro código y nos ahorra la memoria flash.
2. ¿Qué sucedería si los bloques task_params_t se declararan como variables locales en tasks_create_all() en lugar de static? ¿Cómo lo detectarías experimentalmente?
   Ya que las variables que hayas declarado se guardan en la memoria stack temporsal de la función, y cuando termine se liberará y se considerará basura y lo podemos detercar experimentalmente en lo que nos imprimiria en el monitor serial, porque nos arrojaría caracteres basura.
3. Las tres tareas tienen el mismo periodo (50 ms) pero prioridades distintas. ¿En qué situación se hace visible la diferencia de prioridad en la salida del terminal?
   Se es visible cuando al conincidir en el mismo tiempo los mensajes se imprimirán dn orden estricto de prioridad, siendo la 3 con la prioridad más alta.
4. ¿Por qué se usa vTaskDelay(pdMS_TO_TICKS(50)) en lugar de un for con retardo de software? ¿Qué
diferencia implica para el planificador?
   Se usa vTaskDelay(pdMS_TO_TICKS(50)) porque usar un for implica un mal manejo de estados y el uso del CPU. La diferencia que implica para el planificador es que al enviar una tarea a vTaskDelay(pdMS_TO_TICKS(50)) se manda la tarea al estado bloquedo, lo que permite darle en control al procesador, permitiendo a su vez que el planificar asigne el tiempo de ejecución a otras tareas de menor prioridad, y su fuera con el for lo que haría es wue el procesador desperdicia los ciclos de reloj y no oermite wue ninguna tarea de menor o igual prioridad se ejecute.
5. ¿Qué valor esperarías en el stack watermark de la tarea con menor prioridad comparado con el de mayor prioridad? ¿Por qué podrían diferir?
   Pues esperaría un valor más pequeño en comparación con el de mayor prioridad, ya que el watermark nos indica la cantidad de memoria libre mínima que le queda a la tarea durante si ejecución. Podría diferir en que la tarea con menor prioridad se puede pausar forzosamente en cualquier momento por las tareas de mayor prioridad y para una tarea de mayor prioridad, que rara vez es interrumpida, el stack no sufre ninguna carga de contexto extra (registros o contadores del programa).
6. Si se añadiera un cuarto potenciómetro y LED, ¿qué cambios mínimos requeriría el código dado el diseño modular actual y la restricción de no modificar las firmas existentes?
    Primero que nada tendríamos que agregar el nuevo canal para leer el ADC, declarar un cuarto bloque de parámetros, y aladir una cuarta llamada en tasks.c pasando la referencia de &params_ch3 y asignando su prioridad.

Including a header file produces the same results as copying the header file
into each source file that needs it. Such copying would be time-consuming
and error-prone. With a header file, the related declarations appear
in only one place. If they need to be changed, they can be changed in one
place, and programs that include the header file will automatically use the
new version when next recompiled. The header file eliminates the labor of
finding and changing all the copies as well as the risk that a failure to
find one copy will result in inconsistencies within a program.

In C, the convention is to give header files names that end with `.h'.

Read more about using header files in official GCC documentation:

* Include Syntax
* Include Operation
* Once-Only Headers
* Computed Includes

https://gcc.gnu.org/onlinedocs/cpp/Header-Files.html
