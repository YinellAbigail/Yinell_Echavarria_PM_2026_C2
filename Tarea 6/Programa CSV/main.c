#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[50];
}Alumno;

int main()
{
    FILE *fp = fopen("alumnos.csv", "r");
    if (!fp)
{
    printf("Error al abrir archivo\n");
    return 1;
}

    int n = 0;
    char buffer[200];
    while (fgets(buffer, 200, fp)) n++;
    n--;

    rewind(fp);
    fgets(buffer, 200, fp);
    Alumno *lista = (Alumno*) malloc(n * sizeof(Alumno));

    for (int i = 0; i < n; i++)
{
        fgets(buffer, 200, fp);
        strcpy(lista[i].nombre, strtok(buffer, ","));
        strcpy(lista[i].apellido, strtok(NULL, ","));
        lista[i].promedio = atof(strtok(NULL, ","));
        strcpy(lista[i].materia, strtok(NULL, "\n"));
        printf("Alumno: %s %s | Promedio: %.2f | Materia: %s\n", lista[i].nombre, lista[i].apellido, lista[i].promedio, lista[i].materia);
}

    free(lista);
    fclose(fp);
    return 0;
}
