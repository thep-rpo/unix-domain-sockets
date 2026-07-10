#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include<pthread.h>

#define SOCKET_PATH "/tmp/unix_socket" // define socket path
#define BUFFER_SIZE 1024 // buffer size to send message in

// function to handle multiple clients 
// for every client there is a thread
void *handle_client(void *arg);

int main(int argc, char *argv[]) {
    int server_sock, *client_sock_ptr;
    struct sockaddr_un addr;

    unlink(SOCKET_PATH); //remove if old socket exists

    // new unix socket
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);

    // give values for characteristics of socket
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    
    // bind socket to the path
    bind(server_sock, (struct sockaddr *)&addr, sizeof(addr));
    
    // listen to connections (max = 5)
    listen(server_sock, 5);

    printf("Server listening on %s...\n", SOCKET_PATH);

    while(1) {
        // accept clients request to connect
        int client_sock = accept(server_sock, NULL, NULL);
        
        // allocate memory to parse client socket to thread handling function
        client_sock_ptr = malloc(sizeof(int));
        *client_sock_ptr = client_sock;

        pthread_t tid;
        // make thread to handle client
        pthread_create(&tid, NULL, handle_client, client_sock_ptr);
        pthread_detach(tid);
    }

    // should never run, is there for cleanup
    close(server_sock);
    unlink(SOCKET_PATH);
}

void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    free(arg); // free memory allocated from arg, useless now

    while(1) {
        int n;
        
        // read how many numbers client will send
        if( read(client_sock, &n, sizeof(int)) <= 0 ) {
            break;
        }
        
        // allocate memory for these numbers
        int *numbers = malloc(n * sizeof(int));
        if( read(client_sock, numbers, n*sizeof(int)) <= 0) {
            free(numbers);
            break;
        }

        // calc sum
        int sum = 0;
        int i;
        for(i=0 ; i<n ; i++) {
            sum += numbers[i];
        }

        // calc average
        float average = (float)sum/n;
        char response[BUFFER_SIZE];

        // put appropriate message in response
        if(average > 20) {
            snprintf(response, sizeof(response), "Sequence OK\n");
        }
        else {
            snprintf(response, sizeof(response), "Check failed\n");
        }

        // send response to client
        write(client_sock, response, strlen(response)+1);
        // cleanup
        free(numbers);
    }

    // close connection
    close(client_sock);
    return NULL;
}