#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/unix_socket" // has to be the same as in server
                                       // if not then client will not send data to server
#define BUFFER_SIZE 1024 // buffer size for storing server response

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_un addr;
    char response[BUFFER_SIZE];

    // create socket
    sock = socket(AF_UNIX, SOCK_STREAM, 0);

    // define socket type and path
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    // connect to server
    if( connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1 ) {
        perror("connect");
        return EXIT_FAILURE;
    }

    while(1) {
        int n, i;

        // input number of numbers until you press 0
        printf("Enter ints (0 to quit): ");
        scanf("%d", &n);

        if(n <= 0) { break; } // exit 

        // allocate memory for the numbers
        int *numbers = malloc(n * sizeof(int));

        // enter the numbers
        printf("Enter %d ints: ", n);
        for(i=0 ; i<n ; i++) {
            scanf("%d", &numbers[i]);
        }

        // send the sequence size to the server
        write(sock, &n, sizeof(int));
        // send the sequence contents to the server
        write(sock, numbers, n*sizeof(int));
        // free them, useless now
        free(numbers);

        // read response from server
        if(read(sock, response, BUFFER_SIZE) <= 0) {
            printf("Server closed the connection.\n");
            break;
        }

        // print server response
        printf("Server response:\n%s\n", response);
    }
    
    // close connection (socket)
    close(sock);
    return EXIT_SUCCESS;
}