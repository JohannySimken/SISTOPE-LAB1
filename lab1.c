#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "funciones.h"

int main(int argc, char *argv[]) {
    char *input_path = NULL;
    char *output_path = NULL;
    int radius = 0;
    int threshold = 0;
    int debug = 0; // flag -d, por defecto desactivado

    // getopt parsea los flags de la linea de comandos
    int opt;
    while ((opt = getopt(argc, argv, "i:r:t:o:d")) != -1) {
        switch (opt) {
            case 'i': input_path = optarg;        break;
            case 'r': radius = atoi(optarg);      break;
            case 't': threshold = atoi(optarg);   break;
            case 'o': output_path = optarg;       break;
            case 'd': debug = 1;                  break;
            default:
                fprintf(stderr, "Uso: %s -i entrada.bin -r radio -t umbral -o salida.csv -d\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // verificar que los flags obligatorios esten presentes
    if (!input_path || !output_path || radius == 0 || threshold == 0) {
        fprintf(stderr, "Error: Faltan argumentos obligatorios\n");
        fprintf(stderr, "Uso: %s -i entrada.bin -r radio -t umbral -o salida.csv -d\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // en primer lugar se carga imagen original
    Image original = read_image(input_path);

    // se prepara la imagen con erosion y dilatacion (apertura) para eliminar ruido
    Image eroded = erosion(original);
    Image preprocessed = dilation(eroded);
    free_image(eroded);

    // se calcula el ruido eliminado: original - preprocesada
    Image noise = subtract_images(original, preprocessed);

    // si flag -d esta activo, exportar imagenes intermedias
    if (debug) {
        write_image(preprocessed, "preprocesada.bin");
        write_image(noise, "ruido.bin");
    }

    // detectan circulos con hough y se escribe el CSV
    hough(preprocessed, radius, threshold, output_path);
    printf("Deteccion completada. Resultados guardados en %s\n", output_path);

    // se libera la memoria
    free_image(original);
    free_image(preprocessed);
    free_image(noise);

    return 0;
}
