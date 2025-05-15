#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int n;
    printf("Ingrese el valor de n para la sumatoria de 0 hasta n: ");
    scanf("%d", &n);
    
    // Crear la tubería (pipe)
    int fd[2];
    if (pipe(fd) == -1) {
        perror("Error al crear la tubería");
        exit(EXIT_FAILURE);
    }
    
    // Crear el proceso hijo
    pid_t pid = fork();
    
    if (pid < 0) {
        // Error al crear el proceso hijo
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }
    
    int suma = 0;
    int mitad = n / 2;
    
    if (pid == 0) {
        // Proceso hijo: calcula la suma de la segunda mitad
        close(fd[0]); // Cerramos el extremo de lectura porque no lo vamos a usar
        
        int suma_hijo = 0;
        for (int i = mitad + 1; i <= n; i++) {
            suma_hijo += i;
        }
        
        printf("Hijo: Suma de %d hasta %d = %d\n", mitad + 1, n, suma_hijo);
        
        // Enviar resultado al padre a través de la tubería
        write(fd[1], &suma_hijo, sizeof(int));
        close(fd[1]); // Cerramos el extremo de escritura
        
        exit(EXIT_SUCCESS);
    } else {
        // Proceso padre: calcula la suma de la primera mitad
        close(fd[1]); // Cerramos el extremo de escritura porque no lo vamos a usar
        
        int suma_padre = 0;
        for (int i = 0; i <= mitad; i++) {
            suma_padre += i;
        }
        
        printf("Padre: Suma de 0 hasta %d = %d\n", mitad, suma_padre);
        
        // Recibir resultado del hijo a través de la tubería
        int suma_hijo;
        read(fd[0], &suma_hijo, sizeof(int));
        close(fd[0]); // Cerramos el extremo de lectura
        
        // Combinar los resultados
        int suma_total = suma_padre + suma_hijo;
        
        // Esperar a que el hijo termine
        wait(NULL);
        
        printf("\nResultado final:\n");
        printf("Suma de 0 hasta %d = %d\n", n, suma_total);
    }
    
    return 0;
}
