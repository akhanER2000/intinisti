#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Nodo {
    char rut[20];
    char nombre[100];
    int valor;
    struct Nodo* izquierda;
    struct Nodo* derecha;
};

struct Nodo* crearNodo(char rut[], char nombre[], int valor) {
    struct Nodo* nodo = (struct Nodo*)malloc(sizeof(struct Nodo));
    strcpy(nodo->rut, rut);
    strcpy(nodo->nombre, nombre);
    nodo->valor = valor;
    nodo->izquierda = NULL;
    nodo->derecha = NULL;
    return nodo;
}

struct Nodo* insertar(struct Nodo* raiz, struct Nodo* nodo) {
    if (raiz == NULL) {
        return nodo;
    }

    if (strcmp(raiz->rut, nodo->rut) < 0) {
        raiz->derecha = insertar(raiz->derecha, nodo);
    } else if (strcmp(raiz->rut, nodo->rut) > 0) {
        raiz->izquierda = insertar(raiz->izquierda, nodo);
    } else {
        raiz->valor += nodo->valor;
        free(nodo);
    }

    return raiz;
}

struct Nodo* buscarRut(const char rut[], struct Nodo* raiz) {
    if (raiz == NULL || strcmp(raiz->rut, rut) == 0) {
        return raiz;
    }

    if (strcmp(raiz->rut, rut) < 0) {
        return buscarRut(rut, raiz->derecha);
    }

    return buscarRut(rut, raiz->izquierda);
}

void generarArchivoSDE(const char* archivoEntrada) {
    char archivoSalida[100];
    strcpy(archivoSalida, archivoEntrada);
    int len = strlen(archivoSalida);
    strcpy(archivoSalida + len - 3, "sde");

    FILE* archivo = fopen(archivoEntrada, "r");
    FILE* archivoSalidaPtr = fopen(archivoSalida, "w");

    if (archivo == NULL || archivoSalidaPtr == NULL) {
        printf("Error al abrir los archivos.\n");
        return;
    }

    struct Nodo* arbol = NULL;
    int totalPersonas = 0;

    char linea[200];
    while (fgets(linea, sizeof(linea), archivo)) {
        char rut[20], nombre[100];
        int valor;
        sscanf(linea, "%[^,],%[^,],%d", rut, nombre, &valor);

        struct Nodo* nodo = crearNodo(rut, nombre, valor);
        arbol = insertar(arbol, nodo);
        totalPersonas++;
    }

    int personasMasDeDosEntradas = 0;
    generarArchivoSDERecursivo(arbol, archivoSalidaPtr, &personasMasDeDosEntradas);

    fprintf(archivoSalidaPtr, "Total de personas: %d\n", personasMasDeDosEntradas);

    fclose(archivo);
    fclose(archivoSalidaPtr);

    liberarArbol(arbol);

    if (personasMasDeDosEntradas > 0) {
        printf("El archivo %s fue generado.\n", archivoSalida);
    } else {
        remove(archivoSalida);
        printf("Todos solicitaron la cantidad correcta de entradas.\n");
    }
}


void generarArchivoSDERecursivo(struct Nodo* raiz, FILE* archivoSalida, int* contador) {
    if (raiz) {
        generarArchivoSDERecursivo(raiz->izquierda, archivoSalida, contador);

        if (raiz->valor > 2) {
            fprintf(archivoSalida, "%s, %s: %d\n", raiz->rut, raiz->nombre, raiz->valor);
            (*contador)++;
        }

        generarArchivoSDERecursivo(raiz->derecha, archivoSalida, contador);
    }
}

void liberarArbol(struct Nodo* raiz) {
    if (raiz) {
        liberarArbol(raiz->izquierda);
        liberarArbol(raiz->derecha);
        free(raiz);
    }
}

int main() {
    char archivoEntrada[100];
    printf("Ingrese el nombre del archivo de entrada: ");
    scanf("%s", archivoEntrada);

    generarArchivoSDE(archivoEntrada);

    return 0;
}
