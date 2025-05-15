#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int n;
    printf("Ingrese el valor de n para la sumatoria de 0 hasta n: ");
    scanf("%d", &n);
    
    // Crear el proceso hijo
    pid_t pid = fork();
    
    if (pid < 0) {
        // Error al crear el proceso hijo
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }
    
    int mitad = n / 2;
    
    if (pid == 0) {
        // Proceso hijo: calcula la suma de la segunda mitad
        int suma_hijo = 0;
        for (int i = mitad + 1; i <= n; i++) {
            suma_hijo += i;
        }
        
        printf("Hijo: Suma de %d hasta %d = %d\n", mitad + 1, n, suma_hijo);
        
        // Escribir el resultado en un archivo
        FILE *archivo_hijo = fopen("resultado_hijo.txt", "w");
        if (archivo_hijo == NULL) {
            perror("Error al abrir archivo para escritura");
            exit(EXIT_FAILURE);
        }
        
        fprintf(archivo_hijo, "%d", suma_hijo);
        fclose(archivo_hijo);
        
        exit(EXIT_SUCCESS);
    } else {
        // Proceso padre: calcula la suma de la primera mitad
        int suma_padre = 0;
        for (int i = 0; i <= mitad; i++) {
            suma_padre += i;
        }
        
        printf("Padre: Suma de 0 hasta %d = %d\n", mitad, suma_padre);
        
        // Esperar a que el hijo termine
        wait(NULL);
        
        // Leer el resultado del hijo desde el archivo
        FILE *archivo_hijo = fopen("resultado_hijo.txt", "r");
        if (archivo_hijo == NULL) {
            perror("Error al abrir archivo para lectura");
            exit(EXIT_FAILURE);
        }
        
        int suma_hijo;
        fscanf(archivo_hijo, "%d", &suma_hijo);
        fclose(archivo_hijo);
        
        // Combinar los resultados
        int suma_total = suma_padre + suma_hijo;
        
        printf("\nResultado final:\n");
        printf("Suma de 0 hasta %d = %d\n", n, suma_total);
        
        // Opcionalmente, eliminar el archivo temporal
        if (remove("resultado_hijo.txt") != 0) {
            perror("Error al eliminar el archivo temporal");
        }
    }
    
    return 0;
}
