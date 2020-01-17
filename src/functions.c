#include "../incl/functions.h"

// Variables globales.

// Funciones.
void hebraProductora(int cantImagenes, char *nomMascara, int umbNegro, int cantHebras, int tamBuffer, int bandResultados)
{
    int i, j, k, l;
    
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

    // Estructura que será entregada a las hebras.
    trozoImagen trozos[cantHebras];

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

        int alturaTrozo = (int)(alto / cantHebras);             // Trunca el excedente.
        int alturaLeer = (int)(alto / cantHebras) * cantHebras; // Trunca el excedente.

        // Se guardan las direcciones de los segmentos de imagen en la variable trozos.
        for (i = 0; i < cantHebras; i++)
        {
            int *fragmentoImagen = (int *)malloc(alturaTrozo * ancho * sizeof(int));
            trozos[i].matriz = fragmentoImagen;
            trozos[i].filasOcupadas = 0; // Para verificar cuando se llena el fragmento.
        }

        // Buffer que acumulará filas hasta que se llene.
        int *buffer = (int *)malloc(tamBuffer * ancho * sizeof(int));

        int contadorBuffer = 0; // Para verificar cuando se llena el buffer.
        int contadorTrozos = 0; // Para verificar cuando se terminó de llenar trozo de una hebra.

        int auxX = 0; // Auxiliar para moverse en eje X dentro de la matriz dentro de la hebra.
        int auxY = 0; // Auxiliar para moverse en eje Y dentro de la matriz dentro de la hebra.

        for (i = 0; i < alturaLeer; i++) // Proceso de llenado de buffers y hebras.
        {
            if (contadorBuffer < tamBuffer) // Llenar buffer.
            {
                fila = filas[i];
                for (j = 0; j < ancho; j++)
                {
                    pixel = fila[j];
                    *(buffer + contadorBuffer * ancho + j) = pixel;
                }
                contadorBuffer++;
                if (i == alturaLeer - 1) // Se terminaron las filas para leer de la imagen.
                {
                    for (k = 0; k < contadorBuffer; k++)
                    {
                        if (trozos[contadorTrozos].filasOcupadas < alturaTrozo)
                        {
                            for (l = 0; l < ancho; l++)
                            {
                                *(trozos[contadorTrozos].matriz + auxY * ancho + auxX) = *(buffer + k * ancho + l);
                                auxX++;
                            }
                            auxX = 0;
                            auxY++;
                            trozos[contadorTrozos].filasOcupadas++;
                        }
                        else // En el caso que se haya completado el fragmento correspondiente a una hebra.
                        {
                            contadorTrozos++; // Se cambia a la siguiente hebra.
                            auxX = 0;
                            auxY = 0;
                            k--;
                        }
                    }
                }
            }
            else // Vaciar buffer.
            {
                for (k = 0; k < tamBuffer; k++)
                {
                    if (trozos[contadorTrozos].filasOcupadas < alturaTrozo)
                    {
                        for (l = 0; l < ancho; l++)
                        {
                            *(trozos[contadorTrozos].matriz + auxY * ancho + auxX) = *(buffer + k * ancho + l);
                            auxX++;
                        }
                        auxX = 0;
                        auxY++;
                        trozos[contadorTrozos].filasOcupadas++;
                    }
                    else // En el caso que se haya completado el fragmento correspondiente a una hebra.
                    {
                        contadorTrozos++; // Se cambia a la siguiente hebra.
                        auxX = 0;
                        auxY = 0;
                        k--;
                    }
                }
                contadorBuffer = 0;
                i--;
            }
        }

        int a;
        for (a = 0; a < cantHebras; a++)
        {
            for (i = 0; i < alturaTrozo; i++)
            {
                for (j = 0; j < ancho; j++)
                {
                    printf("|%d", *(trozos[a].matriz + i * ancho + j));
                }
                printf("|\n");
            }
        }

        // Creación de cada thread.
        /*
        pthread_t tids[cantHebras];
        for (i = 0; i < cantHebras; i++)
        {
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&tids[i], &attr, hebraConsumidora, &cantHebras); // El tercer atributo es un puntero a función.
        }
        */

        /*
        for (i = 0; i < cantHebras; i++)
        {
            printf("altura hebra %d: %d\n", i + 1, trozos[i].filasOcupadas);
        }
        */

        fclose(fp);
        img++;
    }

    // Esperar hasta que cada thread termine.
    /*for (i = 0; i < cantHebras; i++)
    {
        pthread_join(tids[i], NULL);
    }*/

    return;
}

void *hebraConsumidora(void *argumento)
{
    pthread_exit(0);
}