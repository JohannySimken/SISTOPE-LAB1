# Lab1 Sistemas Operativos
# Integrantes
Martín Oviedo
Johanny Simken

## Compilación
```
make
```

# Elementos necesarios para la ejecución del código
Archivo de entrada: .bin para la imagen de entrada generado a partir de GenerarDatos.py

## Flags
-i: imagen de entrada (.bin)
-r: radio de los circulos a detectar
-t: umbral minimo de votos
-o: archivo de salida (.csv)
-d: activa debug (genera preprocesada.bin y ruido.bin)



# Ejecución
./lab1 -i entrada.bin -r "radio generadio" -t "umbral" -o "archivo_salida.csv"
./lab1 -i entrada.bin -r "radio generadio" -t "umbral" -o "archivo_salida.csv" -d (Para debugear)

Ejemplo de ejecución con los parametros por defecto de la generación de datos segun GenerarDatos.py:

```
./lab1 -i entrada.bin -r 40 -t 60 -o reporte.csv
```
o si se quiere activar el debug

```
./lab1 -i entrada.bin -r 40 -t 60 -o reporte.csv -d
```


# Resultados
Los archivos generados, dependiendo de si se activó el debug o no, son:

- reporte.csv: archivo de salida con los resultados
- preprocesada.bin: imagen preprocesada (solo si se activó el debug)
- ruido.bin: imagen con ruido (solo si se activó el debug)
