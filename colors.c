#include "header/colors.h"

void colored_output(char* COLORCODE, char* message) {
    printf("%s%s%s\n", COLORCODE, message, RESET_COLOR);
}