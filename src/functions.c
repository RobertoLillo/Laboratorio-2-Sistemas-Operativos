#include "../incl/functions.h"

// Variables globales.

// Funciones.
void hebraProductora(int cantImagenes, char *nomMascara, int umbNegro, int cantHebras, int tamBuffer, int bandResultados)
{
    if (bandResultados == 1)
    {
        printf("Nº Imagen\tResultado\n");
    }

    // ###################################
    // # Proceso de lectura de la imagen #
    // ###################################

    int i, j, k, l;

    FILE *fp;
    png_structp pngPtr;
    png_infop infoPtr;
    png_uint_32 ancho;
    png_uint_32 alto;
    png_bytepp filas;
    png_bytep fila;
    png_byte pixel;

    int imagen = 1;
    char nombre[15];
    while (imagen <= cantImagenes)
    {
        sprintf(nombre, "imagen_%i.png", imagen);

        fp = fopen(nombre, "rb");
        pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        infoPtr = png_create_info_struct(pngPtr);

        png_init_io(pngPtr, fp);
        png_read_png(pngPtr, infoPtr, 0, 0);
        png_get_IHDR(pngPtr, infoPtr, &ancho, &alto, NULL, NULL, NULL, NULL, NULL);
        filas = png_get_rows(pngPtr, infoPtr);

        // Estructura que será entregada a las hebras.
        trozoImagen trozos[cantHebras];

        int alturaTrozo = (int)(alto / cantHebras);             // Trunca el excedente.
        int alturaLeer = (int)(alto / cantHebras) * cantHebras; // Trunca el excedente.

        // Se guardan las direcciones de los segmentos de imagen en la variable trozos.
        for (i = 0; i < cantHebras; i++)
        {
            int *fragmentoImagen = (int *)malloc(alturaTrozo * ancho * sizeof(int));

            trozos[i].nombreMascara = nomMascara;
            trozos[i].alto = alturaTrozo;
            trozos[i].ancho = ancho;
            trozos[i].filasOcupadas = 0;        // Para verificar cuando se llena el fragmento.
            trozos[i].matriz = fragmentoImagen; // Puntero a la matriz que contiene el fragmento de la imagen.
            trozos[i].cantidadPixelesNegros = 0;
        }

        // Buffer que acumulará filas hasta que se llene.
        int *buffer = (int *)malloc(tamBuffer * ancho * sizeof(int));

        int contadorBuffer = 0; // Para verificar cuando se llena el buffer.
        int contadorTrozos = 0; // Para verificar cuando se terminó de llenar la capacidad de una hebra.

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

        free(buffer);
        fclose(fp);

        // #####################################
        // # Inicio del pipeline de cada hebra #
        // #####################################
        pthread_t tids[cantHebras];
        for (i = 0; i < cantHebras; i++) // Se inicia la hebra
        {
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&tids[i], &attr, hebraConsumidora, &trozos[i]); // El tercer atributo es un puntero a función.
        }
        // Esperar hasta que cada thread termine.
        for (i = 0; i < cantHebras; i++)
        {
            pthread_join(tids[i], NULL);
        }

        // Se obtienen las dimensiones para la nueva imagen.
        int nuevoAlto = trozos[0].alto * cantHebras;
        int nuevoAncho = trozos[0].ancho;
        int cantidadTotalPixelesNegros = 0;
        for (i = 0; i < cantHebras; i++)
        {
            cantidadTotalPixelesNegros += trozos[i].cantidadPixelesNegros;
        }
        printf("\n%d %d\n", nuevoAlto, nuevoAncho);

        // Se imprime el resultado de la clasificación si es solicitado.
        if (bandResultados == 1)
        {
            int porcentajeDePixeles = (nuevoAlto * nuevoAncho) * (umbNegro / 100);
            if (cantidadTotalPixelesNegros > porcentajeDePixeles)
            {
                printf("imagen_%d.png\tSi\n", imagen);
            }
            else
            {
                printf("imagen_%d.png\tNo\n", imagen);
            }
        }

        // Se guarda la imagen en disco.
        char nombreSalida[15];
        sprintf(nombreSalida, "salida%i.ppm", imagen);

        FILE *nuevaIMG = fopen(nombreSalida, "wb");
        fprintf(nuevaIMG, "P6\n%i %i 255\n", nuevoAncho, nuevoAlto);
        for (k = 0; k < cantHebras; k++)
        {
            for (i = 0; i < trozos[k].alto; i++)
            {
                for (j = 0; j < trozos[k].ancho; j++)
                {
                    fputc(*(trozos[k].matriz + i*trozos[k].ancho + j), nuevaIMG);
                    fputc(*(trozos[k].matriz + i*trozos[k].ancho + j), nuevaIMG);
                    fputc(*(trozos[k].matriz + i*trozos[k].ancho + j), nuevaIMG);
                }
            }
        }
        fclose(nuevaIMG);

        imagen++;
    }

    return;
}

void *hebraConsumidora(void *argumento)
{
    trozoImagen *trozo = (trozoImagen *)argumento; // Contiene: nombreMascara, umbralNegro, alto, ancho, filasOcupadas y matriz.

    // ##########################################
    // # Realizar todas las etapas del pipeline #
    // ##########################################
    int i, j;
    // ###############
    // # Convolución #
    // ###############
    int a = obtenerCantLineas(trozo->nombreMascara);
    char **matriz1 = extraerLineas(trozo->nombreMascara, a);

    int **mtxMascara = crearMatrizMascara(matriz1, a);

    int **mtx = (int **)malloc(sizeof(int *) * trozo->alto);
    for (i = 0; i < trozo->alto; i++)
    {
        mtx[i] = (int *)malloc(sizeof(int) * trozo->ancho);
        for (j = 0; j < trozo->ancho; j++)
        {
            mtx[i][j] = *(trozo->matriz + i * trozo->ancho + j);
        }
    }

    int **matrizConv = matrizConvolucion(mtx, mtxMascara, trozo->alto, trozo->ancho);

    // #################
    // # Normalización #
    // #################

    for (i = 0; i < trozo->alto - 2; i++)
    {
        for (j = 0; j < trozo->ancho; j++)
        {
            if (matrizConv[i][j] < 0)
            {
                matrizConv[i][j] = 0;
            }
        }
    }

    // ###########
    // # Pooling #
    // ###########
    int altoPool = 3;  // Dimensiones de la mascara de pooling.
    int anchoPool = 3; // Se pueden cambiar.

    int nuevoAlto = trozo->alto / altoPool;
    int nuevoAncho = trozo->ancho / anchoPool;

    int **mtxPooling = generarPooling(matrizConv, trozo->alto, trozo->ancho);

    // #################
    // # Clasificación #
    // #################
    int cantidadNegros = 0;
    for (i = 0; i < nuevoAlto - 2; i++)
    {
        for (j = 0; j < nuevoAncho; j++)
        {
            if (mtxPooling[i][j] > 230)
            {
                cantidadNegros++;
            }
        }
    }
    trozo->cantidadPixelesNegros = cantidadNegros;

    // ##############################
    // # Retorno a hebra productora #
    // ##############################
    int *matrizFinal = (int *)malloc((nuevoAlto - 2) * nuevoAncho * sizeof(int));
    for (i = 0; i < nuevoAlto - 2; i++)
    {
        for (j = 0; j < nuevoAncho - 2; j++)
        {
            *(matrizFinal + i * nuevoAncho + j) = mtxPooling[i][j];
        }
    }

    trozo->matriz = matrizFinal;
    trozo->alto = nuevoAlto;
    trozo->ancho = nuevoAncho - 2;
    trozo->filasOcupadas = nuevoAncho;

    pthread_exit(0);
}