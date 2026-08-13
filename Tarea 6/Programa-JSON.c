#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUMNOS 20

typedef struct {
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[50];
} Alumno;

int main() {
    FILE *archivo = fopen("alumnos.json", "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo 'alumnos.json'.\n");
        printf("Asegurate de que este en la misma carpeta del ejecutable.\n");
        return 1;
    }

    Alumno alumnos[MAX_ALUMNOS];
    int total = 0;
    char linea[256];

    // Recorrer el archivo buscando los campos clave
    while (fgets(linea, sizeof(linea), archivo)) {
        if (strstr(linea, "\"nombre\"") != NULL) {
            // Extraer Nombre
            sscanf(linea, " \"nombre\":\"%[^\"]\"", alumnos[total].nombre);

            // Leer siguiente linea: Apellido
            if (fgets(linea, sizeof(linea), archivo))
                sscanf(linea, " \"apellido\":\"%[^\"]\"", alumnos[total].apellido);

            // Leer siguiente linea: Promedio
            if (fgets(linea, sizeof(linea), archivo))
                sscanf(linea, " \"promedio\":%f", &alumnos[total].promedio);

            // Leer siguiente linea: Materia
            if (fgets(linea, sizeof(linea), archivo))
                sscanf(linea, " \"materia\":\"%[^\"]\"", alumnos[total].materia);

            total++;
        }
    }

    fclose(archivo);

    // Mostrar el reporte formateado en pantalla
    printf("===============================================================\n");
    printf("                  LISTADO DE ALUMNOS (JSON)                    \n");
    printf("===============================================================\n");
    printf("%-12s %-12s %-10s %-15s\n", "Nombre", "Apellido", "Promedio", "Materia");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < total; i++) {
        printf("%-12s %-12s %-10.1f %-15s\n", 
               alumnos[i].nombre, 
               alumnos[i].apellido, 
               alumnos[i].promedio, 
               alumnos[i].materia);
    }

    printf("---------------------------------------------------------------\n");
    printf("Total de alumnos procesados: %d\n", total);

    return 0;
}