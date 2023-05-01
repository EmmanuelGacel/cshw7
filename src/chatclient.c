#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include "util.h"
#include <netinet/tcp.h>
//AP server userID: Emmanuel: 1120 Katie: 1362
//userID Equation: (1000 * x) + (120 OR 362)
int client_socket = -1;
int ip_conversion;
int retval = EXIT_SUCCESS;
char username[MAX_NAME_LEN + 1];
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1];


/*
volatile sig_atomic_t running = true;


void catch_signal(int sig) {
    running = false;
    printf("/n");
}
*/

/*Part 4.1*/
int handle_stdin() {

/**	
    char buf[MAX_MSG_LEN + 1];
    if (fgets(username, MAX_NAME_LEN + 1, stdin) == NULL) {
        fprintf(stderr, "Error reading username from stdin: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Remove newline character from username
    username[strcspn(username, "\n")] = '\0';

    while (fgets(buf, MAX_MSG_LEN + 1, stdin) != NULL) {
        // Remove newline character from message
        buf[strcspn(buf, "\n")] = '\0';

        // Check if message is too long
        if (strlen(buf) == MAX_MSG_LEN && !feof(stdin)) {
            fprintf(stderr, "Sorry, limit your message to 1 line of at most %d characters.\n", MAX_MSG_LEN);
            while (fgets(buf, MAX_MSG_LEN + 1, stdin) != NULL && buf[strlen(buf) - 1] != '\n') {
                continue;
            }
            continue;
        }

        // Copy message to outbuf
        strncpy(outbuf, buf, MAX_MSG_LEN);

        // Send message to server
        if (send(client_socket, outbuf, strlen(outbuf), 0) < 0) {
            fprintf(stderr, "Error sending message to server: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }

        // Check if message is "bye"
        if (strcmp(buf, "bye") == 0) {
            printf("Goodbye.\n");
            close(client_socket);
            exit(EXIT_SUCCESS);
        }

        // Check for errors in stdin
        if (ferror(stdin)) {
            fprintf(stderr, "Error reading from stdin: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
    }

    // End of file reached on stdin, exit gracefully
    return EXIT_SUCCESS;

    outbuf[MAX_MSG_LEN] = '\0';
    while (true){
        //int i = 0;
        //char ch;
        //bool is_true_eof;
        //is_true_eof = false;
        outbuf[0] = '\0';
        errno = 0;
        fflush(stdin);
        
        
        /START
        while((ch = getc(stdin) != '\n') && (i < MAX_MSG_LEN)){
             printf("Character: %c\n", ch);
             printf("Decimal: %d\n", ch);
            if (errno == EINTR) {
                printf("\n");
                return EXIT_FAILURE;
            } else if (ch == EOF) {
                if (isatty(STDIN_FILENO)) {
                    // This code handles CTRL+D from a terminal.
                    if (strlen(outbuf) == 0) {
                        printf("\n");
                        return EXIT_SUCCESS;
                    } else {
                        clearerr(stdin);
                        continue;
                    }
                } else {
                    // This code handles EOF when STDIN is redirected.
                    //is_true_eof = true;
                    break;
                }
            } else {
               outbuf[i++] = ch;
            }
        }
        //END
        if (fgets(outbuf, MAX_MSG_LEN, stdin) == NULL) {
            fprintf(stderr, "Error: Read interrupted. %s. \n", strerror(errno));
            return EXIT_FAILURE;
        }
        if(outbuf[MAX_MSG_LEN] != '\0'){
            printf("Sorry, limit your message to 1 line of at most %d characters.\n", MAX_MSG_LEN);
        }
        outbuf[strlen(outbuf) - 1] = '\n';
        //printf("Message: %s\n", outbuf);
        //outbuf[i++] = '\0';
        fflush(stdin);
        int msg_size;
        if ((msg_size = send(client_socket, outbuf, strlen(outbuf), 0)) < 0){
            fprintf(stderr, "Error: Message failed to send. %s.\n", strerror(errno));
        }
        //printf("Message Size: %d\n", msg_size);
        if(strcmp(outbuf, "bye") == 0){
            printf("Goodbye.\n");
            return 2;
        }
        return EXIT_SUCCESS;

    }
 **/
    //char message[MAX_MSG_LEN + 1]; //SUB FOR OUTBUF
    memset(outbuf, 0, MAX_MSG_LEN);

    // read input from the user
    if (fgets(outbuf, MAX_MSG_LEN, stdin) == NULL) {
        fprintf(stderr, "Error: Read interrupted. %s. \n", strerror(errno));
        return EXIT_FAILURE;
    }

    // remove the newline
    
    int len = strlen(outbuf);
   
   
    if (outbuf[len - 1] == '\n') {
        outbuf[len - 1] = '\0';
    }
    
    // check if message is too long
    if (len >= MAX_MSG_LEN) {
        fprintf(stderr, "Sorry, limit your message to 1 line of at most %d characters.\n", MAX_MSG_LEN);
        // discard the rest of the characters
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    } else {
        //printf("\n----------MADE IT: 1----------\n");
        // send the message to the server
        //printf("Message: %s\n", message);
        int ret = send(client_socket, outbuf, strlen(outbuf), 0);
        if (ret == -1) {
            fprintf(stderr, "Error: Message failed to send. %s.\n", strerror(errno));
        }
        //printf("\n----------MADE IT: 2----------\n");
        // check if the user wants to quit
        if (strcmp(outbuf, "bye") == 0) {
            printf("Goodbye.\n");
            close(client_socket);
            exit(EXIT_SUCCESS);
        }
        //fflush(stdout); //make sure the server prints it
    }
    //printf("\n----------MADE IT: 3----------\n");
    return EXIT_SUCCESS;

    
    /**
    outbuf[MAX_MSG_LEN] = '\0';
    //int first_line = 1;
    char username[MAX_NAME_LEN + 1];

    // Read username from first line
    if (fgets(username, MAX_NAME_LEN + 1, stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read username. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    username[strcspn(username, "\n")] = 0; // Remove newline character

    while (true) {
        outbuf[0] = '\0';
        errno = 0;
        fflush(stdin);
        
        if (fgets(outbuf, MAX_MSG_LEN, stdin) == NULL) {
            if (feof(stdin)) {
                // End of file reached
                break;
            }
            fprintf(stderr, "Error: Read interrupted. %s. \n", strerror(errno));
            return EXIT_FAILURE;
        }

        if(outbuf[MAX_MSG_LEN] != '\0'){
            printf("Sorry, limit your message to 1 line of at most %d characters.\n", MAX_MSG_LEN);
            // Consume remaining characters
            return EXIT_FAILURE;
        
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
            
        }

        outbuf[strlen(outbuf) - 1] = '\n';
        int msg_size;
        if ((msg_size = send(client_socket, outbuf, strlen(outbuf), 0)) < 0){
            fprintf(stderr, "Error: Message failed to send. %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }

        if(strcmp(outbuf, "bye") == 0){
            printf("Goodbye.\n");
            close(client_socket);
            exit(EXIT_SUCCESS);
            return 2;
        }
    }

    return EXIT_SUCCESS;
    **/
}

/*Part 4.2*/
int handle_client_socket() {
    int bytes_read, arg_len;
    arg_len = sizeof(client_socket);
    arg_len ++;
    
    if ((bytes_read = recv(client_socket, inbuf, sizeof(inbuf), 0)) < 0){ //receive messages into inbuf
        fprintf(stderr, "Error: Failed to receive welcome message %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }else if(bytes_read == 0) {
        fprintf(stderr, "Error: Server closed the connection %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }else{
        inbuf[sizeof(inbuf) - 1] = '\0';

        printf("Received message: %s\n", inbuf);

        if (strcmp(inbuf, "bye\n") == 0) {
            printf("\nServer initiated shutdown.\n");
            close(client_socket);
            exit(EXIT_SUCCESS);
            //return 2;//Sends the main program straight to END: EXIT_SUCESS
        }
    
    }
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    /* TODO */
    if (argc != 3){
        fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);// Check error message
        retval = EXIT_FAILURE;
        goto END;
    }
    
    /**
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = catch_signal;
    if (sigaction(SIGINT, &action, NULL) == -1) {
        fprintf(stderr, "Error: Failed to register signal handler. %s.\n",
                strerror(errno));
        return EXIT_FAILURE;
    }
    */
    
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

    username[MAX_NAME_LEN ] = '\0';
    
    while (1){//Ask user for username
    
    	printf("Please enter a username: ");
    	fflush(stdout); //makes sure its displayed before the read since theres no newline
   	if (read(STDIN_FILENO, &username, sizeof(username) - 1) < 0) {
        	fprintf(stderr, "Error: Failed to read user input.\n");
        	retval = EXIT_FAILURE;
        	goto END;
   	} else if (strlen(username) > MAX_NAME_LEN) {
        	fprintf(stderr, "Sorry, limit your username to %d characters.\n", MAX_NAME_LEN);
        	continue; //ask for another username
    	} else if (strlen(username) < 1) {
        	fprintf(stderr, "Sorry, your username is too short.\n");
        	continue; //ask for another username
    	} 
        break;
    }
    char *eoln = strchr(username, '\n'); //fixed bug, usernames contained newlines
    if(eoln != NULL){
        *eoln = '\0'; //overwrite newline
    }
    
    printf("\nHello, %s. Let's try to connect to the server.\n", username);
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
        fprintf(stderr, "Error: Failed to create socket %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto END;
    }
    if ((connect(client_socket, (struct sockaddr *)&server_addr, addrlen)) < 0){
        fprintf(stderr, "Error: Failed to connect to the server %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto END;
    }
    int flag = 1;
    int result = setsockopt(client_socket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
    if (result < 0) {
        perror("Error: setsockopt failed");
        close(client_socket);
        exit(EXIT_FAILURE);
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
    inbuf[BUFLEN] = '\0';
    printf("\n%s\n\n", inbuf);

    if (send(client_socket, username, sizeof(username), 0) < 0){
        fprintf(stderr, "Error: Username failed to send %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto END;
    }
    
    //setting up the fd set
    
    fd_set readfds;
    
    
    while (1){
    	
    	FD_ZERO(&readfds);
    	FD_SET(client_socket, &readfds);
    	FD_SET(STDIN_FILENO, &readfds);
    	
    	printf("[%s]:", username);
    	fflush(stdout); //makes sure it prints since theres no newline
    	
    	//check for initial errors on both
    	if (select(FD_SETSIZE, &readfds, NULL, NULL, NULL) < 0) {
            fprintf(stderr,"Error: select() failed. %s.\n", strerror(errno));
            retval = EXIT_FAILURE;
            goto END;
        }
        
        //activity on STDIN_FILENO
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
        	if(handle_stdin() == EXIT_FAILURE){
        		retval = EXIT_FAILURE;
        		goto END;
        	}
        	/*
        	else if(retval == 2){
        	retval = EXIT_SUCCESS;
                goto END;
                }
                */
        }
        //printf("\n----------MADE IT: 1----------\n");
        //activity on the socket
        if (FD_ISSET(client_socket, &readfds)) {
            int retval; 
        	if((retval = handle_client_socket()) == EXIT_FAILURE){
        		retval = EXIT_FAILURE;
        		goto END;
        	}
        	/*
        	else if(retval == 2){
        	retval = EXIT_SUCCESS;
                goto END;
                }
                */
        }
        //fflush(client_socket);
        //printf("\n----------MADE IT: 2----------\n");
    	
    }
    
END:
    if (fcntl(client_socket, F_GETFD) != -1) { //this only is activated if theres a Ctrl+D
        close(client_socket);
    }
    return retval;   
}


