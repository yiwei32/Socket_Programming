#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>

#define BACKLOG 5
#define BUFSIZE 256
#define NAMELENGTH 32
#define SOCKETERROR -1


typedef struct{
    char* name;
    bool isNamed;
}clientInfo;
// Global variables

clientInfo ClientList[BACKLOG + 3];
int ClientCount = 0;
int server_socket = 0;

void check(int exp, const char *msg);
void str_overwrite_stdout();
void str_trim_lf(char *arr, int length);
int setup_server(int port, int backlog);
void accept_new_connection(fd_set *current_sockets);
void handle_connection(int client_socket, fd_set *current_sockets);


// two command line arguments 
// one is the file name, the other is the port number
int main(int argc, char *argv[]){
    
    if (argc < 2){
        fprintf(stderr, "Port number not provided. Program terminated\n");
        exit(1);
    }

    server_socket = setup_server(atoi(argv[1]), BACKLOG);

    fd_set current_sockets, ready_sockets;

    // initialize current set
    FD_ZERO(&current_sockets);
    FD_SET(server_socket, &current_sockets); // add my server_socket to the current set
    

    while (1){
        ready_sockets = current_sockets;

        // Arguments: 2nd for reading, 3rd for writing, 4th for error, 5th for timeout
        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, 0) < 0) {
            perror("select error");
            exit(EXIT_FAILURE);
        }
        // ready_set have changed the status of those ready-for-reading sockets 
        for (int socket = 0; socket < FD_SETSIZE; ++socket){
            if (FD_ISSET(socket, &ready_sockets)){
                if (socket == server_socket){
                    // Accept a new connection
                    accept_new_connection(&current_sockets);
                }
                else{
                    // Accept a new message
                    handle_connection(socket, &current_sockets);
                }
            }
        }
    }

    return 0;
}

void check(int exp, const char *msg){
    if (exp == SOCKETERROR){
        perror(msg);
        exit(1);
    }
}

void str_trim_lf(char *arr, int length){
    for (int i = 0; i < length; ++i){
        if (arr[i] == '\n'){
            arr[i] = '\0';
            break;
        }   
    }
};

void str_overwrite_stdout(){
    printf("\r%s", "> ");
    fflush(stdout);
};

int setup_server(int port, int backlog){
    int server_socket, addr_size;
    struct sockaddr_in server_addr;

    check(server_socket = socket(AF_INET, SOCK_STREAM, 0), "Failed to create socket");

    // Initiate address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr =INADDR_ANY;
    server_addr.sin_port = htons(port);

    check(bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)), "Binding failed");
    check(listen(server_socket, backlog), "Listening, failed");

    printf("Server has been set up, wait for connections...\n");

    return server_socket;
};

void accept_new_connection(fd_set *current_sockets){
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    int client_socket;
    check(client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_len),
             "Accepting failed");
    ClientCount++;
    FD_SET(client_socket, current_sockets);
};

void handle_connection(int client_socket, fd_set *current_sockets){
    char buffer[BUFSIZE];
    char msg[BUFSIZE + NAMELENGTH];
    bzero(buffer, BUFSIZE);
    bzero(msg, BUFSIZE+NAMELENGTH);
    int bytesIn = recv(client_socket, buffer, BUFSIZE, 0);
    if (bytesIn <= 0){
        // Drop the client
        ClientCount--;
        // Ctrl + c after naming
        if (ClientList[client_socket].isNamed){
            char ByeMsg[BUFSIZE];
            sprintf(ByeMsg, "%s (Socket #%d) has left. Now we have %d people in the chatroom!", 
                ClientList[client_socket].name, client_socket, ClientCount);
        
            free(ClientList[client_socket].name);
            ClientList[client_socket].isNamed = false;

            printf("%s\n", ByeMsg);
            for (int outSocket = 0; outSocket < FD_SETSIZE; ++outSocket){
                if (outSocket != server_socket && outSocket != client_socket){
                    send(outSocket, ByeMsg, strlen(ByeMsg), 0);
                }
            }
        }
        else{
            // ctrl + c before naming, just do nothing
        }
        close(client_socket);
        FD_CLR(client_socket, current_sockets);
    }
    else{
        // Naming message
        if (ClientList[client_socket].isNamed == false){
            ClientList[client_socket].name = malloc((strlen(buffer) + 1) * sizeof(char));
            strcpy(ClientList[client_socket].name, buffer);
            ClientList[client_socket].isNamed = true;

            sprintf(buffer, "A new member! %s (Socket #%d) has joined!", ClientList[client_socket].name, client_socket);
            printf("%s\n", buffer);
            for (int outSocket = 0; outSocket < FD_SETSIZE ; ++outSocket){
                if (outSocket != server_socket && outSocket != client_socket && ClientList[outSocket].isNamed){
                send(outSocket, buffer, strlen(buffer), 0);
                }
            }
        }
        // Real conversation
        else{
            // Send message to the ohter clients
            sprintf(msg, "%s (Socket #%d): %s",ClientList[client_socket].name, client_socket, buffer);
            printf("%s\n", msg);

            // Broadcast to all clients except for the speaking one
            for (int outSocket = 0; outSocket < FD_SETSIZE; ++outSocket){
                if (outSocket != server_socket && outSocket != client_socket && ClientList[outSocket].isNamed){
                    send(outSocket, msg, BUFSIZE+NAMELENGTH, 0);
                }
            }
        }
    }
};
