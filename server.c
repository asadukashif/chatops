#include "header/server_helper.h"

// This varibale is SHARED among files.
int client_count = 0;

// This array holds all the clients that are being connected to the server.
client clients[MAX_CLIENTS];

// This holds all the message listener thread
pthread_t recv_threads[MAX_CLIENTS];
pthread_t connections_thread;

/* Protypes */
void cleanup(int num);

int main() {
    // This handles the System Interrupt of SIGINT i.e. When Control + C is pressed on a running program. cleanup is the function that runs to handle this all.                                             
    signal(SIGINT, cleanup);

    // Initializes the server socket and make it usable for send and receiving of data.
    int server_socket = init_socket();

    // This starts the thread that listens for the incomming connections and call the function connections_listener.
    pthread_create(&connections_thread, NULL, connections_listener, (void*)&server_socket);

    // This joins the thread looking for connections with the main thread of the program.
    pthread_join(connections_thread, NULL);
}

void cleanup(int num) {
    char message[PACKET_SIZE] = { '\0' };
    sprintf(message, "%sExiting...\n%s", BRED, RESET_COLOR);    // Also colors the message

    // Writes the text message to the STDOUT
    write(STDOUT_FILENO, message, sizeof(message));

    // This broadcasts the message of server's termination to all connected clients and then they all disconnect.
    broadcast_message(EXIT_MESSAGE);

    exit(EXIT_SUCCESS);
}
