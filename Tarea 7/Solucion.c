#include <stdio.h>

// Función recursiva para resolver la Torre de Hanói
void hanoi(int n, char origen, char destino, char auxiliar) {
    // Caso base: si solo hay 1 disco, se mueve directamente del origen al destino
    if (n == 1) {
        printf("Mover disco 1 de %c -> %c\n", origen, destino);
        return;
    }

    // 1. Mover n-1 discos del origen a la torre auxiliar
    hanoi(n - 1, origen, auxiliar, destino);

    // 2. Mover el disco grande que queda en el origen hacia el destino
    printf("Mover disco %d de %c -> %c\n", n, origen, destino);

    // 3. Mover los n-1 discos de la torre auxiliar al destino final
    hanoi(n - 1, auxiliar, destino, origen);
}

int main() {
    int discos;

    printf("=================================\n");
    printf("    RESOLUTOR DE TORRE DE HANOI   \n");
    printf("=================================\n\n");

    printf("Ingresa el numero de discos: ");
    scanf("%d", &discos);

    if (discos <= 0) {
        printf("El numero de discos debe ser mayor a 0.\n");
        return 1;
    }

    printf("\nPasos a seguir (Torre origen 'A', Torre destino 'C', Torre auxiliar 'B'):\n\n");
    
    // Llamada inicial: Origen = A, Destino = C, Auxiliar = B
    hanoi(discos, 'A', 'C', 'B');

    return 0;
}