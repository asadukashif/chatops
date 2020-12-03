#ifndef __COLORS_H__
#define __COLORS_H__

/*** Color Code Macros ***/
/** Taken from https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a */

// RESET Rule
#define RESET_COLOR "\e[0m"

//Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

#include "utils.h"
#include <stdio.h>
void colored_output(char*, char*);

#endif