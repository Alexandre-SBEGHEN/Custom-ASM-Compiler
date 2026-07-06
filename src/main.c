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
    char* keyword = "INCR";
    Token* token = keyword_to_token(keyword);

    if (token == NULL) {
        printf("Token is NULL\n");
    } else {
        printf("type: %d\nvalue: %d\nlabel: ", token->type, token->value);
        if (token->label == NULL)
            printf("NULL\n");
        else
            printf("%s\n", token->label);
    }


    return 0;
}