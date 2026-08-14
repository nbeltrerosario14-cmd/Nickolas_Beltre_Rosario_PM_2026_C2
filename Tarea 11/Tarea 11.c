#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOMBRE 100
#define MAX_RUTA 256

// Estructura de un Riesgo evaluado por el Método Mosler
typedef struct {
    int id;
    char nombre[MAX_NOMBRE];
    int F; // Funcion (1-5)
    int S; // Sustitucion (1-5)
    int P; // Profundidad (1-5)
    int E; // Extension (1-5)
    int A; // Agresion (1-5)
    int V; // Vulnerabilidad (1-5)

    // Resultados Calculados
    int I;  // Importancia (F x S)
    int D;  // Danos (P x E)
    int C;  // Caracter del Riesgo (I + D)
    int PB; // Probabilidad (A x V)
    int ER; // Evaluacion del Riesgo (C x PB)
    char gravedad[25];
} Riesgo;

// --- Prototipos de Funciones ---
void calcularMosler(Riesgo* r);
int validarCriterio(const char* mensaje);
void crearRiesgo(Riesgo** lista, int* cantidad);
void modificarRiesgo(Riesgo* lista, int cantidad);
void calcularTodosLosRiesgos(Riesgo* lista, int cantidad);
void imprimirRiesgos(const Riesgo* lista, int cantidad);
void borrarRiesgo(Riesgo** lista, int* cantidad);
void guardarEnArchivo(const Riesgo* lista, int cantidad, const char* nombreArchivo);
void cargarDesdeArchivo(Riesgo** lista, int* cantidad, const char* nombreArchivo);
void seleccionarOCrearArchivo(char* nombreArchivo, int crearNuevo);
void limpiarBuffer(void);

// --- Función Principal ---
int main() {
    Riesgo* listaRiesgos = NULL; // Puntero para memoria dinámica
    int cantidad = 0;
    char archivoActual[MAX_RUTA] = "datos_riesgos.csv";
    int opcion;

    do {
        printf("\n========================================================\n");
        printf("    GESTION DE RIESGOS - METODO MOSLER (Code::Blocks)  \n");
        printf("========================================================\n");
        printf(" Archivo Activo: [%s] | Riesgos en memoria: %d\n", archivoActual, cantidad);
        printf("--------------------------------------------------------\n");
        printf(" 1. Crear nuevo riesgo\n");
        printf(" 2. Modificar parametros de un riesgo\n");
        printf(" 3. Re-Calcular todos los riesgos\n");
        printf(" 4. Imprimir / Listar riesgos\n");
        printf(" 5. Borrar un riesgo\n");
        printf(" 6. Seleccionar archivo de trabajo\n");
        printf(" 7. Crear un nuevo archivo de datos\n");
        printf(" 8. Guardar cambios en el archivo\n");
        printf(" 9. Cargar/Recargar desde el archivo\n");
        printf(" 0. Salir\n");
        printf("--------------------------------------------------------\n");
        printf(" Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            limpiarBuffer();
            continue;
        }

        switch (opcion) {
            case 1:
                crearRiesgo(&listaRiesgos, &cantidad);
                break;
            case 2:
                modificarRiesgo(listaRiesgos, cantidad);
                break;
            case 3:
                calcularTodosLosRiesgos(listaRiesgos, cantidad);
                break;
            case 4:
                imprimirRiesgos(listaRiesgos, cantidad);
                break;
            case 5:
                borrarRiesgo(&listaRiesgos, &cantidad);
                break;
            case 6:
                seleccionarOCrearArchivo(archivoActual, 0);
                cargarDesdeArchivo(&listaRiesgos, &cantidad, archivoActual);
                break;
            case 7:
                seleccionarOCrearArchivo(archivoActual, 1);
                free(listaRiesgos);
                listaRiesgos = NULL;
                cantidad = 0;
                break;
            case 8:
                guardarEnArchivo(listaRiesgos, cantidad, archivoActual);
                break;
            case 9:
                cargarDesdeArchivo(&listaRiesgos, &cantidad, archivoActual);
                break;
            case 0:
                printf("\nSaliendo del sistema...\n");
                break;
            default:
                printf("\n[Opcion invalida. Intente nuevamente.]\n");
        }
    } while (opcion != 0);

    // Liberar memoria dinámica al cerrar
    free(listaRiesgos);
    return 0;
}

// --- Limpieza auxiliar del buffer de entrada ---
void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Cálculo individual según reglas de Mosler ---
void calcularMosler(Riesgo* r) {
    r->I = r->F * r->S;
    r->D = r->P * r->E;
    r->C = r->I + r->D;
    r->PB = r->A * r->V;
    r->ER = r->C * r->PB;

    // Determinar nivel de gravedad
    if (r->ER <= 250) strcpy(r->gravedad, "Muy Bajo");
    else if (r->ER <= 500) strcpy(r->gravedad, "Bajo");
    else if (r->ER <= 750) strcpy(r->gravedad, "Medio");
    else if (r->ER <= 1000) strcpy(r->gravedad, "Alto");
    else strcpy(r->gravedad, "Muy Alto (Critico)");
}

// Validar que la entrada del usuario esté entre 1 y 5
int validarCriterio(const char* mensaje) {
    int valor;
    do {
        printf("%s (1-5): ", mensaje);
        if (scanf("%d", &valor) != 1) {
            limpiarBuffer();
            valor = 0;
        }
        if (valor < 1 || valor > 5) {
            printf("  [Error: El valor debe estar entre 1 y 5]\n");
        }
    } while (valor < 1 || valor > 5);
    return valor;
}

// --- Crear Riesgo usando realloc para Memoria Dinámica ---
void crearRiesgo(Riesgo** lista, int* cantidad) {
    Riesgo* temp = (Riesgo*)realloc(*lista, (*cantidad + 1) * sizeof(Riesgo));
    if (!temp) {
        printf("\n[Error de asignacion de memoria]\n");
        return;
    }
    *lista = temp;

    Riesgo* nuevo = &((*lista)[*cantidad]);
    nuevo->id = *cantidad + 1;

    printf("\n--- NUEVO RIESGO (ID: %d) ---\n", nuevo->id);
    printf("Nombre / Descripcion del riesgo: ");
    limpiarBuffer();
    fgets(nuevo->nombre, MAX_NOMBRE, stdin);
    nuevo->nombre[strcspn(nuevo->nombre, "\r\n")] = 0;

    printf("\nEvaluacion de Criterios (Metodo Mosler):\n");
    nuevo->F = validarCriterio("  - Criterio de Funcion (F)");
    nuevo->S = validarCriterio("  - Criterio de Sustitucion (S)");
    nuevo->P = validarCriterio("  - Criterio de Profundidad (P)");
    nuevo->E = validarCriterio("  - Criterio de Extension (E)");
    nuevo->A = validarCriterio("  - Criterio de Agresion (A)");
    nuevo->V = validarCriterio("  - Criterio de Vulnerabilidad (V)");

    calcularMosler(nuevo);
    (*cantidad)++;

    printf("\n[Riesgo creado exitosamente: ER = %d (%s)]\n", nuevo->ER, nuevo->gravedad);
}

// --- Modificar parámetros de un riesgo ---
void modificarRiesgo(Riesgo* lista, int cantidad) {
    if (cantidad == 0) {
        printf("\n[No hay riesgos almacenados.]\n");
        return;
    }

    int idBusqueda;
    printf("\nIngrese el ID del riesgo a modificar: ");
    scanf("%d", &idBusqueda);

    for (int i = 0; i < cantidad; i++) {
        if (lista[i].id == idBusqueda) {
            printf("\nModificando riesgo: %s (ID %d)\n", lista[i].nombre, lista[i].id);
            lista[i].F = validarCriterio("  - Criterio de Funcion (F)");
            lista[i].S = validarCriterio("  - Criterio de Sustitucion (S)");
            lista[i].P = validarCriterio("  - Criterio de Profundidad (P)");
            lista[i].E = validarCriterio("  - Criterio de Extension (E)");
            lista[i].A = validarCriterio("  - Criterio de Agresion (A)");
            lista[i].V = validarCriterio("  - Criterio de Vulnerabilidad (V)");

            calcularMosler(&lista[i]);
            printf("\n[Riesgo actualizado correctamente: Nuevo ER = %d (%s)]\n", lista[i].ER, lista[i].gravedad);
            return;
        }
    }
    printf("\n[Riesgo con ID %d no encontrado.]\n", idBusqueda);
}

// --- Recalcular todos los riesgos ---
void calcularTodosLosRiesgos(Riesgo* lista, int cantidad) {
    if (cantidad == 0) {
        printf("\n[No hay riesgos para calcular.]\n");
        return;
    }
    for (int i = 0; i < cantidad; i++) {
        calcularMosler(&lista[i]);
    }
    printf("\n[Se re-calcularon exitosamente los %d riesgos.]\n", cantidad);
}

// --- Imprimir tabla de riesgos ---
void imprimirRiesgos(const Riesgo* lista, int cantidad) {
    if (cantidad == 0) {
        printf("\n[La lista de riesgos esta vacia.]\n");
        return;
    }

    printf("\n=========================================================================================\n");
    printf(" %-4s %-20s %-12s %-4s %-4s %-4s %-4s %-4s %-4s %-6s %-18s\n",
           "ID", "NOMBRE", "C (I+D)", "F", "S", "P", "E", "A", "V", "ER", "GRAVEDAD");
    printf("=========================================================================================\n");

    for (int i = 0; i < cantidad; i++) {
        printf(" %-4d %-20.20s %-12d %-4d %-4d %-4d %-4d %-4d %-4d %-6d %-18s\n",
               lista[i].id, lista[i].nombre, lista[i].C,
               lista[i].F, lista[i].S, lista[i].P, lista[i].E, lista[i].A, lista[i].V,
               lista[i].ER, lista[i].gravedad);
    }
    printf("=========================================================================================\n");
}

// --- Borrar riesgo y reajustar memoria ---
void borrarRiesgo(Riesgo** lista, int* cantidad) {
    if (*cantidad == 0) {
        printf("\n[No hay riesgos para eliminar.]\n");
        return;
    }

    int idBusqueda, encontrado = -1;
    printf("\nIngrese el ID del riesgo a eliminar: ");
    scanf("%d", &idBusqueda);

    for (int i = 0; i < *cantidad; i++) {
        if ((*lista)[i].id == idBusqueda) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("\n[Riesgo con ID %d no encontrado.]\n", idBusqueda);
        return;
    }

    // Desplazar elementos hacia la izquierda
    for (int i = encontrado; i < *cantidad - 1; i++) {
        (*lista)[i] = (*lista)[i + 1];
        (*lista)[i].id = i + 1; // Reindexar IDs
    }

    (*cantidad)--;
    if (*cantidad > 0) {
        *lista = (Riesgo*)realloc(*lista, (*cantidad) * sizeof(Riesgo));
    } else {
        free(*lista);
        *lista = NULL;
    }

    printf("\n[Riesgo eliminado correctamente.]\n");
}

// --- Seleccionar o Crear Archivo ---
void seleccionarOCrearArchivo(char* nombreArchivo, int crearNuevo) {
    char temp[MAX_RUTA];
    printf("\nIngrese el nombre del archivo (ejemplo: mi_riesgo.csv): ");
    limpiarBuffer();
    fgets(temp, MAX_RUTA, stdin);
    temp[strcspn(temp, "\r\n")] = 0;

    if (strlen(temp) > 0) {
        strcpy(nombreArchivo, temp);
    }

    if (crearNuevo) {
        FILE* f = fopen(nombreArchivo, "w");
        if (f) {
            fprintf(f, "ID,Nombre,F,S,P,E,A,V\n");
            fclose(f);
            printf("[Archivo '%s' creado exitosamente.]\n", nombreArchivo);
        } else {
            printf("[Error al crear el archivo.]\n");
        }
    }
}

// --- Guardar en archivo CSV plano ---
void guardarEnArchivo(const Riesgo* lista, int cantidad, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("\n[Error al abrir el archivo '%s' para guardar.]\n", nombreArchivo);
        return;
    }

    fprintf(archivo, "ID,Nombre,F,S,P,E,A,V\n");
    for (int i = 0; i < cantidad; i++) {
        fprintf(archivo, "%d,%s,%d,%d,%d,%d,%d,%d\n",
                lista[i].id, lista[i].nombre,
                lista[i].F, lista[i].S, lista[i].P,
                lista[i].E, lista[i].A, lista[i].V);
    }

    fclose(archivo);
    printf("\n[Se guardaron %d riesgos en el archivo '%s']\n", cantidad, nombreArchivo);
}

// --- Cargar desde archivo CSV a memoria dinámica ---
void cargarDesdeArchivo(Riesgo** lista, int* cantidad, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        printf("\n[Advertencia: No se pudo abrir el archivo '%s'.]\n", nombreArchivo);
        return;
    }

    free(*lista);
    *lista = NULL;
    *cantidad = 0;

    char linea[256];
    fgets(linea, sizeof(linea), archivo); // Ignorar encabezado

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\r\n")] = 0;
        if (strlen(linea) == 0) continue;

        Riesgo tempRiesgo;
        if (sscanf(linea, "%d,%99[^,],%d,%d,%d,%d,%d,%d",
                   &tempRiesgo.id, tempRiesgo.nombre,
                   &tempRiesgo.F, &tempRiesgo.S, &tempRiesgo.P,
                   &tempRiesgo.E, &tempRiesgo.A, &tempRiesgo.V) == 8) {

            calcularMosler(&tempRiesgo);

            Riesgo* ptrTemp = (Riesgo*)realloc(*lista, (*cantidad + 1) * sizeof(Riesgo));
            if (!ptrTemp) {
                printf("\n[Error de asignacion de memoria al cargar archivo.]\n");
                fclose(archivo);
                return;
            }
            *lista = ptrTemp;
            (*lista)[*cantidad] = tempRiesgo;
            (*cantidad)++;
        }
    }

    fclose(archivo);
    printf("\n[Se cargaron %d riesgos desde '%s']\n", *cantidad, nombreArchivo);
}
