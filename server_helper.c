#include "header/server_helper.h"

/*** External Variables declared in server.c ***/
extern int client_count;
extern client clients[MAX_CLIENTS];
extern pthread_t recv_threads[MAX_CLIENTS];

// This function returns a server socket after initializing and configuring it.
int init_socket() {
    // Initializes a server socket. Its a TCP Socket, and sets the Address Family to be used for IPv4 
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // If the server_socket returns a -ve number in this case -1. It means there was an error
    if (server_socket == -1) {
        colored_output(RED, "Error: Server Socket Failed!");
        exit(EXIT_FAILURE);
    }

    // Creates and initializes a sockaddr_in struct, telling it the Address Family, Host (IP) and Port to be used for the connection.
    struct sockaddr_in server_addr = {
        .sin_port = htons(PORT),            // Sets the Port on which the server listens.
        .sin_family = AF_INET,              // Sets the INET Family.
        .sin_addr.s_addr = inet_addr(HOST)  // Sets the IP Address on which the server listens.
    };

    // Binds the server_socket to server_addr struct.
    bind(server_socket,
        (struct sockaddr*)&server_addr,
        sizeof(server_addr));

    // Tells the server to listen on this configured socket and have a certain backlog.
    listen(server_socket, BACKLOG);

    return server_socket;
}

// This methods gets called by connections_thread of the main() of server.c
void* connections_listener(void* socket) {
    char message[PACKET_SIZE] = { '\0' };
    sprintf(message, "Waiting for connections at %s:%d", HOST, PORT);
    colored_output(YEL, message);

    while (1) {
        // Defines a sockaddr_in struct that will store the client's address information.
        struct sockaddr_in client_addr;
        // It holds the length of the sockaddr_in
        int addr_length;

        // Gets the server_socket as argument
        int server_socket = *(int*)socket;

        // Accepts the connection for a client that is asking for connect to the server and stores it temporarily.
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addr_length);

        // If the client_socket returns -1 then an error has occurred and exits the program with an EXIT_FAILURE code.
        if (client_socket == -1) {
            colored_output(BRED, "Error: There was a problem connecting to Client");
            exit(EXIT_FAILURE);
        }

        // Requests for a memory of IP_LENGTH bytes on the heap.
        char* IP = &(clients[client_count].IP)[0];
        // Conversion of IP from bit values to a string.
        addr_to_IP(client_addr, IP);
        client* current_client = &clients[client_count];

        // Fills the current_client object with the relavent information.
        set_client_object(client_addr, IP, client_socket, client_count, current_client);

        char welcome_message[PACKET_SIZE] = { '\0' };
        // Formats the message accordingly.
        sprintf(welcome_message, "%s%s has joined the chat!\n%s", GRN, current_client->IP, RESET_COLOR);
        // Broadcasts it to all activate users.
        broadcast_message(welcome_message);

        // Adds a reception listener to the current client object.
        add_reception_listener(client_count);

        // Increments the client_count.
        client_count++;

        // Logs that a client with a certain IP has joined.
        char logging_message[PACKET_SIZE] = { '\0' };
        sprintf(logging_message, "Connected to the client (%s), Total Users: %d", IP, client_count);
        colored_output(GRN, logging_message);
        printf("\n");
    }
    return NULL;
}
void set_client_object(struct sockaddr_in addr, char* IP, int socket, int index, client* object) {
    // Dereferences the pointer and sets their respective values.
    object->addr_info = addr;
    object->socket = socket;
    object->index = index;
}

void add_to_clients(client* new_client, int index) {
    // Adds the dereferenced version of pointer holding the new_client in the clients array
    clients[index] = *new_client;
}

void* reception_listener(void* client_object_raw) {
    // Dereferences the client_object and stores it client_object for local usage.
    client client_object = *(client*)client_object_raw;

    while (1) {
        // Declares a char[] for incomming user message.
        char text[PACKET_SIZE - IP_LENGTH] = { '\0' };

        // Receives the message and saves it in text variable.
        recv(client_object.socket, text, PACKET_SIZE, 0);

        // Checks if the user has requested to end the connection if so it handles it by breaking the loop.
        if (strncmp(text, EXIT_MESSAGE, strlen(EXIT_MESSAGE)) == 0)
            break;

        // Initializes another string to NULL to store a formatted version of the message.
        char message[PACKET_SIZE + 2 * ARB_PADDING] = { '\0' };
        // Formats the string by prefixing it with IP of the user.
        char text_cpy[PACKET_SIZE] = { '\0' };
        strcpy(text_cpy, text);
        client_object.username = strtok(text_cpy, USERNAME_SEPERATOR);
        char text_message[PACKET_SIZE] = { '\0' };
        substring(&text_message[0], text, strlen(text_cpy) + 1, strlen(text));
        sprintf(message, "%s(%s)%s: %s%s%s\n", BCYN, client_object.username, RESET_COLOR, BWHT, text_message, RESET_COLOR);

        // Logs that the X User has sent Z messsage.
        char logging_message[PACKET_SIZE + 3 * ARB_PADDING] = { '\0' };
        sprintf(logging_message, "%sUser%s %s", BBLU, RESET_COLOR, message);
        rtrim(logging_message); // Trims the nessage from right side.
        colored_output(BLU, logging_message);

        // Boardcasts the message to all connected users.
        broadcast_message(message);
    }
    // Loops gets broken if that user wishes to leave. And then it's connection is ended.
    close_connection((client*)client_object_raw);
    return NULL;
}

void close_connection(client* client_object) {
    // Logs what user has left the chat.
    char message[PACKET_SIZE] = { '\0' };
    sprintf(message, "%s has disconnected!", client_object->IP);
    colored_output(RED, message);

    // Closes the socket connection for that user.
    close(client_object->socket);

    // Broadcasts the message of disconnection to all the other connected users.
    char exit_message[PACKET_SIZE] = { '\0' };
    sprintf(exit_message, "%s%s has left the chat%s\n", RED, client_object->IP, RESET_COLOR);
    broadcast_message(exit_message);

    // Decrements the client_count so the next user can take their place.
    client_count--;
}

void add_reception_listener(int index) {
    pthread_create(&recv_threads[index], NULL, reception_listener, (void*)&clients[index]);
}

void broadcast_message(char message[PACKET_SIZE]) {
    // Iterates over all the clients and sends that message 
    for (int i = 0; i < client_count; i++) {
        // Stores the client[i] locally.
        client current_client = clients[i];
        // Checks if the client's socket is a valid socket id.
        if (current_client.socket != -1) {
            // Send the actual message.
            send(current_client.socket, message, PACKET_SIZE, 0);
        }
    }
}

void substring(char* dest, char* src, int start, int end) {
    int indexer = 0;
    for (int i = start; (i < end) || src[i] != '\0'; i++, indexer++) {
        dest[indexer] = src[i];
    }
}