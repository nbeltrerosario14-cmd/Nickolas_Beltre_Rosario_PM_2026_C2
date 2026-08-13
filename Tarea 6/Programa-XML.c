typedef struct {
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[100];
} Alumno;
int main() {

    const char *archivo = "alumnos.xml";   // cambiar por json o csv

    int n = contar_elementos(archivo);

    if(n <= 0){
        printf("Error al leer el archivo.\n");
        return 1;
    }

    Alumno *alumnos = (Alumno*) malloc(n * sizeof(Alumno));

    if(alumnos == NULL){
        printf("No hay memoria.\n");
        return 1;
    }

    cargar_datos(archivo, alumnos, n);

    imprimir_alumnos(alumnos, n);

    free(alumnos);

    return 0;
}
