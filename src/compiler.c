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
char* remove_comments(const char* str) {
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

/* Pré-tokénisation → tableau de mots clés */
char** string_to_keywords(const char* str) {
    char* copy = strdup(str);
    if (copy == NULL)
        return NULL;

    // Préparer la séparation (token, séparateurs, nombre de lignes nécessaires)
    char* token;
    const char* sep = "\n\t ;";
    size_t rows = 0;

    // Combien de lignes à allouer ?
    token = strtok(copy, sep);
    while (token != NULL) {
        ++rows;
        token = strtok(NULL, sep);
    }
    free(copy);

    // Allocation du tableau des mots clés
    // Tableau de pointeurs
    char** array;
    if ((array = malloc(rows * sizeof(char*))) == NULL)
        return NULL;

    // Bloc de données contiguës
    const size_t max_chars_per_row = 16;
    char* data;
    if ((data = calloc(rows * max_chars_per_row, sizeof(char))) == NULL) {
        free(array);
        return NULL;
    }

    // Lier adresses et emplacements dans le bloc
    for (size_t i = 0; i < rows; ++i)
        array[i] = data + i * max_chars_per_row;

    // Affectation dans le tableau
    copy = strdup(str);
    if (copy == NULL) {
        free(data);
        free(array);
        return NULL;
    }

    token = strtok(copy, sep);
    size_t i = 0;
    while (i < rows && token != NULL) {
        strcpy(array[i], token);
        token = strtok(NULL, sep);
        ++i;
    }
    free(copy);

    return array;
}

/* Tokénisation d'un mot-clé */
Token* keyword_to_token(const char* keyword) {
    if (keyword == NULL || strlen(keyword) < 2)
        return NULL;

    if (strchr(keyword, ':') != NULL) {

    }
}