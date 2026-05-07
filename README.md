# Lab1 Sistemas Operativos
# Integrantes
Martín Oviedo
Johanny Simken

## Compilación
```
make
```

# Ejecución
./lab1 -i entrada.bin -r 60 -t 70 -o reporte.csv
./lab1 -i entrada.bin -r 40 -t 50 -o reporte.csv -d (Para debug)

## Flags
-i: imagen de entrada (.bin)
-r: radio de los circulos a detectar
-t: umbral minimo de votos
-o: archivo de salida (.csv)
-d: activa debug (genera preprocesada.bin y ruido.bin)
