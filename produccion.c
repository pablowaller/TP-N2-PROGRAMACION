#include <stdio.h>
#include <stdlib.h>

struct Pieza
{
    int numeroPieza;
    int tiempoLavado;
    int tiempoHorneado;
    float temperaturaHorneado;
};

struct CicloProduccion
{
    int numeroCiclo;
    int cantidadPiezas;
    struct Pieza *piezas;
};

void mostrarCiclo(struct CicloProduccion ciclo)
{
    printf("Nro de Ciclo: %d\n", ciclo.numeroCiclo);
    printf("Pieza T.Lav. T.Hor. Temp.Hor.\n");
    for (int i = 0; i < ciclo.cantidadPiezas; i++)
    {
        struct Pieza pieza = ciclo.piezas[i];
        printf("%d %d %d %.2f\n", pieza.numeroPieza, pieza.tiempoLavado, pieza.tiempoHorneado, pieza.temperaturaHorneado);
    }
    printf("\n");
}

void generarArchivoSalida(struct CicloProduccion *ciclos, int cantidadCiclos)
{
    FILE *archivoSalida = fopen("salida.txt", "w");
    if (archivoSalida == NULL)
    {
        printf("Error al abrir el archivo de salida.\n");
        return;
    }

    for (int i = 0; i < cantidadCiclos; i++)
    {
        struct CicloProduccion ciclo = ciclos[i];
        fprintf(archivoSalida, "Nro de Ciclo: %d\n", ciclo.numeroCiclo);
        fprintf(archivoSalida, "Pieza T.Lav. T.Hor. Temp.Hor.\n");
        for (int j = 0; j < ciclo.cantidadPiezas; j++)
        {
            struct Pieza pieza = ciclo.piezas[j];
            fprintf(archivoSalida, "%d %d %d %.2f\n", pieza.numeroPieza, pieza.tiempoLavado, pieza.tiempoHorneado, pieza.temperaturaHorneado);
        }
        fprintf(archivoSalida, "\n");
    }

    fclose(archivoSalida);
}

int calcularTiempoTotal(struct CicloProduccion *ciclos, int cantidadCiclos)
{
    int tiempoTotalMinutos = 0;

    for (int i = 0; i < cantidadCiclos; i++)
    {
        struct CicloProduccion ciclo = ciclos[i];
        for (int j = 0; j < ciclo.cantidadPiezas; j++)
        {
            struct Pieza pieza = ciclo.piezas[j];
            tiempoTotalMinutos += pieza.tiempoLavado + pieza.tiempoHorneado;
        }
    }

    return tiempoTotalMinutos;
}


int main()
{

    FILE *archivoProduccion = fopen("produccion.bin", "rb");
    if (archivoProduccion == NULL)
    {
        printf("Error al abrir el archivo de producción.\n");
        return 1;
    }

    int cantidadTotalPiezas = 0;
    int cantidadTotalCiclos = 0;

    struct CicloProduccion ciclos[10];

    int numeroCiclo;
    int cantidadPiezas;

    while (fread(&numeroCiclo, sizeof(int), 1, archivoProduccion) == 1)
    {
        fread(&cantidadPiezas, sizeof(int), 1, archivoProduccion);

        cantidadTotalCiclos++;
        cantidadTotalPiezas += cantidadPiezas;

        struct Pieza *piezas = malloc(cantidadPiezas * sizeof(struct Pieza));

        for (int i = 0; i < cantidadPiezas; i++)
        {
            fread(&piezas[i].numeroPieza, sizeof(int), 1, archivoProduccion);
            fread(&piezas[i].tiempoLavado, sizeof(int), 1, archivoProduccion);
            fread(&piezas[i].tiempoHorneado, sizeof(int), 1, archivoProduccion);
            fread(&piezas[i].temperaturaHorneado, sizeof(float), 1, archivoProduccion);
        }

        ciclos[cantidadTotalCiclos - 1].numeroCiclo = numeroCiclo;
        ciclos[cantidadTotalCiclos - 1].cantidadPiezas = cantidadPiezas;
        ciclos[cantidadTotalCiclos - 1].piezas = piezas;

        mostrarCiclo(ciclos[cantidadTotalCiclos - 1]);
        printf("\n");
    }

    fclose(archivoProduccion);

    printf("Cantidad total de piezas procesadas: %d\n", cantidadTotalPiezas);
    int tiempoTotalMinutos = calcularTiempoTotal(ciclos, cantidadTotalCiclos);
    int horas = tiempoTotalMinutos / 60;
    int minutos = tiempoTotalMinutos % 60;
    printf("Tiempo total de proceso: %d horas %d minutos\n", horas, minutos);

    generarArchivoSalida(ciclos, cantidadTotalCiclos);

}