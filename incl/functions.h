#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <png.h>

void hebraProductora(int cantImagenes, char *nomMascara, int umbNegro, int cantHebras, int tamBuffer, int bandResultados);
void *hebraConsumidora(void *argumento);