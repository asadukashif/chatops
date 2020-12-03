#include "./header/utils.h"

/* Prototypes */
int init_socket();
void* reception_listener(void*);
void exit_program();
char* get_username(int, char**);

int network_socket;


int main(int argc, char** argv) {
    // Listens for a SIGINT interrupt and uses it to cleanup and then close the program.
    signal(SIGINT, exit_program);
    char _username[USERNAME_LENGTH] = { '\0' };
    char* username = &_username[0];
    username = get_username(argc, argv);

    // Gets a configured network socket for sending and receiving data.
    network_socket = init_socket();

    pthread_t recv_thread;  // Thread initialization to be used for receiving server messages and then starts that thread.
    pthread_create(&recv_thread, NULL, reception_listener, (void*)&network_socket);

    while (1) {
        char message[PACKET_SIZE - USERNAME_LENGTH] = { '\0' };
        fgets(message, PACKET_SIZE, stdin); // Gets user input. Used in place of scanf due to it's limitations.

        ltrim(message); // Trims the string from the left side.

        // Checks if the message entered is whitespaces only. 
        if (strlen(message) == 0) {
            continue;
        }

        // Ensures if the message is less than the PACKET_SIZE
        if (strlen(message) <= PACKET_SIZE) {
            char complete_message[PACKET_SIZE] = { '\0' };
            strcat(complete_message, username);
            strcat(complete_message, USERNAME_SEPERATOR);
            strcat(complete_message, message);
            send(network_socket, &complete_message, sizeof(message), 0); // Sent to the server
        }
        else {  // Logs an error message indicate that the buffer is overflown.
            char error_message[PACKET_SIZE] = { '\0' };
            sprintf(error_message, "Message should be below %d characters.", PACKET_SIZE);
            colored_output(RED, error_message);
            continue;
        }
    }
}

// Configures and returns the nertwork socket that will be used to communicate with the server.
int init_socket() {
    // Creating the TCP socket having IPv4 Address Family.
    int network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Checks if the client socket is -1 which indicates error and if so then exits the program.
    if (network_socket == -1) {
        colored_output(BRED, "Error: Server Socket Failed!");
        exit(EXIT_FAILURE);
    }

    // Initializes a sockaddr_in struct that will store the server's information
    struct sockaddr_in server_addr = {
        .sin_port = htons(PORT),            // Port to connect to the server
        .sin_family = AF_INET,              // IP Family i.e. IPv4 Address Family
        .sin_addr.s_addr = inet_addr(HOST)  // IP Address is set at which the server is listening.
    };

    // Tries to connect to the server.
    int conn_status = connect(network_socket,
        (struct sockaddr*)&server_addr,
        sizeof(server_addr)
    );

    // If the connection status is -1 this means server was unable to establish the connection and also exits the program with the EXIT_FAILURE code.
    if (conn_status == -1) {
        colored_output(BRED, "Error: There was an error connecting to the server!");
        exit(EXIT_FAILURE);
    }

    colored_output(BGRN, "Connected to Server!");
    return network_socket;
}

// This gets called when the recv_thread is created.
void* reception_listener(void* socket) {
    // Gets the client_socket in order to communicate with the server.
    int client_socket = *((int*)socket);

    while (1) {
        char message[PACKET_SIZE] = { '\0' };

        // Receives the message and stores it in message.
        recv(client_socket, &message, sizeof(message), 0);

        // Checks to see the if the server wants to end the connection.
        if (strncmp(EXIT_MESSAGE, message, EXIT_MESSAGE_LENGTH) == 0) {
            colored_output(RED, "Server Requested to end the connection!");
            break;
        }
        // Else logs the message to the server's screen.
        printf("%s", message);
    }

    exit_program();
    return NULL;
}


// This gets called if the SIGINT interrupt is detected or when program is to be terminated.
void exit_program() {
    // Creates a message that is to be displayed at successful exit.
    char message[PACKET_SIZE] = { '\0' };

    sprintf(message, "%sConnection Terminated...\n%s", BRED, RESET_COLOR);  // Also colors the output as red.

    // Writes the message to STDOUT. Didn't use printf because it's not thread safe and program may crash.
    write(STDOUT_FILENO, message, sizeof(message));

    send(network_socket, EXIT_MESSAGE, EXIT_MESSAGE_LENGTH, 0);

    close(network_socket);
    exit(EXIT_SUCCESS);
}

char* get_username(int argc, char** argv) {
    if (argc <= 1) {
        printf("Usage: ./bin/client <Username>\n");
        exit(1);
    }
    return argv[1];
}