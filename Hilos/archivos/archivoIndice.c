#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
    unsigned int clave;
    char nombre[21];
    char telefono[11];
    double sueldo;
} Empleado;

// Función de comparación para qsort
int compare(const void *a, const void *b) {
    return strcmp(((Empleado *)a)->nombre, ((Empleado *)b)->nombre);
}

int main() {
    Empleado empleados[7]; // Definir el arreglo de empleados
    int fd = open("db.personas", O_RDONLY); // Abrir el archivo de la base de datos

    if (fd == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Leer los datos de la base de datos
    read(fd, empleados, sizeof(empleados));
    close(fd);

    // Crear un índice de empleados
    Empleado *indice[7];
    for (int i = 0; i < 7; i++) {
        indice[i] = &empleados[i];
    }

    // Ordenar el índice por nombre
    qsort(indice, 7, sizeof(Empleado *), compare);

    // Imprimir los registros utilizando el índice ordenado
    printf("Clave\tNombre\t\tTelefono\tSueldo\n");
    for (int i = 0; i < 7; i++) {
        printf("%u\t%s\t\t%s\t%.2f\n", indice[i]->clave, indice[i]->nombre, indice[i]->telefono, indice[i]->sueldo);
    }

    return 0;
}
