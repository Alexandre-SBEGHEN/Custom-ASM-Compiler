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
    char* complex_code_with_comments = file_to_string(
        PROJECT_ROOT "/tests/testdata/test_compiler/test_remove_comments/very_tricky_code.asm"
    );
    char* complex_code_without_comments = remove_comments(complex_code_with_comments);

    string* keywords = string_to_keywords(complex_code_without_comments);

    for (size_t i = 0; i < array_size(keywords); i++)
        printf("%s\n", keywords[i]);



    for (size_t i = 0; i < array_size(keywords); i++)
        string_delete(keywords[i]);
    array_delete(keywords);
    free(complex_code_without_comments);
    free(complex_code_with_comments);
    return 0;
}