#include "header/utils.h"

void addr_to_IP(struct sockaddr_in addr, char* ip_dest) {
    /*
        Courtesy of SoapBox from https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c
    */
    // Formats the IP to convert to a readable string format.
    sprintf(ip_dest, "%d.%d.%d.%d",
        (int)(addr.sin_addr.s_addr & 0xFF),
        (int)((addr.sin_addr.s_addr & 0xFF00) >> 8),
        (int)((addr.sin_addr.s_addr & 0xFF0000) >> 16),
        (int)((addr.sin_addr.s_addr & 0xFF000000) >> 24));
}

void ltrim(char* string) {
    int index = 0;

    while (1) {
        if (string[index] == '\0') {
            break;
        }
        if (!(string[index] == '\n' || string[index] == '\t' || string[index] == ' ')) {
            break;
        }
        else {
            int new_counter = index;
            while (1) {
                if (string[new_counter] == '\0') {
                    break;
                }

                string[new_counter] = string[new_counter + 1];
                new_counter++;
            }
        }
    }
}

void rtrim(char* string) {
    size_t string_length = strlen(string);

    for (int i = string_length - 1; i >= 0;i--) {
        if (string[i] == ' ' || string[i] == '\n' || string[i] == '\t') {
            string[i] = '\0';
            rtrim(string);
        }
        else return;
    }
}