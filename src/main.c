/**
 * @file main.c
 * @brief Point d'entrée du programme
 * @author Alexandre SBEGHEN
 * @date 2026-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myarray.h"
#include "mystring.h"
#include "machine.h"
#include "compiler.h"
#include "interpreter.h"

int main(void) {
    char* code = file_to_string(PROJECT_ROOT "/examples/asm/a_equals_a_plus_10.asm");
    char* code_without_comments = remove_comments(code);
    string* keywords = string_to_keywords(code_without_comments);
    Token** tokens = keywords_to_tokens(keywords);

    for (size_t i = 0; i < array_size(tokens); ++i) {
        Token* t = tokens[i];
        printf("token[type: %d, val: %d, label: %s]\n", t->type, t->value, t->label);
    }
    return 0;
}