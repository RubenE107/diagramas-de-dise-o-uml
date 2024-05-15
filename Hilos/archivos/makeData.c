#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*

Escriba un programa que lea la siguiente base de datos, y la
ordene por nombre utilizando un archivo índice. El programa debe
imprimir el contenido de la tabla utilizando el archivo indice.
Note que la tabla debe quedar sin cambios.

Para el ordenamiento utilice cualquier algoritmo de ordenamiento.

*/

typedef struct {
    unsigned int clave; // se utiliza para identificación de la tupla
    char nombre[21],
         telefono[11];
    double sueldo;
} Empleado;

int main(){
    Empleado empleados[] = {
            {1, "juan",    "210848",   8240.99  }, // registro 0
            {2, "aurora",  "710848",   3240.34  }, // registro 1
            {3, "roberto", "810848",   9240.23  }, // registro 2
            {4, "celia",   "910848", 103240.23  }, // registro 3
            {5, "cecilia", "310848",  13240.27  }, // registro 4
            {6, "ramon",   "450848",  32540.23  }, // registro 5
            {7, "jacinto", "830848",  32460.55  }, // registro 6
        };
    printf("record size = %ld\n", sizeof(Empleado) );
    int fd = open("db.personas", O_WRONLY|O_CREAT|O_TRUNC, 0600 );
    write( fd, (void *)empleados, 7*sizeof( Empleado) );
    close(fd);
    return 0;
}
