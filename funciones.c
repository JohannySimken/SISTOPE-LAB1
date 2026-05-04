#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Entradas: ruta del archivo .bin a leer
// Salidas: struct Image con width, height y pixels cargados en memoria heap
// Descripcion: Abre el archivo binario, lee 4 bytes para width y 4 para height, sereserva memoria dinamica para width*height pixels y los carga
Image read_image(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", path);
        exit(EXIT_FAILURE);
    }

    Image img;
    fread(&img.width, sizeof(int), 1, file);   // lee 4 bytes correpondientes al ancho de la imagen
    fread(&img.height, sizeof(int), 1, file);  // lee 4 bytes correspondientes a la altura de la imagen

    // se reserva width*height bytes en el heap para almacenar los pixels
    img.pixels = (uint8_t *)malloc(img.width * img.height);
    if (!img.pixels) {
        fprintf(stderr, "Error allocating memory for pixels\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(img.pixels, sizeof(uint8_t), img.width * img.height, file); // carga todos los pixels 
    fclose(file);
    return img;
}

// Entradas: struct Image a guardar, ruta del archivo de destino
// Salidas: ninguna 
// Descripcion: Abre un archivo binario para escritura, escribe el width y height (4 bytes cada uno), luego escribe el arreglo de pixels completo (width*height bytes)
void write_image(Image img, const char *path) {
    FILE *file = fopen(path, "wb");
    if (!file) {
        fprintf(stderr, "Error creating file: %s\n", path);
        exit(EXIT_FAILURE);
    }

    fwrite(&img.width, sizeof(int), 1, file);   // escribe 4 bytes correspondientes al ancho de la imagen 
    fwrite(&img.height, sizeof(int), 1, file);  // escribe 4 bytes correspondientes a la altura de la imagen

    // escribe todos los pixels 
    fwrite(img.pixels, sizeof(uint8_t), img.width * img.height, file);

    fclose(file);
}


Image erosion(Image img){}