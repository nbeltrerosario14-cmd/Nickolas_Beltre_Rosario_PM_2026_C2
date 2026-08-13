#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ANCHO_VENTANA 900
#define ALTO_VENTANA 500
#define NUM_ELEMENTOS 60      // Cantidad de barras a ordenar
#define DELAY_MS 15           // Velocidad del ordenamiento (menor = mas rapido)

/* ---------- Estructura del Estado del Algoritmo ---------- */
typedef struct {
    int valores[NUM_ELEMENTOS];
    int i;                    // Pasada actual
    int j;                    // Indice de comparacion actual
    bool completado;          // Indica si el arreglo ya esta ordenado
    int comparaciones;        // Contador de comparaciones
    int intercambios;         // Contador de intercambios
} EstadoSort;

/* ---------- Funciones de Inicializacion ---------- */

void mezclarArreglo(EstadoSort *estado) {
    for (int k = 0; k < NUM_ELEMENTOS; k++) {
        // Genera valores de altura entre 20 y ALTO_VENTANA - 80
        estado->valores[k] = 20 + rand() % (ALTO_VENTANA - 100);
    }
    estado->i = 0;
    estado->j = 0;
    estado->completado = false;
    estado->comparaciones = 0;
    estado->intercambios = 0;
}

/* ---------- Logica: Un paso de Bubble Sort ---------- */
void pasoBubbleSort(EstadoSort *estado) {
    if (estado->completado) return;

    estado->comparaciones++;

    // Si el elemento actual es mayor que el siguiente, se intercambian
    if (estado->valores[estado->j] > estado->valores[estado->j + 1]) {
        int temp = estado->valores[estado->j];
        estado->valores[estado->j] = estado->valores[estado->j + 1];
        estado->valores[estado->j + 1] = temp;
        estado->intercambios++;
    }

    // Avanzar indice interno
    estado->j++;

    // Al llegar al final de una pasada:
    if (estado->j >= NUM_ELEMENTOS - estado->i - 1) {
        estado->j = 0;
        estado->i++;

        // Si ya se hicieron todas las pasadas, finalizar
        if (estado->i >= NUM_ELEMENTOS - 1) {
            estado->completado = true;
        }
    }
}

/* ---------- Renderizado Gráfico ---------- */

void dibujarBarras(SDL_Renderer *ren, EstadoSort *estado) {
    int anchoBarra = ANCHO_VENTANA / NUM_ELEMENTOS;

    for (int k = 0; k < NUM_ELEMENTOS; k++) {
        int altura = estado->valores[k];
        int x = k * anchoBarra;
        int y = ALTO_VENTANA - altura;

        SDL_Rect r = { x + 1, y, anchoBarra - 2, altura };

        // Definicion de colores segun el estado de la barra:
        if (estado->completado) {
            // Verde: Algoritmo terminado
            SDL_SetRenderDrawColor(ren, 46, 204, 113, 255);
        } else if (k == estado->j || k == estado->j + 1) {
            // Rojo: Barras comparandose actualmente
            SDL_SetRenderDrawColor(ren, 231, 76, 60, 255);
        } else if (k >= NUM_ELEMENTOS - estado->i) {
            // Azul claro: Elementos ya ordenados en su posicion final
            SDL_SetRenderDrawColor(ren, 52, 152, 219, 255);
        } else {
            // Blanco/Gris: Elementos pendientes de ordenar
            SDL_SetRenderDrawColor(ren, 220, 221, 225, 255);
        }

        SDL_RenderFillRect(ren, &r);
    }
}

/* ---------- Funcion Principal ---------- */

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    srand((unsigned int)time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error al iniciar SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *ventana = SDL_CreateWindow(
        "Visualizador de Bubble Sort - SDL2",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        ANCHO_VENTANA, ALTO_VENTANA, SDL_WINDOW_SHOWN);

    if (!ventana) {
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

    EstadoSort estado;
    mezclarArreglo(&estado);

    bool ejecutando = true;
    bool pausado = false;
    SDL_Event evento;
    Uint32 ultimoPaso = SDL_GetTicks();

    printf("=== Visualizador de Ordenamiento ===\n");
    printf("Controles:\n");
    printf("  [ ESPACIO ] : Pausar / Reanudar\n");
    printf("  [ R ]       : Reiniciar / Mezclar barras\n");
    printf("  [ ESC ]     : Salir\n\n");

    /* ---------- Game Loop ---------- */
    while (ejecutando) {

        /* 1. Manejo de Eventos */
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                ejecutando = false;
            } else if (evento.type == SDL_KEYDOWN) {
                switch (evento.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        ejecutando = false;
                        break;
                    case SDLK_SPACE:
                        pausado = !pausado;
                        break;
                    case SDLK_r:
                        mezclarArreglo(&estado);
                        pausado = false;
                        break;
                }
            }
        }

        /* 2. Logica de Actualizacion */
        Uint32 ahora = SDL_GetTicks();
        if (!pausado && !estado.completado && (ahora - ultimoPaso >= DELAY_MS)) {
            pasoBubbleSort(&estado);
            ultimoPaso = ahora;
        }

        /* 3. Renderizado */
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255); // Fondo oscuro
        SDL_RenderClear(renderer);

        dibujarBarras(renderer, &estado);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    printf("Estadisticas finales:\n");
    printf(" - Comparaciones: %d\n", estado.comparaciones);
    printf(" - Intercambios: %d\n", estado.intercambios);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return EXIT_SUCCESS;
}