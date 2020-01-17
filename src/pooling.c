#include "../incl/pooling.h"

int **obtenerMtx(int **m, int filas, int columnas, int x, int y)
{
    int i, j;
    int **posicion = (int **)malloc(sizeof(int *) * 3);
    for (i = 0; i < 3; i++)
    {
        posicion[i] = (int *)malloc(sizeof(int) * 3);
    }
    posicion[0][0] = m[x][y];
    posicion[0][1] = m[x][y + 1];
    posicion[0][2] = m[x][y + 2];

    posicion[1][0] = m[x + 1][y];
    posicion[1][1] = m[x + 1][y + 1];
    posicion[1][2] = m[x + 1][y + 2];

    posicion[2][0] = m[x + 2][y];
    posicion[2][1] = m[x + 2][y + 1];
    posicion[2][2] = m[x + 2][y + 2];

    return posicion;
}

int *matrizPooling(int **matriz)
{
    int *mtx = (int *)malloc(sizeof(int *) * 9);
    mtx[0] = matriz[0][0];
    mtx[1] = matriz[0][1];
    mtx[2] = matriz[0][2];

    mtx[3] = matriz[1][0];
    mtx[4] = matriz[1][1];
    mtx[5] = matriz[1][2];

    mtx[6] = matriz[2][0];
    mtx[7] = matriz[2][1];
    mtx[8] = matriz[2][2];
    return mtx;
}

int buscarMayor(int *arreglo)
{
    int mayor = 0;
    int i, j;
    for (i = 0; i < 9; i++)
    {
        if (arreglo[i] > mayor)
        {
            mayor = arreglo[i];
        }
    }
    return mayor;
}

int pooling(int **matriz1, int filas, int columnas, int x, int y)
{

    int **miniImagen = obtenerMtx(matriz1, filas, columnas, x, y);

    int *arregloPooling = matrizPooling(miniImagen);

    int datoPooling = buscarMayor(arregloPooling);

    return datoPooling;
}

int **generarPooling(int **matriz1, int filas, int columnas)
{
    int filaAux = (filas - 2) / 3;
    int columnaAux = (columnas - 2) / 3;
    int i, j;
    int **mtxPooling = (int **)malloc(sizeof(int *) * filaAux);
    int pool = 0;
    for (i = 0; i < filaAux; i++)
    {
        mtxPooling[i] = (int *)malloc(sizeof(int) * columnaAux);
        for (j = 0; j < columnaAux; j++)
        {
            pool = pooling(matriz1, filas, columnas, i * 3, j * 3);
            mtxPooling[i][j] = pool;
        }
    }
    return mtxPooling;
}