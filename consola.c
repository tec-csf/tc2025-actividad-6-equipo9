/*
 *  Actividad 6:
 *  Sockets y Señales
 * 
 *  Programación Avanzada
 *  Profesor: Vicente Cubells
 * 
 *  Daniel Roa              -   A01021960
 *  Miguel Monterrubio      -   A01
 * 
 *  Este código fue hecho en conjunto con:
 *  Sergio Hernández
 *  Antonio Junco
 * 
 *  Ese es el programa de la consola de monitoreo, 
 *  le envía señales a los semaforos que se encuentran
 *  en el código de cruce.c
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define TCP_PORT 8000
#define STOPLIGHT 4

void copy(int senial){
    printf("Este es un demo, si imprimió el valor de C %d\n", senial);
}

void undo(int senial){
    static int contOut = 0;
    printf("Este es un demo, si imprimió el valor de Z %d\n", senial);

    printf("%d\n", contOut);

    if (++contOut <= 2){
        exit(-1);
    }
    
}

int main(int argc, const char *argv[])
{
    struct sockaddr_in direccion;
    char buffer[1000];
    size_t bufferSize = sizeof(buffer);

    int servidor, cliente;

    ssize_t leidos, escritos;

    int continuar = 1;

    pid_t pid;

    //signal(SIGINT, copy);
    signal(SIGTSTP, undo);

    if (argc != 2)
    {
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }

    // Crear el socket
    servidor = socket(PF_INET, SOCK_STREAM, 0);

    // Enlace con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;

    escritos = sizeof(direccion);
    bind(servidor, (struct sockaddr *)&direccion, escritos);

    // Escuhar especificamente para 4 semaforos
    listen(servidor, STOPLIGHT);

    // Aceptar conexiones
    while (continuar)
    {
        cliente = accept(servidor, (struct sockaddr *)&direccion, &escritos);

        printf("Aceptando conexiones en %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));

        pid = fork();

        if (pid == 0){
            continuar = 0;
        }
    }

    if (pid == 0)
    {
        signal(SIGINT, copy);
        signal(SIGTSTP, undo);

        close(servidor);

        if (cliente >= 0)
        {
            read(cliente, &buffer, bufferSize);

            // Leer datos del socket
            while (leidos = read(cliente, &buffer, sizeof(buffer)))
            {
                write(fileno(stdout), &buffer, leidos);

                /* Leer de teclado y escribir en el socket */
                leidos = read(fileno(stdin), &buffer, sizeof(buffer));
                write(cliente, &buffer, leidos);
            }
        }

        close(cliente);
    }

    else if (pid > 0)
    {
        while (wait(NULL) != -1);

        // Cerrar sockets
        close(servidor);
    }
    return 0;
}
