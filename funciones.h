#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdint.h>

// Estructura que representa la imagen en memoria
typedef struct {
    int width;
    int height;
    uint8_t *pixels; // arreglo plano de width*height bytes
} Image;

// Entradas: ruta del archivo .bin
// Salidas: Image con los datos cargados (pixels en heap)
// Descripcion: Lee el encabezado (width, height) y los pixels del archivo binario
Image read_image(const char *path);

// Entradas: imagen a guardar, ruta de destino
// Salidas: ninguna
// Descripcion: Escribe la imagen en formato .bin (cabecera + pixels)
void write_image(Image img, const char *path);

// Entradas: imagen original
// Salidas: nueva imagen erosionada
// Descripcion: Aplica erosion con SE 3x3 en cruz
Image erosion(Image img);

// Entradas: imagen erosionada
// Salidas: nueva imagen dilatada
// Descripcion: Aplica dilatacion con SE 3x3 en cruz
Image dilation(Image img);

// Entradas: imagen preprocesada, radio r, umbral t, ruta del csv
// Salidas: ninguna (escribe el CSV directamente)
// Descripcion: Aplica la Transformada de Hough y guarda centros detectados
void hough(Image img, int radius, int threshold, const char *csv_path);

// Entradas: dos imagenes del mismo tamaño
// Salidas: nueva imagen resultado de original - preprocesada
// Descripcion: Resta pixel a pixel, clampea a 0 si negativo
Image subtract_images(Image original, Image preprocessed);

// Entradas: imagen a liberar
// Salidas: ninguna
// Descripcion: Libera la memoria del arreglo de pixels
void free_image(Image img);

#endif