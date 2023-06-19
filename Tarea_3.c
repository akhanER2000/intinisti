#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Cliente{
    char Rut[12];
    char Nombre[50];
    int Numero_entradas;
}Cliente;
typedef struct ClienteEntrada{
    Cliente cliente;
    struct ClienteEntrada* siguiente;
}ClienteEntrada;
ClienteEntrada* crear_nodo(Cliente cliente){
    ClienteEntrada* nuevo_nodo = (ClienteEntrada*)malloc(sizeof(ClienteEntrada));
    nuevo_nodo->cliente = cliente;
    nuevo_nodo->siguiente = NULL;
    return nuevo_nodo;
}
typedef struct NodoCliente {
    Cliente cliente;
    struct NodoCliente* siguiente;
} NodoCliente;
NodoCliente* lista_clientes_asignados = NULL;
NodoCliente* lista_clientes_espera = NULL;
void guardar_lista_en_archivo_asigandos(NodoCliente* lista){
    FILE* archivo;
    archivo = fopen("clientes.asg", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    NodoCliente* actual = lista;
    while (actual != NULL) {
        fprintf(archivo, "%s %s %d\n", actual->cliente.Rut, actual->cliente.Nombre, actual->cliente.Numero_entradas);
        actual = actual->siguiente;
    }
    fclose(archivo);
}
void guardar_lista_en_archivo_Espera(NodoCliente* lista){
    FILE* archivo;
    archivo = fopen("clientes.esp", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    NodoCliente* actual = lista;
    while (actual != NULL) {
        fprintf(archivo, "%s %s\n", actual->cliente.Rut, actual->cliente.Nombre);
        actual = actual->siguiente;
    }
    fclose(archivo);
}
int abrir_leer_archivo(){
	/* Declaramos la variable fichero como puntero a FILE. */
	FILE *fichero;
    FILE *fichero2;
    ClienteEntrada* lista_clientes = NULL;
    Cliente cliente;
	/* Abrimos "fichero1.txt" en modo texto y
	 * guardamos su direccion en el puntero. */
	fichero = fopen("datos1.txt", "r");
	fichero2 = fopen("datos2.txt", "r");
	if(fichero == NULL && fichero2 == NULL ){
		/* Imprimimos un mensaje para indicar que no existe. */
		printf("Los ficheros no se han podido abrir, no existen.");
        return (-1);
	} 
    else {
		/* Imprimimos mesaje de exito y la direccion para este ejemplo. */
		printf("El fichero 1 existe y esta en la direccion: %p\n\n",fichero);
		printf("El fichero 2 existe y esta en la direccion: %p\n\n",fichero2);
	}
return 0;
}
int main() {
    abrir_leer_archivo();
    return 0;
}