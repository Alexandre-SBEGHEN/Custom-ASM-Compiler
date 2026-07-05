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

const KeywordEntry KEYWORDS[] = {
    {"LOAD", OP_LOAD_DIRECT},
    {"STORE", OP_STORE_TO},
    {"INCR", OP_INCR},
    {"DECR", OP_DECR},
    {"JUMP", OP_JUMP},
    {"JZ", OP_JZ},
    {"HALT", OP_HALT}
};
const size_t KEYWORDS_COUNT = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);

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

/* Création dynamique d'une structure Token */
Token* token_create(const TokenType type, const int32_t value) {
    Token* token;

    // Allocation, return NULL si échec
    if ((token = malloc(sizeof(Token))) == NULL)
        return NULL;

    // Assignation des champs
    token->type = type;
    token->value = value;

    return token;
}

/* Libération de mémoire d'une structure Token */
void token_delete(Token** token) {
    if (token == NULL || *token == NULL)
        return;

    free(*token);
    (*token) = NULL;
}

/* Mot-clé -> Opcode */
Opcode get_opcode_from_keyword(const char* keyword) {
    size_t keyword_index = 0;
    while (keyword_index < KEYWORDS_COUNT && strcmp(keyword, KEYWORDS[keyword_index].keyword))
        ++keyword_index;

    if (keyword_index >= KEYWORDS_COUNT)
        return OP_NOTHING;

    return KEYWORDS[keyword_index].opcode;
}

/* Vérifier si une string correspond à une étiquette */
int string_is_a_label(const char* str) {
    return 0;
}

/* Tokénisation d'un mot-clé */
Token* keyword_to_token(const char* keyword) {
    size_t len = strlen(keyword);
    if (keyword == NULL || len < 2)
        return NULL;

    Token* token;
    char* colon_pos = strchr(keyword, ':');
    Opcode opcode;
    if (colon_pos != NULL && colon_pos == keyword + len - 1) {
        // ':' est unique et en fin de string
        token = token_create(TT_LABEL_DEF, -1);
    } else if (keyword[0] == '#') {
        // Nombre entier
        token = token_create(TT_NUMBER, (int32_t)atoi(keyword + 1));
    } else if (keyword[0] == '@') {
        // Adresse mémoire
        token = token_create(TT_ADDRESS, (int32_t)atoi(keyword + 1));
    } else if ((opcode = get_opcode_from_keyword(keyword)) != OP_NOTHING) {
        // Instruction
        token = token_create(opcode, 0);
    } else if (1) {
        // Etiquette détectée
        token = token_create(TT_LABEL_GOTO, -1);
    } else {
        // Mot-clé invalide
        token = token_create(TT_NOTHING, -1);
    }

    return token;
}