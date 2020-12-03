#ifndef __HELPER_H_
#define __HELPER_H_

/*** Constants ***/

/* This file holds all the common things like.
    1. Macros
    2. Includes
    3. Functions
*/
#include "utils.h"

#include <assert.h>

/*** Structs ***/
// This struct acts as a client object. It is holds it's IP, Address Information, Socket and Index
typedef struct client {
    struct sockaddr_in addr_info;
    char IP[IP_LENGTH];
    int socket;
    int index;
    char* username;
} client;

/*** Helper functions ***/
int init_socket();
void* connections_listener(void*);
void add_reception_listener(int);
void* reception_listener(void*);
void set_client_object(struct sockaddr_in, char*, int, int, client*);
void add_to_clients(client*, int);
void add_reception_listener(int);
void broadcast_message(char[PACKET_SIZE]);
void close_connection(client*);
void free_client(int);
void substring(char*, char*, int, int) ;

#endif