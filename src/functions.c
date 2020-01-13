#include "../incl/functions.h"

// Variables globales.

// Funciones.
void iniciarPipeline(int cantImagenes, char *nomMascara, int umbNegro, int cantHebras, int tamBuffer, int bandResultados)
{
	int i;

	// Creación de cada thread.
	pthread_t tids[cantHebras];
	for (i = 0; i < cantHebras; i++)
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids[i], &attr, pipeline, &cantHebras); // El tercer atributo es un puntero a función.
	}

	// Espacio para hacer otras cosas.

	// Esperar hasta que cada thread termine.
	for (i = 0; i < cantHebras; i++)
	{
		pthread_join(tids[i], NULL);
	}
	printf("termine\n");

	return;
}

void *pipeline(void *argumento)
{
	printf("funciona\n");

	pthread_exit(0);
}