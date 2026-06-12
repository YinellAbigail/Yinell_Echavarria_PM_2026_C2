#include <stdio.h>
#define CANT_DE_JUGADAS 5

unsigned int numero_aleatorio = 246524;

void Ingresarjugadas(int numeros[], float dineros[], int cant);
void Jugarloteria(int numeros[], float dineros[], int cant);
int generarNumeroAleatorio(int min, int max);

int main(void)
{
int numerosJugados[CANT_DE_JUGADAS];
float dinerojugado[CANT_DE_JUGADAS];
int Opcion;
int JugadasIngresadas = 0;

do
{
    printf("\n== Menu de loteria (5 JUGADAS) ==\n");
    printf("1. Ingresar las 5 jugadas\n");
    printf("2. Jugar y simular sorteo\n");
    printf("3. Salir\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &Opcion);

    numero_aleatorio += Opcion * 13;

    switch(Opcion)
{
    case 1:
        Ingresarjugadas(numerosJugados, dinerojugado, CANT_DE_JUGADAS);
        JugadasIngresadas = 1;
    break;

    case 2:
        if(JugadasIngresadas == 0)
        {
        printf("\n[Error] Primero debes ingresar tus 5 jugadas (Opcion 1).\n");
        }
        else
        {
        Jugarloteria(numerosJugados, dinerojugado, CANT_DE_JUGADAS);
        }
    break;

    case 3:
        printf("\n¡Gracias por jugar! Saliendo del programa...\n");
    break;

    default:
    printf("\nOpcion no valida. Intente nuevamente.\n");
}
}
while (Opcion !=3);

return 0;
}

void Ingresarjugadas(int numeros[], float dineros[], int cant)
{
    int i;
    for (i = 0; i < cant; i++)
    {
        printf("\n--- JUGADA #%d ---\n", i + 1);

        do
        {
            printf("Pregunta 1: Ingrese el numero a jugar (1 al 40): ");
            scanf("%d", &numeros[i]);
            if (numeros[i] < 1 || numeros[i] > 40)
            {
                printf("Error: El numero debe estar entre 1 y 40.\n");
            }
        }
    while (numeros[i] < 1 || numeros[i] > 40);

    do
    {
        printf("Pregunta 2: Ingrese la cantidad de dinero a apostar: ");
        scanf("%f", &dineros[i]);
        if (dineros[i] <=0)
        {
            printf("Error: La cantidad de dinero debe ser mayor a 0.\n");
        }
    }
    while (dineros[i] <= 0);
    }
}

void Jugarloteria(int numeros[], float dineros[], int cant)
{
int numeroganador;
int i;
int huboganador = 0;

numeroganador = generarNumeroAleatorio(1,40);

printf("\n==========================================\n");
printf("¡EL NUMERO QUE SALIO ES: %d!\n", numeroganador);
printf("==========================================\n");

for (i = 0; i < cant; i++)
{
    if (numeros[i] == numeroganador)
    {
        float premio = dineros[i] * 1000;
        printf("\n¡Felicidades! Ganaste en la jugada #%d.\n", i + 1);
        printf("Numero jugado: %d | Dinero apostado: %.2f\n", numeros[i], dineros[i]);
        printf("Monto ganado: %.2f\n", premio);
        huboganador = 1;
    }
}

if (huboganador == 0)
{
    printf("\nNadie salio ganador en esta ronda.\n");
}
}

int generarNumeroAleatorio(int min, int max)
{
    numero_aleatorio = (numero_aleatorio * 1664525) + 1012904223;
    int numero_base = (int)(numero_aleatorio % 20) + 1;
    return numero_base * 2;
}
