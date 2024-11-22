# Laboratorio 1 - Sistemas Operativos Universidad de Santiago de Chile
***
> En este laboratorio se verá cómo se utiliza la función ```getopt()```.
> Además, es de importancia notar que no se utilizará la librería ```<string.h>```

# Ejecución del laboratorio
Cómo fue estipulado en clases, para poder probar los comandos es necesario el uso de `makefile`, por lo que el comando para la generación de los archivos ejecutables es:
```
make all
```

Para limpiar los archivos residuales post ejecución de los archivos, se utiliza:
```
make clean
```

# Especificación de Archivos


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