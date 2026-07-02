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
    FILE *fp = fopen("alumnos.xml", "r");
    if (!fp)
{
        printf("Error: No se pudo abrir alumnos.xml\n");
        return 1;
}

    char buffer[1024];
    fread(buffer, 1, 1024, fp);
    fclose(fp);

    char *ptr = buffer;
    Alumno temp;

    printf("Datos encontrados:\n");

    while ((ptr = strstr(ptr, "<Alumno>")) != NULL)
{

        sscanf(strstr(ptr, "<Nombre>") + 8, "%[^<]", temp.nombre);
        sscanf(strstr(ptr, "<Apellido>") + 10, "%[^<]", temp.apellido);
        sscanf(strstr(ptr, "<Promedio>") + 10, "%f", &temp.promedio);
        sscanf(strstr(ptr, "<Materia>") + 9, "%[^<]", temp.materia);

        printf("Nombre: %s %s | Promedio: %.2f | Materia: %s\n",temp.nombre, temp.apellido, temp.promedio, temp.materia);

        ptr += 8;
}

    return 0;
}
