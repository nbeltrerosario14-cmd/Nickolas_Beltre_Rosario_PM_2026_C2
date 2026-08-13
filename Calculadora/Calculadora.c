#include <stdio.h>
#include <stdlib.h>
#define SALIR 0
#define SUMAR 1
#define RESTAR 2
#define MULTIPLICAR 3
#define DIVIDIR 4

void entrada(int oper);//declaracion de la funcion
double Sumar(double sumando1,double sumando2);
void salida(double numero);
double Mul(double numero1, double numero2);
double Restar(double minuendo,double sustraendo);
double Dividir(double numerador,double denominador);
//ambito de variables
//variables globales
double n1 = 0.0;//decimal de 64 bits
double n2 = 0.0;
double buf = 0.0;

int main()
{
    int menu = -1;

    printf("\nCALCULADORA BASICA");

    do
    {
        printf("\n0-Salir\n1-Sumar\n2-Restar\n3-Multiplicar\n4-Dividir\n");
        scanf("%i", &menu);
        if(menu == SUMAR)
        {
            entrada(SUMAR);
            buf = Sumar(n1,n2);//pasamos por valor
            printf("\nResuelto:%lf",buf);
        }
        else if(menu == RESTAR)
        {
            entrada(RESTAR);
            buf = Restar(n1,n2);
            printf("\nResuelto:%lf",buf);
        }
        else if(menu == MULTIPLICAR)
        {
            entrada(MULTIPLICAR);
            buf = Mul(n1,n2);
            salida(buf);
        }
        else if(menu == DIVIDIR)
        {
            entrada(DIVIDIR);
            buf = Dividir(n1,n2);
            salida(buf);
        }else
        {
            printf("\nEntrada Menu invalida\nValores validos 0 - 4\n");
        }

    }while(menu != SALIR);


    return 0;
}
void salida(double numero)
{
    printf("\nResultado:%lf",numero);
}
void entrada(int oper)
{
   if(oper == SUMAR || oper == MULTIPLICAR)
   {
       printf("\nIngrese el primer operando:");
       scanf("%lf",&n1);
       printf("\nIngrese el segundo operando:");
       scanf("%lf",&n2);
       return;
   }
   else if(oper == DIVIDIR)
   {
     printf("\nIngrese el numerador:");
        scanf("%lf",&n1);
        printf("\nIngrese el denominador:");
        scanf("%lf",&n2);
        return;
   }
   else if(oper == RESTAR)
    {
        printf("\nIngrese el minuendo:");
        scanf("%lf",&n1);
        printf("\nIngrese el sustraendo:");
        scanf("%lf",&n2);
        return;
    }
}
//recivimos por valor
double Sumar(double sumando1,double sumando2)
{
    double tmp =0.0;
    tmp = sumando1 + sumando2;
    return tmp;
}
double Mul(double numero1, double numero2)
{
   return numero1 * numero2;
}
double Restar(double minuendo,double sustraendo)
{
    double tmp =0.0;
    tmp = minuendo - sustraendo;
    return tmp;
}
double Dividir(double numerador,double denominador)
{
    return numerador / denominador;
}
