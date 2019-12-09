#include "../incl/main.h"
#include "../incl/functions.h"

int main(int argc, char **argv)
{
	if (argc < 11)
	{
		usoArgumentos(1, 0);
	}
	else if (argc > 12)
	{
		usoArgumentos(1, 0);
	}
	else
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
		char *nombreMascara = NULL;

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
				nombreMascara = optarg;
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
			usoArgumentos(2, contError);
		}
	}
	return 0;
}
