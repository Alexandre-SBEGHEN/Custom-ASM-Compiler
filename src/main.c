/**
 * @file main.c
 * @brief Point d'entrée du programme
 * @author Alexandre SBEGHEN
 * @date 2026-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine.h"
#include "compiler.h"
#include "interpreter.h"

int main(void) {
    printf("Number of keywords : %lu\n", KEYWORDS_COUNT);

    char* keyword = "MAIN";
    printf("%s -> %d", keyword, get_opcode_from_keyword(keyword));

    return 0;
}