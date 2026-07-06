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
#include "interpreter.h"

/* --- Alias --------------------------------------------------------------- */

typedef enum TokenTypeEnum TokenType;
typedef struct TokenStruct Token;
typedef struct KeywordEntryStruct KeywordEntry;

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
    char* label;
    Token* next;
};

struct KeywordEntryStruct {
    char* keyword;
    Opcode opcode;
};

/* --- Constantes ---------------------------------------------------------- */

extern const KeywordEntry KEYWORDS[];
extern const size_t KEYWORDS_COUNT;

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
 * @brief Crée dynamiquement une structure Token.
 *
 * @param[in] type Type du token (enum TokenType).
 * @param[in] value Valeur du token.
 * @param[in] label (OPTIONNEL) Label du token, mettre NULL si aucun label
 * @return Pointeur vers la structure allouée ou NULL en cas
 * d'échec d'allocation.
 *
 * @note Penser à libérer la mémoire après utilisation.
 * @see token_delete()
 */
Token* token_create(TokenType type, int32_t value, const char* label);

/**
 * @brief Libération de mémoire d'une structure Token.
 *
 * Utilisation du double pointeur en paramètre pour
 * mettre automatiquement sa valeur à NULL.
 *
 * @param[in, out] token Adresse du pointeur vers la structure.
 */
void token_delete(Token** token);

/**
 * @brief Renvoie l'Opcode associé à un mot clé.
 *
 * Exemples :
 * - INCR -> OP_INCR
 * - JUMP -> OP_JUMP
 * - SIXSEVEN -> OP_NOTHING
 *
 * @param[in] keyword Le mot-clé.
 * @return Opcode correspondant, OP_NOTHING si aucune correspondance.
 */
Opcode get_opcode_from_keyword(const char* keyword);

/**
 * @brief Vérifie si la string en entrée est une étiquette,
 * i. e. composée de lettres et '_' uniquement.
 *
 * @param[in] str String à vérifier
 * @return Valeur booléenne de la vérification.
 */
int string_is_a_label(const char* str);

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