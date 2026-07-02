#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[50];
} Alumno;

int main()
{
    FILE *fp = fopen("alumnos.json", "r");
    if (!fp)
{
        printf("Error: No se pudo abrir alumnos.json\n");
        return 1;
}

    char buffer[4096];
    size_t n = fread(buffer, 1, 4095, fp);
    buffer[n] = '\0';
    fclose(fp);

    printf("Leyendo datos del JSON:\n");

    char *ptr = strstr(buffer, "{");
    while (ptr != NULL)
{
        Alumno temp;

        if (sscanf(ptr, " { \"Nombre\": \"%[^\"]\", \"Apellido\": \"%[^\"]\", \"Promedio\": %f, \"Materia\": \"%[^\"]\" }",temp.nombre, temp.apellido, &temp.promedio, temp.materia) == 4)
{

            printf("Alumno: %s %s | Promedio: %.2f | Materia: %s\n", temp.nombre, temp.apellido, temp.promedio, temp.materia);
}


        ptr = strstr(ptr + 1, "{");
}

    return 0;
}
