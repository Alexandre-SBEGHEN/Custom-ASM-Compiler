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
    Token* token = token_create(TT_INST, 4, NULL);

    printf("type: %d\n", token->type);
    printf("type: %d\n", token->value);
    printf("type: %s\n", token->label);

    token_delete(token);
    return 0;
}