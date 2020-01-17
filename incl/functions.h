#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <png.h>

typedef struct
{
    int filasOcupadas;
    int *matriz;
} trozoImagen;

void hebraProductora(int cantImagenes, char *nomMascara, int umbNegro, int cantHebras, int tamBuffer, int bandResultados);
void *hebraConsumidora(void *argumento);