#ifndef __CONSTANTS_H_
#define __CONSTANTS_H_

/*** Common Includes ***/

// C Standard Libs
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// pthread Unix based only
#include <pthread.h>

// Socket related
#include <sys/socket.h>
#include <sys/types.h>

// Networking related
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

// For Colored Output
#include "colors.h"

/*** Constants ***/
#define PORT 9999
#define HOST "127.0.0.1"
#define BACKLOG 10
#define PACKET_SIZE 1024
#define MAX_CLIENTS 128
#define EXIT_MESSAGE "--EX!T--"
#define EXIT_MESSAGE_LENGTH 8
#define IP_LENGTH 15
#define ARB_PADDING 64

#define USERNAME_LENGTH 64
#define USERNAME_SEPERATOR "`"

/*** Functions used across both server and clients ***/
// Converts the IP from bit format to a more readable string format.
void addr_to_IP(struct sockaddr_in, char*);
void ltrim(char*);
void rtrim(char*);
#endif