#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "util.h"
//AP server userID: 1120
//userID Equation: 1000 * x * 12
int client_socket = -1;
int ip_conversion;
int retval = EXIT_SUCCESS;
char username[MAX_NAME_LEN + 1];
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1];


int handle_stdin() {
    /* TODO */
}

int handle_client_socket() {
    /* TODO */
}

int main(int argc, char **argv) {
    /* TODO */
    if (argc != 3){
        fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);// Check error message
        retval = EXIT_FAILURE;
        goto END;
    }
    struct sockaddr_int server_addr;
    socklen_t addrlen = sizeof(struct sockaddder_in);
    char *addr_str = argv[1];

    ip_conversion = inet_pton(AF_INET, addr_str, &server_addr);
    if (ip_conversion == 0){
        fprintf(stderr, "Error: Invalid IP address '%s'.\n", argv[2]);
        retval = EXIT_FAILURE;
        goto END;
    }else if (ip_conversion < 0){
        fprintf(stderr, "Error: Failed to convert IP address '%s', %s.\n", argv[2], strerror(errno));
        retval = EXIT_FAILURE;
        goto END;
    }

    memset(&server_addr, 0, addrlen);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = argv[2];

    username[MAX_NAME_LEN + 1] = NULL;
    while (1){//Ask user for username
        printf("Please enter a username");
        if (read(stdin, &username, sizeof(username) - 1)){
            fprintf(stderr, "Error: Failed to read user input.\n",strerror(errno));
            retval = EXIT_FAILURE;
            goto END;
        } else if(username [MAX_NAME_LEN + 1] != NULL){
            fprintf(stderr, "Sorry, limit your username to %d characters.\n.\n", MAX_NAME_LEN);
        } else if(strlen(username) < 1){
            fprintf(stderr, "Sorry, your username is too short.\n",strerror(errno));
        }else break;
    }
    printf("Hello, %s. Let's try to connect to the server.\n", username);
    
    

END:
    return retval;
    
}