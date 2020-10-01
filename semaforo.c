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
 *  Ese es el programa de los semaforos, 
 *  responde a las señales enviadas por el código
 *  de consola.c
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define TCP_PORT 8000

int main(int argc, const char *argv[])
{
    struct sockaddr_in direccion;
    char buffer[1000];

    int cliente;

    ssize_t leidos, escritos;

    if (argc != 2)
    {
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }

    // Crear el socket
    cliente = socket(PF_INET, SOCK_STREAM, 0);

    // Establecer conexión
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;

    escritos = connect(cliente, (struct sockaddr *)&direccion, sizeof(direccion));

    if (escritos == 0)
    {
        printf("Conectado a %s:%d \n", inet_ntoa(direccion.sin_addr), ntohs(direccion.sin_port));

        // Escribir datos en el socket
        while ((leidos = read(fileno(stdin), &buffer, sizeof(buffer))))
        {
            write(cliente, &buffer, leidos);

            /* Lee del buffer y escribe en pantalla */
            leidos = read(cliente, &buffer, sizeof(buffer));
            write(fileno(stdout), &buffer, leidos);
        }
    }

    // Cerrar sockets
    close(cliente);

    return 0;
}
