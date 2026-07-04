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
    char* str = file_to_string(PROJECT_ROOT
        "/tests/testdata/test_compiler/test_code_without_comments/very_tricky_code.asm");
    char* str_w_o_comments = code_without_comments(str);
    char** tokens = array_of_isolated_keywords(str_w_o_comments);

    for (size_t i = 0; tokens[i] != NULL; i++)
        printf("%s\n", tokens[i]);

    free(tokens[0]);
    free(tokens);
    free(str_w_o_comments);
    free(str);
    return 0;
}