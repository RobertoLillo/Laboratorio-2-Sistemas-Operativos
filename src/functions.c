#include "../incl/functions.h"

void usoArgumentos(int numError, int cantError)
{
	switch (numError)
	{
	case 1:
		printf("La cantidad de argumentos ingresados no es la correcta.\n");
		break;

	case 2:
		if (cantError == 1)
		{
			printf("%d argumento fue ingresado de forma incorrecta.\n", cantError);
		}
		else
		{
			printf("%d argumentos fueron ingresados de forma incorrecta.\n", cantError);
		}
		break;

	default:
		break;
	}

	printf("\nEl formato correcto es el siguiente:\n");
	printf("\t-c Cantidad de imagenes.\n");
	printf("\t-m nombre del archivo de mascara.\n");
	printf("\t-n umbral de negrura.\n");
	printf("\t-h cantidad de hebras.\n");
	printf("\t-t tamano del buffer.\n");
	printf("\t-b bandera para mostrar resultados por pantalla.\n");
	printf("\n El siguiente corresponde a un ejemplo:\n");
	printf("\t./lab.out -c 3 -m mascara.txt -n 80 -h 10 -t 10 -b\n");

	return;
}