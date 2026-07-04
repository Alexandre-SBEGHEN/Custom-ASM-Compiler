/**
 * @file compiler.h
 * @brief Compilateur de code ASM
 *
 * Lecture de fichiers, nettoyage des
 * lignes de code, tokenisation,
 * puis parsing.
 *
 * Ça va pas être de la rigolade
 *
 * @author Alexandre SBEGHEN
 * @date 2026-06-26
 */

#ifndef COMPILER_H
#define COMPILER_H

#include <stdint.h>

/* --- Alias --------------------------------------------------------------- */

typedef enum TokenTypeEnum TokenType;
typedef struct TokenStruct Token;

/* --- Enums et Structs ---------------------------------------------------- */

enum TokenTypeEnum {
    TT_NOTHING,
    TT_LABEL_DEF,
    TT_LABEL_GOTO,
    TT_NUMBER,
    TT_ADDRESS,
    TT_INST
};

struct TokenStruct {
    TokenType type;
    int32_t value;
};

/* --- Constantes ---------------------------------------------------------- */



/* --- Fonctions ----------------------------------------------------------- */

/**
 * @brief Récupère le contenu d'un fichier dans une string.
 *
 * @param[in] filename Chemin vers le fichier.
 * @return Pointeur vers la string, NULL en cas d'échec.
 *
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire avec free().
 */
char* file_to_string(const char* filename);

/**
 * @brief Retire les commentaires d'un code.
 *
 * Remplace les commentaires (# jusqu'à fin de ligne)
 * par des espaces.
 * N'affecte les # qui servent d'opérande (ex: LOAD #0).
 *
 * @param[in] str Code sur lequel effectuer les modifications
 * @return Pointeur vers la string, NULL en cas d'échec.
 *
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire avec free().
 */
char* remove_comments(const char* str);

/**
 * @brief Transforme un code clean (sans commentaires) en liste de mots clés.
 *
 * C'est l'étape juste avant le lexer, c'est-à-dire la tokénisation.
 * Ici, on prépare le lexer en listant tous les mots clés (tokens)
 * qui apparaissent dans le code, sans les ';'.
 *
 * @param[in] str Code clean sur lequel on veut effectuer la pré-tokénisation
 * @return Pointeur vers le tableau de strings, NULL en cas d'échec
 *
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire avec free(). À
 * effectuer sur array[0] ET array.
 */
char** string_to_keywords(const char* str);

/**
 * @brief Crée un token à partir d'un mot-clé.
 *
 * @param[in] keyword Le mot-clé à tokeniser.
 * @return Pointeur vers la structure allouée ou NULL en cas
 * d'échec d'allocation.
 *
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire avec free().
 */
Token* keyword_to_token(const char* keyword);

#endif