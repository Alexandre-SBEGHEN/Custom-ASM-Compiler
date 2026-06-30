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
    char** arr = string_to_array(str);
    char** arr_clean = string_to_array(str);

    // printf("|%s|\n————————————————————\n", str);

    /*
    for (int i = 0; i < 15; i++)
            printf("|%s|\n", arr[i]);
    printf("————————————————————\n");
    */



    for (int i = 0; i < 15; i++) {
        char* temp = string_clean(arr_clean[i]);
        free(arr_clean[i]);
        arr_clean[i] = temp;
        printf("%s\n", arr_clean[i]);
    }


    free(arr_clean);
    free(arr);
    free(str);
    return 0;
}