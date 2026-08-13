#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUMNOS 50
#define TAM_CADENA 50

// Estructura para almacenar los datos de cada alumno
typedef struct {
    char nombre[TAM_CADENA];
    char apellido[TAM_CADENA];
    float promedio;
    char materia[TAM_CADENA];
} Alumno;

int main() {
    FILE *archivo = fopen("alumnos.csv", "r");
    
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo 'alumnos.csv'.\n");
        printf("Asegurate de que el archivo exista en el mismo directorio que el ejecutable.\n");
        return 1;
    }

    Alumno alumnos[MAX_ALUMNOS];
    char linea[256];
    int contador = 0;

    // 1. Leer y descartar la primera linea (encabezados: Nombre,Apellido,Promedio,Materia)
    fgets(linea, sizeof(linea), archivo);

    // 2. Leer cada linea del CSV
    while (fgets(linea, sizeof(linea), archivo) != NULL && contador < MAX_ALUMNOS) {
        // Eliminar el salto de linea al final si existe
        linea[strcspn(linea, "\r\n")] = 0;

        // Extraer los valores separados por coma usando sscanf
        // %49[^,] lee hasta encontrar una coma
        if (sscanf(linea, "%49[^,],%49[^,],%f,%49[^\n]",
                   alumnos[contador].nombre,
                   alumnos[contador].apellido,
                   &alumnos[contador].promedio,
                   alumnos[contador].materia) == 4) {
            contador++;
        }
    }

    fclose(archivo);

    // 3. Imprimir reporte en consola
    printf("===================================================================\n");
    printf("                   LISTADO DE ALUMNOS (CSV)                        \n");
    printf("===================================================================\n");
    printf(" %-12s %-12s %-10s %-15s\n", "NOMBRE", "APELLIDO", "PROMEDIO", "MATERIA");
    printf("-------------------------------------------------------------------\n");

    float sumaPromedios = 0;
    float mejorPromedio = -1.0;
    int indiceMejor = 0;

    for (int i = 0; i < contador; i++) {
        printf(" %-12s %-12s %-10.2f %-15s\n",
               alumnos[i].nombre,
               alumnos[i].apellido,
               alumnos[i].promedio,
               alumnos[i].materia);

        sumaPromedios += alumnos[i].promedio;

        if (alumnos[i].promedio > mejorPromedio) {
            mejorPromedio = alumnos[i].promedio;
            indiceMejor = i;
        }
    }

    printf("===================================================================\n");
    if (contador > 0) {
        printf(" Total de registros leidos: %d\n", contador);
        printf(" Promedio general del grupo: %.2f\n", sumaPromedios / contador);
        printf(" Alumno con mayor promedio:  %s %s (%.2f en %s)\n",
               alumnos[indiceMejor].nombre,
               alumnos[indiceMejor].apellido,
               alumnos[indiceMejor].promedio,
               alumnos[indiceMejor].materia);
    }
    printf("===================================================================\n");

    return 0;
}