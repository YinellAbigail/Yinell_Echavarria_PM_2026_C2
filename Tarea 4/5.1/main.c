#include <stdio.h>
#include <stdlib.h>
#define CANT 5

void main(void)
{
int I, NUM, CUE = 0;
int ARRE[CANT];
for (I=0; I<CANT; I++)
{
    printf("Ingrese el elemento %d del a rreglo: ", I+1);
    scanf("%d", &ARRE[I]);
}
printf("\n\nIngrese el numero que se va a buscar en el arreglo: ");
scanf("%d", &NUM);
for (I=0; I<CANT; I++)
    if (ARRE[I] == NUM)

    CUE++;
printf("\n\nEl %d se encuentra %d veces en el arreglo", NUM, CUE);
}
