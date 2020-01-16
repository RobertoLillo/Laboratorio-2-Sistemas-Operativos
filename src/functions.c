#include "../incl/functions.h"

// Variables globales.

// Funciones.
void hebraProductora(int cantImagenes, char *nomMascara, int umbNegro, int cantHebras, int tamBuffer, int bandResultados)
{
    int i, j, k, l;

    // Creación de cada thread.
    pthread_t tids[cantHebras];
    for (i = 0; i < cantHebras; i++)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tids[i], &attr, hebraConsumidora, &cantHebras); // El tercer atributo es un puntero a función.
    }

    // ###################################
    // # Proceso de lectura de la imagen #
    // ###################################
    FILE *fp;
    png_structp pngPtr;
    png_infop infoPtr;
    png_uint_32 ancho;
    png_uint_32 alto;
    png_bytepp filas;
    png_bytep fila;
    png_byte pixel;

    int img = 1;
    char nombre[15];
    while (img <= cantImagenes)
    {
        sprintf(nombre, "imagen_%i.png", img);

        fp = fopen(nombre, "rb");
        pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        infoPtr = png_create_info_struct(pngPtr);

        png_init_io(pngPtr, fp);
        png_read_png(pngPtr, infoPtr, 0, 0);
        png_get_IHDR(pngPtr, infoPtr, &ancho, &alto, NULL, NULL, NULL, NULL, NULL);
        filas = png_get_rows(pngPtr, infoPtr);

        int buffer[tamBuffer][ancho];

        int contBuffer = 0;
        for (i = 0; i < alto; i++)
        {
            if (contBuffer < tamBuffer)
            {
                fila = filas[i];
                for (j = 0; j < ancho; j++)
                {
                    pixel = fila[j];
                    buffer[contBuffer][j] = pixel;
                }
                contBuffer++;
            }
            else
            {
                for (k = 0; k < tamBuffer; k++)
                {
                    for (l = 0; l < ancho; l++)
                    {
                        printf("%d", buffer[k][l]);
                    }
                    printf("\n");
                }
                printf("Vaciar buffer\n");
                contBuffer = 0;
            }
        }
        fclose(fp);

        img++;
    }

    // Esperar hasta que cada thread termine.
    for (i = 0; i < cantHebras; i++)
    {
        pthread_join(tids[i], NULL);
    }

    printf("termine\n");

    return;
}

void *hebraConsumidora(void *argumento)
{
    pthread_exit(0);
}