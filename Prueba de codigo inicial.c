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
struct Nodo* buscarRut(const char rut[], struct Nodo* raiz);  // Prototipo de función
void actualizarRuts(struct Nodo* raiz, FILE* archivo) {
    if (raiz) {
        actualizarRuts(raiz->izquierda, archivo);

        struct Nodo* nodo = buscarRut(raiz->rut, raiz);
        if (nodo != raiz) {
            nodo->valor += raiz->valor;  // Sumar el valor al RUT repetido
            free(raiz);
        } else {
            fprintf(archivo, "%s,%s,%d\n", raiz->rut, raiz->nombre, raiz->valor);
        }

        actualizarRuts(raiz->derecha, archivo);
    }
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

void liberarArbol(struct Nodo* raiz) {
    if (raiz) {
        liberarArbol(raiz->izquierda);
        liberarArbol(raiz->derecha);
        free(raiz);
    }
}

int main() {
    FILE* archivo1 = fopen("datos1.txt", "r");
    FILE* archivo2 = fopen("datos2.txt", "r");
    FILE* archivo3 = fopen("datos3.txt", "r");
    FILE* archivoActualizado = fopen("datos_actualizados.txt", "w");

    struct Nodo* arbol = NULL;

    char linea[200];
    while (fgets(linea, sizeof(linea), archivo1)) {
        char rut[20], nombre[100];
        int valor;
        sscanf(linea, "%[^,],%[^,],%d", rut, nombre, &valor);

        struct Nodo* nodo = crearNodo(rut, nombre, valor);
        arbol = insertar(arbol, nodo);
    }

    while (fgets(linea, sizeof(linea), archivo2)) {
        char rut[20], nombre[100];
        int valor;
        sscanf(linea, "%[^,],%[^,],%d", rut, nombre, &valor);

        struct Nodo* nodo = crearNodo(rut, nombre, valor);
        arbol = insertar(arbol, nodo);
    }

    while (fgets(linea, sizeof(linea), archivo3)) {
        char rut[20], nombre[100];
        int valor;
        sscanf(linea, "%[^,],%[^,],%d", rut, nombre, &valor);

        struct Nodo* nodo = crearNodo(rut, nombre, valor);
        arbol = insertar(arbol, nodo);
    }

    actualizarRuts(arbol, archivoActualizado);

    fclose(archivo1);
    fclose(archivo2);
    fclose(archivo3);
    fclose(archivoActualizado);

    liberarArbol(arbol);

    return 0;
}
