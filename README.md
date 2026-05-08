# Lab1 Sistemas Operativos
# Integrantes
- Martín Oviedo
- Johanny Simken

## Compilación
Requisitos previos: `gcc`, `make`  y la libreria matemática estandar (`-lm`). El programa esta diseñado para ejecutarse en sistemas operativos basados en Linux. Aunque puede ser ejecutado por sistemas operativos como Windows.

```
make
```

# Descripción
El programa `lab1` es un detector de circulos en imágenes binarias que utiliza un algoritmo de votación para identificar los circulos presentes en la imagen.
Las etapas son:
1. **Carga de la imagen**: carga la imagen binaria desde un archivo .bin.
2. **Preprocesamiento**: meidante el uso de la morfología matemática (erosión y dilatación) para reducir el ruido en la imagen.
3. **Análisis de ruido**: resta entre la imagen original y la preprocesada
4. **Transformada de Hough**: Votacion en la transformada de Hough para detectar los circulos en la imagen.
5. **Resultado**: Guarda los resultados en un archivo .csv con las coordenadas y radio de los circulos detectados.


# Archivos necesarios para la ejecución del código
El archivo `.bin.` debe seguir el siguiente formato:
- 4 bytes: ancho de la imagen
- 4 bytes: alto de la imagen
- `ancho x alto` bytes: pixeles(0 = negro, 1 = blanco)

Puede generarse con el script `GenerarDatos.py`.

## Flags
| Flag | Descripción |
|------|-------------|
| -i | imagen de entrada (.bin) |
| -r | radio de los circulos a detectar |
| -t | umbral minimo de votos |
| -o | archivo de salida (.csv) |
| -d | activa debug (genera preprocesada.bin y ruido.bin) |



# Ejecución

### Formato general
```
./lab1 -i entrada.bin -r "radio generado" -t "umbral" -o "archivo_salida.csv"
```
```
./lab1 -i entrada.bin -r "radio generado" -t "umbral" -o "archivo_salida.csv" -d (Para debugear)
```

Ejemplo de ejecución con los parametros por defecto de la generación de datos segun GenerarDatos.py:

```
./lab1 -i entrada.bin -r 40 -t 60 -o reporte.csv
```
o si se quiere activar el debug

```
./lab1 -i entrada.bin -r 40 -t 60 -o reporte.csv -d
```

### Ejemplos con distintos radios y umbrales

Los parámetros del script `GenerarDatos.py` generan por defecto círculos de radio 40. El umbral recomendado para ese caso es 60, pero puede ajustarse según la cantidad de ruido y la precisión deseada.
 
**Radio 40, umbral 60** (configuración base de `GenerarDatos.py`):
```bash
./lab1 -i entrada.bin -r 40 -t 60 -o reporte.csv
```
 
**Radio 60, umbral 70** (círculos más grandes, umbral más estricto → menos falsos positivos):
```bash
./lab1 -i entrada.bin -r 60 -t 70 -o reporte.csv
```
 
**Radio 80, umbral 50** (círculos grandes con umbral más permisivo → detecta más centros):
```bash
./lab1 -i entrada.bin -r 80 -t 50 -o reporte.csv
```
 
**Radio 20, umbral 90** (círculos pequeños con umbral muy alto → solo centros muy bien definidos):
```bash
./lab1 -i entrada.bin -r 20 -t 90 -o reporte.csv
```

### Efectos de los parámetros

- **Radio**: Aumentar el radio hace que el detector busque centros más grandes, lo que puede aumentar la cantidad de falsos positivos.
- **Umbral**: Aumentar el umbral hace que el detector sea más estricto, lo que puede reducir la cantidad de falsos positivos pero también aumentar la cantidad de falsos negativos.

## Archivos generados

| Archivo | Condicion | Descripción |
|---------|-----------|-------------|
| reporte.csv | Siempre | Archivo de salida con los resultados |
| preprocesada.bin | Solo con -d | Imagen tras erosión y dilatación  |
| ruido.bin | Solo con -d | Imagen con ruido, resultande de `original - procesada` |
