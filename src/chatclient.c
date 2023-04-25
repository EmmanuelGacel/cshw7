#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "util.h"
//AP server userID: Emmanuel: 1120 Katie: 1362
//userID Equation: (1000 * x) + (120 OR 362)
int client_socket = -1;
int ip_conversion;
int retval = EXIT_SUCCESS;
char username[MAX_NAME_LEN + 1];
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1];

/*Part 4.1*/
int handle_stdin() {
    /* TODO * */
	return 1;
}
/*Part 4.2*/
int handle_client_socket() {
    /* TODO */
	return 1;
}

int main(int argc, char **argv) {
    /* TODO */
    if (argc != 3){
        fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);// Check error message
        retval = EXIT_FAILURE;
        goto END;
    }
    struct sockaddr_in server_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in); //New type
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
    server_addr.sin_port = htons(atoi(argv[2]));//Converts to Big-Endian notation

    username[MAX_NAME_LEN + 1] = '\0';
    while (1){//Ask user for username
        printf("Please enter a username");
        if (read(STDIN_FILENO, &username, sizeof(username) - 1)){
            fprintf(stderr, "Error: Failed to read user input.%s.\n",strerror(errno));
            retval = EXIT_FAILURE;
            goto END;
        } else if(username [MAX_NAME_LEN + 1] != '\0'){
            fprintf(stderr, "Sorry, limit your username to %d characters.\n.\n", MAX_NAME_LEN);
        } else if(strlen(username) < 1){
            fprintf(stderr, "Sorry, your username is too short.%s.\n",strerror(errno));
        }else break;
    }
    printf("Hello, %s. Let's try to connect to the server.\n", username);
    
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
        fprintf(stderr, "Error: Failed to create socket %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto END;
    }

    if ((connect(client_socket, (struct sockaddr *)&server_addr, addrlen))){
        fprintf(stderr, "Error: Failed to connect to the server %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto END;
    }

    int bytes_read;
    if ((bytes_read = recv(client_socket, inbuf, (sizeof(inbuf) - 1), 0)) < 0){
        fprintf(stderr, "Error: Failed to receive welcome message %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto END;
    }else if(bytes_read == 0) {
        fprintf(stderr, "Error: Server closed the connection %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto END;
    }
    inbuf[sizeof(inbuf)] = 0;
    printf("\n%s\n\n", inbuf);

    if (send(client_socket, username, sizeof(username), 0) < 0){
        fprintf(stderr, "Error: Username failed to send %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto END;
    }

END:
    return retval;
    
}
