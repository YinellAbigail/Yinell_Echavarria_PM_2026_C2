#include <stdio.h>
#include <stdlib.h>

void main(void)
{
int i;
double d;
long l;
char cad0[20], *cad1;

printf("\nIngrese una cadena de caracteres: ");
gets(cad0);
i = atoi(cad0);
printf("\n%s \t %d", cad0, i+3);

printf("\nIngrese una cadena de caracteres: ");
gets(cad0);
d = atof(cad0);
printf("\n%s \t %.21f ", cad0, d+1.50);

d = strtod(cad0, &cad1);
printf("\n%s \t %.21f", cad0, d+1.50);
puts(cad1);

l = atol(cad0);
printf("\n%s \t %1d", cad0, 0);

l = strtol(cad0, &cad1, 0);
printf("\n%s \t %1d", cad0, 1+10);
puts(cad1);
}
