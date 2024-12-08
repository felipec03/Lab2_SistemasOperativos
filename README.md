# Laboratorio 2 - Sistemas Operativos Universidad de Santiago de Chile
***
> En este laboratorio se verá una implementación rudimentaria de **pipelining** en el lenguaje C orientado a librerías UNIX usando procesos, pipes y otras funcionas propias de `<unistd.h>`.
> El propósito de esta experiencia es de entender cómo la consola (bash) utiliza el concepto de procesos y lo aplica a algo de suma utilidad cómo lo es la concatenación de operaciones por consola.

# Ejecución del laboratorio
Cómo fue estipulado en clases, para poder probar los comandos es necesario el uso de `makefile`, por lo que el comando para la generación de los archivos ejecutables es:
```
make all
```

Para limpiar los archivos residuales post ejecución de los archivos, se utiliza:
```
make clean
```
## Consideraciones de Ejecución
> Cómo se mencionó mas adelante, para la correcta ejecución **es necesario el uso de comillas seguido del comando** `./lab2`, por ejemplo:


**Comandos válidos**
- `./lab2 "./srep -i input.txt -s x -S y | ./srep -s a -S b | ./srep -s c -S d"`
- `./lab2 "./srep -i input.txt -s x -S y | ./srep -s a -S b | ./srep -s c -S d | ./srep -o outputMalevolo.txt -s y -S x"`

**Comandos inválidos**
- `./lab2 ./srep -i input.txt -s x -S y | ./srep -s a -S b | ./srep -s c -S d`
- `./lab2 ./srep -i input.txt -s x -S y | ./srep -s a -S b | ./srep -s c -S d | ./srep -o outputMalevolo.txt -s y -S x`

# Especificación de Archivos
> Para esta sección, se reutilizó makefile de laboratorio anterior, lo único que difiere es el ejecutable `./lab2`.

## ```lab2.c```
> Este archivo cumple la función de simular el pipelining en una terminal (bash) de un sistema basado en UNIX, este sigue la siguiente secuencia de pasos:

- Tomar por consola el string por medio de `argv[0]`, **esto implica que para el funcionamiento correcto del laboratorio todo lo seguido después de la invocación de `./lab2` debe ser escrito entre comillas.** Luego esta es parseada por `'|'` y guardada en un arreglo de punteros a caracter llamada `comandos`.

- Se itera por sobre el arreglo de comandos, creando un pipe para cada dupla de comandos, osease $comandos -1$ pipes, mientras que se deben almacenar los $2 \times comandos$ descriptores en el respectivo arreglo `fd[]`

- Finalmente, por medio de `fork()` cada proceso hijo le corresponde un comando, donde este por medio de `dup2()` se lee el comando y finalmente es ejecutado el proceso invocado por medio de `execvp()`, considerando además el caso donde no se encuentre el PATH, por eso se recurre en las últimas líneas al comando `setenv()` (https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-setenv-add-delete-change-environment-variables)

- Por último y no menos importante, para el correcto manejo de procesos se cierran los pipes y procesos por medio de `close()` y `wait()`. 

## ```archivos.c```
> Este archivo contiene varias implentaciones de funciones típicas de la biblioteca ```<string.h>``` y otras funciones de utilida general. Ejemplos cómo el string copy, string length, string concatenation, transformacion de string a arreglo, entre otros. 

> Más importantemente, contiene una estructura para representación de archivos CSV cómo triple puntero, aptamente llamada ```CSVData.```

**Este archivo se utiliza a lo largo de las 3 funciones del laboratorio.**

## ```./count```
> Este comando tiene por finalidad contar la cantidad de caracteres y lineas de un archivo tanto CSV cómo texto plano. Esto gracias a la construcción de la estructura, que permite facil acceso a estos datos, se convierte en iterar respecto de las líneas e ir contando tanto lineas cómo caracteres.

*El archivo de texto es obligatorio, mientras que se puede elegir si es que se quiere obtener solamente líneas o solamente caracteres, o en su defecto ambas. Las banderas de salida son obligatorias.* \
```./count -i archivo.txt -C -L```

## ```./srep```
> Este comando tiene por finalidad reemplazar las cadenas de caracteres y en un archivo cualquiera de texto, tomando un archivo de entrada y devolviendo un archivo de nombre especificado con cada instanciación del string objetivo reemplazada por la otra.

*Ambos archivo de texto son obligatorios, en caso de no poner nada en el string objetivo o el de reemplazo, no se efectuará la escritura.* \
```./srep -i archivo.txt -o salida.txt -s stringObjetivo -S stringReemplazo```

**Para el ejemplo otorgado en el enunciado, el ```\``` es un caracter especial en bash, por lo que requiere de otro ```\``` para especificar que se trata de un caracter y no un fin de linea, osease en el ejemplo planteado:**

```./srep -i input.txt -o output.txt -s / -S \\```


## ```./cut```
> Este comando tiene por finalidad: tomar un archivo de texto, procesarlo y entregar un archivo de texto, cuyo nombre es dictado por el usuario y, dado un delimitador y un **número de columnas separado por coma**, este archivo de salida presentaría las columnas elegidas empezando del 1 hasta la cantidad deseada, tomando en cuenta que no puede superar la cantidad de columnas que posea el original.

*Ambos archivo de texto son obligatorios, en caso de no poner nada en el string objetivo o el de reemplazo, no se efectuará la escritura.* \
```./cut -i archivo.txt -o salida.txt -d delimitador -c columnasPorComa```

**NOTA: Si funciona para el caso donde no se eligen columnas, aun que muestre el error de formato.**