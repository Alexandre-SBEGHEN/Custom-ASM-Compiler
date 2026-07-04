/**
 * @file interpreter.c
 * @brief Implémentation du compilateur.
 * @author Alexandre SBEGHEN
 * @date 2026-06-26
 */

#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Lecture de fichier dans une string */
char* file_to_string(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        return NULL;

    // Taille du fichier
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Allouer la string
    char* str;
    if ((str = malloc(size + 1)) == NULL) {
        fclose(file);
        return NULL;
    }

    fread(str, 1, size, file);
    str[size] = '\0';

    fclose(file);
    return str;
}

/* Suppression des commentaires */
void remove_comments(char* str) {
    
}