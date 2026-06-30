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

/* Découpage d'une string en tableau de lignes */
char** string_to_array(const char* str) {
    char* copy; // Copie de str, pour strtok

    if ((copy = strdup(str)) == NULL)
        return NULL;
    const char* sep = "\n";

    // Calculer le nombre de lignes
    size_t rows = 0;
    size_t max_length = 0;
    char* token = strtok(copy, sep);
    while (token != NULL) {
        ++rows;
        if (strlen(token) > max_length)
            max_length = strlen(token);
        token = strtok(NULL, sep);
    }

    // Allouer le tableau de strings
    char** array;
    if ((array = malloc(rows * sizeof(char*))) == NULL) {
        free(copy);
        return NULL;
    }
    for (size_t i = 0; i < rows; ++i) {
        // Libérer tous les précédents si échec de l'allocation de l'actuel
        if ((array[i] = calloc(max_length + 1, sizeof(char))) == NULL) { // max_length ne compte pas \0
            for (size_t j = 0; j < i; ++j)
                free(array[j]);
            free(array);
            free(copy);
            return NULL;
        }
    }

    // On relit les token mais cette fois on les met dans le tableau
    // Nouvelle copie de str
    free(copy);
    if ((copy = strdup(str)) == NULL) {
        for (size_t i = 0; i < rows; ++i)
            free(array[i]);
        free(array);
        return NULL;
    }
    // Copie dans le tableau
    token = strtok(copy, sep);
    for (size_t i = 0; i < rows; ++i) {
        strcpy(array[i], token);
        token = strtok(NULL, sep);
    }

    free(copy);
    return array;
}

/* Nettoyage d'une ligne de code */
char* string_clean(char* str) {
    char* copy = strdup(str);
    if (copy == NULL)
        return NULL;

    if (strlen(copy) == 0)
        return copy;

    // Cropper à gauche
    size_t trim;
    size_t len = strlen(copy);
    for (trim = 0; trim < len && (copy[trim] == ' ' || copy[trim] == '\t'); ++trim);
    memmove(copy, copy + trim, strlen(copy) - trim + 1);

    // Commence par commentaire
    if (copy[0] == '#') {
        copy[0] = '\0';
        return copy;
    }

    len = strlen(copy);
    if (len == 0)
        return copy;

    // Cropper à droite (commentaires)
    for (trim = len - 1; trim > 0 && copy[trim] != '#'; --trim);
    if ((trim > 0) && !(trim >= 2 && copy[trim-2] == 'D'))
        copy[trim] = '\0';

    len = strlen(copy);
    if (len == 0)
        return copy;

    // Cropper à droite (espaces)
    for (trim = len - 1; trim > 0 && (copy[trim] == ' ' || copy[trim] == '\t'); --trim);
    copy[trim+1] = '\0';

    return copy;
}

/* Pré-tokenisation d'un tableau de lignes de codes */
char** array_to_string_tokens(const char** array) {
    return NULL;
}