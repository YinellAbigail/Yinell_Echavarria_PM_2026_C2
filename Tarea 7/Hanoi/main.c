#include <stdio.h>

int contador = 1;

void resolverHanoi(int n, char origen, char destino, char auxiliar, FILE *archivo)
{
    if (n == 1)
    {
        fprintf(archivo, "Movimiento %d: Mover disco 1 de %c a %c\n", contador, origen, destino);
        contador++;
        return;
    }
resolverHanoi(n - 1, origen, auxiliar, destino, archivo);

fprintf(archivo, "Movimiento %d: Mover disco %d de %c a %c\n", contador, n, origen, destino);
contador++;

resolverHanoi(n - 1, auxiliar, destino, origen, archivo);
}

int main()
{
    int n = 3;

    FILE *archivo = fopen("solucion.txt", "w");

    if (archivo == NULL)
    {
        printf("Error: no se pudo crear el archivo.\n");
        return 1;
    }

    printf("Resolviendo torres de Hanoi para %d discos...\n", n);

    resolverHanoi(n, 'A', 'C', 'B', archivo);

    fclose(archivo);

    printf("Tarea completada. Los movimientos han sido guardados en 'solucion. txt'.\n");
    return 0;
}
