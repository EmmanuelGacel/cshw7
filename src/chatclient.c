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
//completed version
int client_socket = -1;
int ip_conversion;
int retval = EXIT_SUCCESS;
char username[MAX_NAME_LEN + 1];
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1]; //can hold the newline and null terminator



/*Part 4.1*/
int handle_stdin() {
    outbuf[MAX_MSG_LEN + 1] = '\0'; 
    outbuf[0] = '\0';
    errno = 0;
    
    char temp_buf[MAX_MSG_LEN + 2]; //has space for newline and terminator
    
    if (fgets(temp_buf, sizeof(temp_buf), stdin) == NULL) {
        if(feof(stdin)) return 2;
        printf("Message: %s\n", temp_buf);
        fprintf(stderr, "Error: Read interrupted. %s. \n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    //printf("TEMP SIZE: %ld\n", strlen(temp_buf));
    char * newline = NULL;
    if((newline = strchr(temp_buf, '\n')) == NULL){//Checks to see if a new line was read, if not then the message was too long
        printf("Sorry, limit your message to 1 line of at most %d characters.\n", MAX_MSG_LEN);
        char ch = getc(stdin);
        while((ch != '\n') && (ch != EOF)) ch = getc(stdin);//Consumes unread characters
        return EXIT_SUCCESS; //Because it is not a fatal error
    }
    *newline = '\0';//Replaces newline with null terminator
    //printf("Message: %s\n", outbuf);
    strcpy(outbuf, temp_buf);
    
    int msg_size;
    if ((msg_size = send(client_socket, outbuf, strlen(outbuf)+1 , 0)) < 0){
        fprintf(stderr, "Error: Message failed to send. %s.\n", strerror(errno));
    }
    //printf("Message: %s\n", outbuf);
    //printf("Message Size: %d\n", msg_size);
    if(strcmp(outbuf, "bye") == 0){
        printf("Goodbye.\n");
        return 2;
    }
    return EXIT_SUCCESS;
}
/*Part 4.2*/
int handle_client_socket() {
    int bytes_read, arg_len;
    arg_len = sizeof(client_socket);
    arg_len ++;
    if ((bytes_read = recv(client_socket, inbuf, sizeof(inbuf), 0)) < 0){
        fprintf(stderr, "Error: Failed to receive welcome message %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }else if(bytes_read == 0) {
        fprintf(stderr, "\nServer initiated shutdown %s.\n", strerror(errno));
        return 2; //ON GIVEN EXECUTABLE, THIS IS A SUCCESS AFTER WELCOME MESSAGE IS RECIEVED
    }else{
        inbuf[bytes_read + 1] = '\0'; //(vs sizeof(infbuf)  - 1)
	
        printf("\n%s\n", inbuf); //PRINTS THE RECIEVED MESSAGE
        fflush(stdout);

        if (strcmp(inbuf, "bye\n") == 0) {
            printf("\nServer initiated shutdown.\n");
            return 2;//Sends the main program straight to END: EXIT_SUCESS
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
    char new_buf[MAX_NAME_LEN + 2];
    
    
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
   	    if (fgets(new_buf, MAX_NAME_LEN + 2, stdin) == NULL) {
        	fprintf(stderr, "Error: Failed to read user input.\n");
        	retval = EXIT_FAILURE;
        	goto END;
        } 
        char * newline = NULL;

        //for(int i = 0; i < MAX_NAME_LEN; i++){
            //printf("Index: %d; Decimal: %d; Char: %c\n",i, new_buf[i], new_buf[i]);
        //}
        if (strlen(new_buf) < 1) {
            fprintf(stderr, "Sorry, your username is too short.\n");
            continue; //ask for another username
        }else if ((newline = strchr(new_buf, '\n')) == NULL) {
            char ch = getc(stdin);
            while((ch != '\n') && (ch != EOF)) ch = getc(stdin);//Consumes unread characters
            printf("stdin: %d\n", feof(stdin));
            fprintf(stderr, "Sorry, limit your username to %d characters.\n", MAX_NAME_LEN);
            continue; //ask for another username
        }
        *newline = '\0';
        strcpy(username, new_buf);
        
        //printf("Username %c\n", username[19]);
        //printf("Max name len %d\n", MAX_NAME_LEN);
        //printf("Username %c\n", username[MAX_NAME_LEN]);
        //printf("Next character %c\n", getc(stdin));
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
    
    
    username[strlen(username)] = '\0'; //null terminate username
    
    if (send(client_socket, username, strlen(username) + 1, 0) < 0){
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
    	
    	
    	if (isatty(fileno(stdin))) { //only prints this when not reading from a file
    		printf("[%s]:", username);
   		fflush(stdout);
	}
    	
    	//check for initial errors on both
    	if (select(client_socket + 1, &readfds, NULL, NULL, NULL) < 0) { // Client socekt + 1
           
	    fprintf(stderr,"Error: select() failed. %s.\n", strerror(errno));
            retval = EXIT_FAILURE;
            goto END;
        }
        
        //activity on STDIN_FILENO
        if (FD_ISSET(STDIN_FILENO, &readfds)) {	
        	int messageval = handle_stdin();
        	if(messageval  == EXIT_FAILURE){
        		retval = EXIT_FAILURE;
        		goto END;
        	}else if(messageval == 2){
                goto END;
            }
        }
        //activity on the socket
        if (FD_ISSET(client_socket, &readfds)) {
            int messageval = handle_client_socket();
        	if(messageval  == EXIT_FAILURE){
        		retval = EXIT_FAILURE;
        		goto END;
        	}else if(messageval == 2){
                goto END;
            }
        }
    	
    }
    
END:
    if (fcntl(client_socket, F_GETFD) != -1) {
        close(client_socket);
    }
    return retval;   
}


