#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void iniciarPipeline(int cantImagenes, char *nomMascara, int umbNegro, int cantHebras, int tamBuffer, int bandResultados);
void *pipeline(void *argumento);