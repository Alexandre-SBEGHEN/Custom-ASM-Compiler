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
    char *str = file_to_string(PROJECT_ROOT "/examples/asm/a_equals_a_plus_10_messycode.asm");

    printf("%s\n————————————————————\n", str);

    free(str);
    return 0;
}