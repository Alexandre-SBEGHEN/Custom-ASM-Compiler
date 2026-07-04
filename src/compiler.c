/**
 * @file interpreter.c
 * @brief Implémentation du compilateur.
 * @author Alexandre SBEGHEN
 * @date 2026-06-26
 */

#include "compiler.h"

#include <ctype.h>
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
char* code_without_comments(const char* str) {
    char* copy = strdup(str);
    if (copy == NULL)
        return NULL;

    int in_comment = 0;
    int writing_code = 0;

    const size_t len = strlen(copy);
    for (size_t i = 0; i < len; ++i) {
        const char c = copy[i];

        // Remplacement commentaire par ' '
        if (in_comment) {
            if (c == '\n')
                in_comment = 0;
            else
                copy[i] = ' ';
            continue;
        }

        // Commentaire ou instruction ?
        if (c == '#') {
            if (!writing_code) { // Si on n'est pas en train d'écrire du code, c'est un commentaire
                in_comment = 1;
                copy[i] = ' ';
            }
        } else if (c == ';' || c == ':') { // Fin de l'instruction, on n'écrit plus de code
            writing_code = 0;
        } else if (!isspace((unsigned char)c)) { // c est un autre caractère, on écrit donc du code
            writing_code = 1;
        }
    }

    return copy;
}

/* Pré-tokénisation -> tableau de mots clés */
char** array_of_isolated_keywords(const char* str) {
    return NULL;
}