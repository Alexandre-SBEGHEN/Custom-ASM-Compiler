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
#include "myarray.h"
#include "mystring.h"
#include "stdbool.h"

/* --- Alias --------------------------------------------------------------- */

typedef enum TokenTypeEnum TokenType;
typedef enum CompilerErrorsEnum CompilerErrors;
typedef struct TokenStruct Token;
typedef struct KeywordEntryStruct KeywordEntry;

/* --- Enums et Structs ---------------------------------------------------- */

/**
 * @brief Types qu'un token peut prendre.
 */
enum TokenTypeEnum {
    TT_NOTHING,
    TT_LABEL_DEF,
    TT_LABEL_GOTO,
    TT_NUMBER,
    TT_ADDRESS,
    TT_INST
};

/**
 * @brief Codes d'erreur du compilateur lors de
 * la compilation d'un programme.
 */
enum CompilerErrorsEnum {
    CERR_SUCCESS, /**< Aucune erreur */
    CERR_NO_TOKENS, /**< Tokens inexistants */
    CERR_UNKNOWN_INSTRUCTION, /**< Instruction inconnue */
    CERR_ORPHAN_OPERAND, /**< Opérande orphelin */
    CERR_INST_W_WRONG_OPERAND, /**< Instruction avec mauvais/sans opérande */
    CERR_JUMP_W_O_LABEL, /**< Instruction saut sans étiquette */
    CERR_ORPHAN_LABEL, /**< Label sans son instruction jump */
    CERR_LABEL_ALREADY_DEFINED, /**< Définition d'une étiquette déjà existante */
    CERR_LABEL_UNDEFINED, /**< Saut vers une étiquette inexistante */
};

/**
 * @brief Structure d'un token.
 */
struct TokenStruct {
    TokenType type; /**< Type du token */
    int32_t value; /**< Valeur du token */
    string label; /**< Libellé du token (facultatif) */
};

/**
 * @brief Association mot-clé / opcode.
 */
struct KeywordEntryStruct {
    char* keyword; /**< Mot clé */
    Opcode opcode; /**< Opcode correspondant */
};

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
 * de mémoire, penser à libérer la mémoire.
 *
 * @see string_delete()
 * @see array_delete()
 */
string* string_to_keywords(const char* str);

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
 * @param[out] token Adresse du pointeur vers la structure.
 */
void token_delete(Token* token);

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
bool string_is_a_label(const char* str);

/**
 * @brief Crée un token à partir d'un mot-clé.
 *
 * @param[in] keyword Le mot-clé à tokeniser.
 * @return Pointeur vers la structure allouée ou NULL en cas
 * d'échec d'allocation.
 *
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire.
 *
 * @see token_delete()
 */
Token* keyword_to_token(const string keyword);

/**
 * @brief Crée une série de tokens à partir de mots-clé.
 *
 * @param[in] keywords Pointeur vers le tableau des mots-clé.
 * @return Pointeur vers le tableau de tokens.
 *
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire.
 *
 * @see token_delete()
 */
Token** keywords_to_tokens(string* keywords);

/**
 * @brief Lève les ambiguïtés entre les tokens qui peuvent
 * avoir plusieurs significations en fonction du contexte.
 *
 * Le token LOAD par exemple, peut être une instruction
 * de registre ou de mémoire s'il est suivi de '#...'
 * ou '@...'. Cette fonction a donc pour but d'examiner
 * le contexte des tokens pour effectuer les changements
 * nécessaires.
 *
 * @param[in, out] tokens Pointeur vers le tableau de tokens.
 */
void tokens_disambiguate(Token** tokens);

/**
 * Cette fonction a pour but de vérifier si les tokens ont du
 * sens, donc si le code écrit en amont est correct.
 *
 * Voici les critères d'invalidité :
 * - Opérande (TT_NUMBER, TT_ADDRESS) non précédé d'une instruction
 *   capable de le recevoir (LOAD pour #<valeur> ou @<adresse>,
 *   STORE pour @<adresse> uniquement).
 * - LOAD ou STORE non suivi immédiatement d'un opérande valide
 *   pour cette instruction.
 * - JUMP ou JZ non suivi immédiatement d'un token TT_LABEL_GOTO.
 * - TT_LABEL_DEF dont l'étiquette est déjà définie ailleurs dans
 *   le programme.
 * - TT_LABEL_GOTO référençant une étiquette jamais définie dans
 *   le programme (l'ordre d'apparition n'importe pas, les sauts
 *   en avant sont autorisés).
 *
 * @param[in] tokens Pointeur vers le tableau de tokens.
 * @return Validité des tokens.
 */
CompilerErrors tokens_check_validity(Token** tokens);

/**
 * Crée un programme compilé, compréhensible par l'interpréteur,
 * à partir de tokens.
 *
 * @param[in] tokens Pointeur vers le tableau de tokens.
 *
 * @return Pointeur vers le programme compilé.
 *
 * @note Cette fonction suppose que les tokens sont déjà
 * valides, et ont déjà été passés dans la fonction
 * tokens_check_validity(). Il pourrait y avoir des
 * comportements non définis / crash si cela n'a pas
 * été fait au préalable.
 *
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire.
 *
 * @see tokens_check_validity()
 * @see program_delete()
 */
Program* tokens_parse(Token** tokens);

/**
 * @brief Cette fonction permet de savoir si un programme
 * est compilable, grâce à un code d'erreur (ou de réussite).
 *
 * Permet également d'identifié le ou les problèmes rencontrés.
 *
 * @param[in] filename Chemin vers le fichier.
 * @return Code d'erreur (ou de réussite).
 *
 * @note Si plusieurs problèmes ont lieu, seul celui rencontré
 * en premier sera renvoyé.
 */
CompilerErrors program_is_compilable(const char* filename);

/**
 * @brief Compile le code issu d'un fichier ASM.
 *
 * Effectue toute la chaîne de compilation et se charge
 * de libérer la mémoire allouée durant le cycle, pour les strings
 * ou encore les tokens.
 *
 * @param[in] filename Chemin vers le fichier.
 * @return Pointeur vers le programme compilé, NULL en cas d'échec
 * de compilation.
 *
 * @note Un appel à cette fonction présuppose que le programme
 * est commpilable, et a été vérifié en amont.
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire.
 *
 * @see program_is_compilable()
 * @see program_delete()
 */
Program* program_compile(const char* filename);

#endif