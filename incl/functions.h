#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <png.h>
#include "../incl/convolucion.h"
#include "../incl/pooling.h"

typedef struct
{
    char *nombreMascara;
    int alto;
    int ancho;
    int filasOcupadas;
    int *matriz;
    int cantidadPixelesNegros;
} trozoImagen;

void hebraProductora(int cantImagenes, char *nomMascara, int umbNegro, int cantHebras, int tamBuffer, int bandResultados);
void *hebraConsumidora(void *argumento);