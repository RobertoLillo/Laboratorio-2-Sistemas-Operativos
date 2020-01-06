#include "../incl/functions.h"

// Variables globales


// Funciones
void iniciarPipeline(int cantImagenes, char* nomMascara, int umbNegro, int cantHebras, int tamBuffer, int bandResultados)
{
	int i;
	pthread_t tids [cantHebras];
	for (i = 0; i < cantHebras; i++)
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids[i], &attr, prueba, &cantImagenes);
	}
	for (i = 0; i < cantHebras; i++)
	{
		pthread_join(tids[i], NULL); 
	}
	printf("termine\n");

	return;
} 

void *prueba(void *arg)
{
	int *valor = (int *) arg;
	int valor2 = *valor;

	printf("funciona\n");

	pthread_exit(0);
}