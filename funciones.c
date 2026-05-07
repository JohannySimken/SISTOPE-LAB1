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

// Entradas: Image original con ruido
// Salidas: nueva Image erosionada con memoria propia en heap
// Descripcion: Por cada pixel revisa sus 5 vecinos segun el SE en cruza, cuando haya alguno fuera del borde o que sea 0, el pixel resultado es 0. Además elimina pixels aislados de ruido
Image erosion(Image img){

    Image result;
    result.width = img.width;
    result.height = img.height;

    // reserva memoria para la imagen resultado
    result.pixels = (uint8_t *)malloc(result.width * result.height);
    if (!result.pixels) {
        fprintf(stderr, "Error allocating memory for erosion\n");
        exit(EXIT_FAILURE);
    }

    // los movimientos que realiza SE en cruz: centro, arriba, abajo, izquierda, derecha
    int se_row[] = { 0, -1, 1,  0, 0};
    int se_col[] = { 0,  0, 0, -1, 1};

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {

            uint8_t val = 1; // asume que el pixel sobrevive, si algun vecino es 0 o esta fuera de borde, el pixel muere.

            for (int k = 0; k < 5; k++) {
                int neigh_i = i + se_row[k]; // fila del vecino
                int neigh_j = j + se_col[k]; // columna del vecino

                // si el vecino esta fuera de borde o es 0, el pixel central muere
                if (neigh_i < 0 || neigh_i >= img.height || neigh_j < 0 || neigh_j >= img.width || img.pixels[neigh_i * img.width + neigh_j] == 0) {
                    val = 0;
                    break;
                }
            }

            // guarda el resultado en el arreglo
            result.pixels[i * result.width + j] = val;
        }
    }
    return result;
}

// Entradas: Image erosionada
// Salidas: nueva Image dilatada con memoria propia en heap
// Descripcion: Por cada pixel revisa sus 5 vecinos segun el SE en cruz. Si alguno de sus vecinos es 1, restaura el grosor de losbordes adelgazados por la erosion.
Image dilation(Image img) {
    Image result;
    result.width = img.width;
    result.height = img.height;

    result.pixels = (uint8_t *)malloc(result.width * result.height);
    if (!result.pixels) {
        fprintf(stderr, "Error allocating memory for dilation\n");
        exit(EXIT_FAILURE);
    }

    // los movimientos que realiza SE en cruz: centro, arriba, abajo, izquierda, derecha
    int se_row[] = { 0, -1, 1,  0, 0};
    int se_col[] = { 0,  0, 0, -1, 1};

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            uint8_t val = 0; // asume que el pixel muere, si algun vecino es 1, el pixel se enciende.

            for (int k = 0; k < 5; k++) {
                int neigh_i = i + se_row[k]; // fila del vecino
                int neigh_j = j + se_col[k]; // columna del vecino

                // si el vecino esta dentro del borde y es 1, el pixel se enciende
                if (neigh_i >= 0 && neigh_i < img.height && neigh_j >= 0 && neigh_j < img.width && img.pixels[neigh_i * img.width + neigh_j] == 1) {
                    val = 1;
                    break;
                }
            }
            // guarda en arreglo plano: fila i, columna j
            result.pixels[i * result.width + j] = val;
        }
    }

    return result;
}

//subtract_images resta pixel a pixel la imagen dilatada de la erosionada, dejando solo los bordes del objeto: original - preprocesada = ruido.
// Entradas: Image original con ruido, Image preprocesada (resultado de erosion + dilatacion)
// Salidas: nueva Image con los pixels que fueron eliminados durante el preprocesamiento
// Descripcion: Resta pixel a pixel original - preprocesada. El resultado son los pixels que existian en la original pero que no sobrevivieron la apertura (el ruido).
Image subtract_images(Image original, Image preprocessed) {
    Image result;
    result.width = original.width;
    result.height = original.height;

    result.pixels = (uint8_t *)malloc(result.width * result.height);
    if (!result.pixels) {
        fprintf(stderr, "Error allocating memory for subtract\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < original.height; i++) {
        for (int j = 0; j < original.width; j++) {
            int idx = i * original.width + j; // indice en el arreglo plano

            // si el pixel existia en original pero no en preprocesada, es ruido
            int diff = original.pixels[idx] - preprocessed.pixels[idx];
            // se usa operarador ternario para darle el valor de 1 la resta es positiva (era ruido) o 0 si es negativa (no era ruido). Clampea a 0 si negativo con el fin de evitar valores negativos.
            result.pixels[idx] = (diff > 0) ? 1 : 0;
        }
    }

    return result;
}

// Entradas: Image a liberar
// Salidas: ninguna
// Descripcion: Libera la memoria del arreglo de pixels reservada con malloc
void free_image(Image img) {
    free(img.pixels);
}


// Entradas: Image a procesar, radio de la circunferencia, umbral de deteccion, ruta del archivo CSV para guardar los resultados
// Salidas: ninguna
// Descripcion: Implementacion del algoritmo de Hough para detectar circunferencias en una imagen. Al finalizar, guarda los resultados en un archivo CSV.
void hough(Image img, int radius, int threshold, const char *csv_path) {
    int W = img.width;
    int H = img.height;

    // Inicializacion del acumulador
    int *accumulator = (int *)calloc(W * H, sizeof(int));
    if (!accumulator) {
        fprintf(stderr, "Error al asignar memoria para el acumulador\n");
        exit(EXIT_FAILURE);
    }

    int grades = 360; // Numero de grados en la circunferencia


    // fila
    for (int i = 0; i < H; i++) {
        // columna
        for (int j = 0; j < W; j++) {
            if(img.pixels[i * W + j] == 0) continue;
            for (int k = 0; k < grades; k++) {

                double theta = 2.0 * M_PI * k / grades; // Calculo del angulo theta en radianes para trabajar con cos y sin segun transformada

                // Calculo de las coordenadas x e y en la imagen para el punto en la circunferencia
                int x = (int)round(j + radius * cos(theta));
                int y = (int)round(i + radius * sin(theta));

                // Verificacion de que las coordenadas x e y estan dentro de los limites de la imagen
                if (x < 0 || x >= W || y < 0 || y >= H) {
                    continue;
                }

                // Incremento del acumulador en la posicion correspondiente
                accumulator[y * W + x]++;
            }
        }
    }

    // Guardado de los resultados en un archivo CSV
    FILE *csv = fopen(csv_path, "w");
    if (!csv) {
        fprintf(stderr, "Error al crear archivo CSV: %s\n", csv_path);
        free(accumulator);
        exit(EXIT_FAILURE);
    }

    fprintf(csv, "X,Y\n");
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (accumulator[i * W + j] >= threshold) {
                fprintf(csv, "%d,%d\n", i, j);
            }
        }
    }

    // Cierre del archivo CSV y liberacion de memoria
    fclose(csv);
    free(accumulator);

}
