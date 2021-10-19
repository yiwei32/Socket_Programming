/*
    fileName, server_IPaddress, portNumber
    argv[0] : filename
    argv[1] : server_IPaddress
    argv[2] : portNumber
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <pthread.h>

#define BUFSIZE 256
#define SOCKETERROR (-1)
#define NAMELENGTH 32

volatile int flag = 0;
int sockfd = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void catch_ctrl_c_and_exit(int sig){
    flag = 1;
}

void check(int exp, const char *msg);
void str_overwrite_stdout();
void str_trim_lf(char *arr, int length);
void* send_msg_handler();
void* recv_msg_handler();

int main(int argc, char** argv){
    int portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    if (argc < 3){
        fprintf(stderr, "server IP address or portNumber not provided. Program terminated.\n");
        exit(1);
    }
    portno = atoi(argv[2]);
    check(sockfd = socket(AF_INET, SOCK_STREAM, 0), "Error opening socket.");
    check((server = gethostbyname(argv[1])) == NULL, "No such host.");
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // h_addr == h_addr_list[0]
    bcopy((char *) server->h_addr,(char *) &serv_addr.sin_addr.s_addr, server->h_length); 
    serv_addr.sin_port = htons(portno);

    // connect to server
    check(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)), "Connection failed.");

    // Naming
    char buffer[BUFSIZE];
    printf("Please enter your name:\n");
    do {
        scanf("%s", buffer);
        if (strlen(buffer) > NAMELENGTH){
            printf("Length of your name should be less than %d, try again.\n",NAMELENGTH + 1);
        }
        else{
            break;
        }
    } while (true);
    // Send name to server
    send(sockfd, buffer, NAMELENGTH + 1, 0);
    printf("Hi %s, welcome to the chatroom!\n", buffer);

    
    pthread_t send_msg_thread;
    if (pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
        printf("Thread creation failed.\n");
        exit(EXIT_FAILURE);
    };

    pthread_t recv_msg_thread;
    if (pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
        printf("Thread creation failed.\n");
        exit(EXIT_FAILURE);
    };


    while(true){
        if (flag){
            printf("Bye\n");
            break;
        }
    }
    close(sockfd);
    return 0;
}

void check(int exp, const char *msg){
    if (exp == SOCKETERROR){
        perror(msg);
        exit(1);
    }
};

void str_overwrite_stdout(){
    printf("\r%s", "> ");
    fflush(stdout);
};

void str_trim_lf(char *arr, int length){
    for (int i = 0; i < length; ++i){
        if (arr[i] == '\n'){
            arr[i] = '\0';
            break;
        }
    }
};

void* send_msg_handler(){
    char buffer[BUFSIZE];
    while (true){
        str_overwrite_stdout();
        fgets(buffer, BUFSIZE, stdin);
        str_trim_lf(buffer, BUFSIZE);

        if (strcmp(buffer, "exit") == 0){
            break;
        }
        else{
            send(sockfd, buffer, strlen(buffer), 0);
        }
        bzero(buffer, BUFSIZE);
    }
    catch_ctrl_c_and_exit(2);
    pthread_detach(pthread_self());
    return NULL;
};

void* recv_msg_handler(){
    char msg[BUFSIZE+NAMELENGTH];
    while (1){
        int bytesIn = recv(sockfd, msg , BUFSIZE+NAMELENGTH, 0);
        if (bytesIn > 0){
            printf("%s\n", msg);
            str_overwrite_stdout();
        }
        else if (bytesIn == 0){
            break; // disconnected
        }
        bzero(msg, BUFSIZE+NAMELENGTH);
    }
    pthread_detach(pthread_self());
    return NULL;
};

