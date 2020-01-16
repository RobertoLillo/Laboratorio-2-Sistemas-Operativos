#include "../incl/main.h"
#include "../incl/functions.h"

int main(int argc, char **argv)
{
	if (argc < 11)
	{
		usoArgumentos(1, 0); // La cantidad de argumentos ingresados no es la correcta.
	}
	else if (argc > 12)
	{
		usoArgumentos(1, 0); // La cantidad de argumentos ingresados no es la correcta.
	}
	else // La cantidad de argumentos esta dentro de lo esperado.
	{
		int opcion;
		int contError = 0;

		int cflag = 0;
		int mflag = 0;
		int nflag = 0;
		int hflag = 0;
		int tflag = 0;
		int bflag = 0;

		int cantImagenes, umbNegro, cantHebras, tamBuffer;
		char *nomMascara = NULL;

		while ((opcion = getopt(argc, argv, "c:m:n:h:t:b")) != -1)
		{
			switch (opcion)
			{
			case 'c':
				cflag = 1;
				cantImagenes = atoi(optarg);
				if (cantImagenes == 0)
				{
					contError++;
				}
				break;

			case 'm':;
				mflag = 1;
				nomMascara = optarg;
				break;

			case 'n':
				nflag = 1;
				umbNegro = atoi(optarg);
				if (umbNegro == 0)
				{
					contError++;
				}
				break;

			case 'h':
				hflag = 1;
				cantHebras = atoi(optarg);
				if (cantHebras == 0)
				{
					contError++;
				}
				break;

			case 't':
				tflag = 1;
				tamBuffer = atoi(optarg);
				if (tamBuffer == 0)
				{
					contError++;
				}
				break;

			case 'b':
				bflag = 1;
				break;

			default:
				contError++;
			}
		}

		if (contError != 0)
		{
			usoArgumentos(2, contError); // Cierta cantidad de argumentos fueron ingresados incorrectamente.
		}
		else // Los argumentos fueron ingresados correctamente.
		{
			hebraProductora(cantImagenes, nomMascara, umbNegro, cantHebras, tamBuffer, bflag);
		}
	}
	return 0;
}

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