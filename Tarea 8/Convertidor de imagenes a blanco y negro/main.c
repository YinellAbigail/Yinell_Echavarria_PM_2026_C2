#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int validar_extension(const char *nombre)
{
    size_t len = strlen(nombre);
    if (len < 5)
        return 0;

    const char *ext = nombre + len - 4;
    return (strcasecmp(ext, ".png") == 0 ||
            strcasecmp(ext, ".jpg") == 0 ||
            strcasecmp(ext, ".bmp") == 0);
}

void procesar_imagen(const char *nombre_entrada)
{
    int ancho, alto, canales;

    unsigned char *pixeles = stbi_load(nombre_entrada, &ancho, &alto, &canales, 0);

    if (pixeles == NULL)
    {
        printf("[VALIDACION FALLIDA]: El archivo '%s' no contiene una estructura de imagen valida o esta corrupto.\n\n", nombre_entrada);
        return;
    }

    printf("-> Estructura valida detectada. Dimensiones: %dx%d | Canales de color: %d\n", ancho, alto, canales);

    if (canales < 3)
    {
        printf("[AVISO]: La imagen ya se encuentra en escala de grises o no tiene canales RGB suficientes.\n\n");
        stbi_image_free(pixeles);
        return;
    }

    size_t total_bytes = (size_t)ancho * alto * canales;
    for (size_t i = 0; i < total_bytes; i += canales)
    {
        unsigned char r = pixeles[i];
        unsigned char g = pixeles[i + 1];
        unsigned char b = pixeles[i + 2];

        unsigned char gris = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);

        pixeles[i] = gris;
        pixeles[i + 1] = gris;
        pixeles[i + 2] = gris;
    }

    char nombre_salida[260];
    int punto_idx = (int)strlen(nombre_entrada) - 1;
    while (punto_idx > 0 && nombre_entrada[punto_idx] != '.')
    {
        punto_idx--;
    }

    strncpy(nombre_salida, nombre_entrada, punto_idx);
    nombre_salida[punto_idx] = '\0';
    strcat(nombre_salida, "_bn");
    strcat(nombre_salida,  nombre_entrada + punto_idx);

    int guardado_exitoso = 0;
    const char *ext = nombre_entrada + punto_idx;

   if (strcasecmp(ext, ".png") == 0)
    {
        guardado_exitoso = stbi_write_png(nombre_salida, ancho, alto, canales, pixeles, ancho * canales);
    }
   else
    if (strcasecmp(ext, ".jpg") == 0)
    {
        guardado_exitoso = stbi_write_jpg(nombre_salida, ancho, alto, canales, pixeles, 90);
    }
   else
    if (strcasecmp(ext, ".bmp") == 0)
    {
        guardado_exitoso = stbi_write_bmp(nombre_salida, ancho, alto, canales, pixeles);
    }

   if (guardado_exitoso)
   {
       printf("[EXITO]: Imagen convertida y guardada en: %s\n\n", nombre_salida);
   }
   else
   {
       printf("[ERROR]: No se pudo escribir el archivo en el disco.\n\n");
   }

   stbi_image_free(pixeles);
}

int main()
{
    char lista_archivos[100][260];

    while (1)
    {
        int total_archivos = 0;
        DIR *dir = opendir(".");

        if (!dir)
        {
            printf("Error: No se pudo abrir el directorio actual.\n");
            return 1;
        }

        printf("==================================================\n");
        printf("       SISTEMA DE PROCESAMIENTO DIGITAL (C)       \n");
        printf("==================================================\n");

        struct dirent *entrada_dir;
        while ((entrada_dir = readdir(dir)) != NULL)
        {
            if (validar_extension(entrada_dir->d_name))
            {
                strcpy(lista_archivos[total_archivos], entrada_dir->d_name);
                total_archivos++;
                if (total_archivos >= 100)
                    break;
            }
        }
        closedir(dir);

        if (total_archivos == 0)
        {
            printf("No se encontraron archivos .png, .jpg, .bmp en esta carpeta.\n");
            printf("Coloque imagenes de prueba junto al programa.\n\n");
            break;
        }

        for (int i = 0; i<total_archivos; i++)
        {
            printf("%d. %s\n", i + 1, lista_archivos[i]);
        }
        printf("0. Salir del programa\n");

        int opcion;
        printf("\nSeleccione el numero de la imagen a procesar: ");
        if (scanf("%d", &opcion) != 1)
        {
            printf("Entrada no valida.\n\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        if (opcion == 0)
        {
            printf("Finalizando ejecucion...\n");
            break;
        }
        else
            if (opcion > 0 && opcion <= total_archivos)
        {
            printf("\nAnalizando archivo: '%s'...\n", lista_archivos[opcion - 1]);
            procesar_imagen(lista_archivos[opcion - 1]);
        }
        else
        {
            printf("Opcion fuera de rango. Reintente.\n\n");
        }

    }
    return 0;
}
