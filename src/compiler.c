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
#include "myarray.h"
#include "mystring.h"
#include "stdbool.h"

static const KeywordEntry KEYWORDS[] = {
    {"LOAD", OP_LOAD_DIRECT},
    {"STORE", OP_STORE_TO},
    {"INCR", OP_INCR},
    {"DECR", OP_DECR},
    {"JUMP", OP_JUMP},
    {"JZ", OP_JZ},
    {"HALT", OP_HALT}
};
static const size_t KEYWORDS_COUNT = sizeof(KEYWORDS) / sizeof(KeywordEntry);

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

    bool in_comment = false;
    bool reading_code = false;

    const size_t len = strlen(copy);
    for (size_t i = 0; i < len; ++i) {
        const char c = copy[i];

        // Remplacement commentaire par ' '
        if (in_comment) {
            if (c == '\n')
                in_comment = false;
            else
                copy[i] = ' ';
            continue;
        }

        // Commentaire ou instruction ?
        if (c == '#') {
            if (!reading_code) { // Si on n'est pas en train de lire du code, c'est un commentaire
                in_comment = true;
                copy[i] = ' ';
            }
        } else if (c == ';' || c == ':') { // Fin de l'instruction, on ne lit plus de code
            reading_code = false;
        } else if (!isspace((unsigned char)c)) { // C'est un autre caractère, on lit donc du code
            reading_code = true;
        }
    }

    return copy;
}

/* Pré-tokénisation → tableau de mots clés */
string* string_to_keywords(const char* str) {
    char* copy = strdup(str);
    if (copy == NULL)
        return NULL;

    char* token;
    const char* sep = "\n\t ;";

    string* keywords = array_create(string);
    if (keywords == NULL) {
        free(copy);
        return NULL;
    }

    token = strtok(copy, sep);
    while (token != NULL) {
        array_push(keywords, string_create(token));
        token = strtok(NULL, sep);
    }
    free(copy);

    return keywords;
}

/* Création dynamique d'une structure Token */
Token* token_create(const TokenType type, const int32_t value, const char* label) {
    Token* token;

    // Allocation, return NULL si échec
    if ((token = malloc(sizeof(Token))) == NULL)
        return NULL;

    // Allocation du label
    if (label == NULL) {
        token->label = NULL;
    } else {
        if ((token->label = string_create(label)) == NULL) {
            free(token);
            return NULL;
        }
    }

    // Assignation des champs
    token->type = type;
    token->value = value;

    return token;
}

/* Libération de mémoire d'une structure Token */
void token_delete(Token* token) {
    if (token == NULL)
        return;

    if (token->label != NULL)
        string_delete(token->label);

    free(token);
}

/* Mot-clé -> Opcode */
Opcode get_opcode_from_keyword(const char* keyword) {
    size_t keyword_index = 0;
    while (keyword_index < KEYWORDS_COUNT && !string_equals(keyword, KEYWORDS[keyword_index].keyword))
        ++keyword_index;

    if (keyword_index >= KEYWORDS_COUNT)
        return OP_NOTHING;

    return KEYWORDS[keyword_index].opcode;
}

/* Vérifier si une string correspond à une étiquette */
bool string_is_a_label(const char* str) {
    for (size_t i = 0; str[i] != '\0'; ++i)
        if (!isalpha(str[i]) && str[i] != '_')
            return false;

    return true;
}

/* Tokénisation d'un mot-clé */
Token* keyword_to_token(const string keyword) {
    const size_t len = string_length(keyword);

    if (keyword == NULL || len < 2)
        return token_create(TT_NOTHING, -1, NULL);

    Token* token;
    char* colon_pos = strchr(keyword, ':');
    Opcode opcode;

    if (colon_pos != NULL && colon_pos == keyword + len - 1) {
        // ':' est unique et en fin de string
        token = token_create(TT_LABEL_DEF, -1, keyword);
        string_crop(token->label, 0, 1);
        //token->label[strlen(token->label) - 1] = '\0';
    } else if (keyword[0] == '#') {
        // Nombre entier
        token = token_create(TT_NUMBER, (int32_t)atoi(keyword + 1), NULL);
    } else if (keyword[0] == '@') {
        // Adresse mémoire
        token = token_create(TT_ADDRESS, (int32_t)atoi(keyword + 1), NULL);
    } else if ((opcode = get_opcode_from_keyword(keyword)) != OP_NOTHING) {
        // Instruction
        token = token_create(TT_INST, (int32_t)opcode, NULL);
    } else if (string_is_a_label(keyword)) {
        // Etiquette détectée
        token = token_create(TT_LABEL_GOTO, -1, keyword);
    } else {
        // Mot-clé invalide
        token = token_create(TT_NOTHING, -1, NULL);
    }

    return token;
}

/* Tokénisation d'un tableau de mot clés */
Token** keywords_to_tokens(string* keywords) {
    Token** tokens = array_create(Token*);
    if (tokens == NULL)
        return NULL;

    for (size_t i = 0; i < array_size(keywords); ++i) {
        const string keyword = keywords[i];
        Token* token = keyword_to_token(keyword);
        array_push(tokens, token);
    }

    return tokens;
}

/* Ajustement des tokens */
void tokens_disambiguate(Token** tokens) {
    for (size_t i = 0; i < array_size(tokens) - 1; ++i) {
        Token* token = tokens[i];
        Token* next = tokens[i + 1];

        if (token->type != TT_INST || token->value != (int32_t)OP_LOAD_DIRECT)
            continue;

        if (next->type == TT_ADDRESS)
            token->value = (int32_t)OP_LOAD_FROM;
    }
}

/* Validité des tokens */
bool tokens_check_validity(Token** tokens) {
    const size_t tokens_len = array_size(tokens);

    string* all_labels = array_create(string);
    for (size_t i = 0; i < tokens_len; ++i)
        if (tokens[i]->type == TT_LABEL_DEF)
            array_push(all_labels, string_create(tokens[i]->label));
    size_t all_labels_len = array_size(all_labels);

    string* declared_labels = array_create(string);

    bool tokens_are_valid = true;

    for (size_t i = 0; i < tokens_len; ++i) {
        size_t declared_labels_len = array_size(declared_labels);
        Token* previous = (i > 0) ? tokens[i-1] : NULL;
        Token* token = tokens[i];
        Token* next = (i < tokens_len - 1) ? tokens[i+1] : NULL;

        // Critère 1 : opérande orphelin
        bool is_number = token->type == TT_NUMBER;
        bool is_address = token->type == TT_ADDRESS;

        bool previous_is_load = previous != NULL &&
            previous->type == TT_INST && previous->value == (int32_t)OP_LOAD_DIRECT;
        bool previous_is_load_or_store = previous != NULL && previous->type == TT_INST &&
            (previous->value == (Opcode)OP_LOAD_FROM || previous->value == (Opcode)OP_STORE_TO);

        bool is_orphan = (is_number && !previous_is_load) || (is_address && !previous_is_load_or_store);

        if (is_orphan) {
            tokens_are_valid = false;
            break;
        }

        // Critère 2 : LOAD/STORE sans opérande ou mauvais opérande
        bool is_load = token->type == TT_INST &&
            (token->value == (int32_t)OP_LOAD_DIRECT || token->value == (int32_t)OP_LOAD_FROM);
        bool is_store = token->type == TT_INST && token->value == (int32_t)OP_STORE_TO;
        bool next_is_number = (next != NULL && next->type == TT_NUMBER);
        bool next_is_address = (next != NULL && next->type == TT_ADDRESS);

        bool wrong_or_no_operand = (is_load && !next_is_number && !next_is_address) ||
            (is_store && !next_is_address);

        if (wrong_or_no_operand) {
            tokens_are_valid = false;
            break;
        }

        // Critère 3: JUMP/JZ sans étiquette en successeur
        bool is_jump = token->type == TT_INST &&
            (token->value == (int32_t)OP_JUMP || token->value == (int32_t)OP_JZ);
        bool next_is_label = (next != NULL && next->type == TT_LABEL_GOTO);

        bool jump_missing_label = is_jump && !next_is_label;

        if (jump_missing_label) {
            tokens_are_valid = false;
            break;
        }

        // Critère 4 : redéfinition d'une étiquette déjà existante
        bool is_label_def = token->type == TT_LABEL_DEF;
        bool label_already_exists = false;
        if (declared_labels_len > 0 && token->label != NULL)
            for (size_t j = 0; j < declared_labels_len; ++j)
                if (string_equals(declared_labels[j], token->label)) {
                    label_already_exists = true;
                    break;
                }

        bool declaring_existing_label = is_label_def && label_already_exists;

        if (declaring_existing_label) {
            tokens_are_valid = false;
            break;
        } else if (is_label_def) {
            array_push(declared_labels, string_create(token->label));
        }

        // Critère 5 : saut vers une étiquette jamais déclarée
        bool is_label_goto = token->type == TT_LABEL_GOTO;
        bool label_does_not_exist = true;
        if (all_labels_len > 0 && token->label != NULL)
            for (size_t j = 0; j < all_labels_len; ++j)
                if (string_equals(all_labels[j], token->label)) {
                    label_does_not_exist = false;
                    break;
                }

        bool jump_to_nonexisting_label = is_label_goto && label_does_not_exist;

        if (jump_to_nonexisting_label) {
            tokens_are_valid = false;
            break;
        }
    }

    // Libérer la mémoire
    for (size_t i = 0; i < array_size(declared_labels); ++i)
        string_delete(declared_labels[i]);
    for (size_t i = 0; i < array_size(all_labels); ++i)
        string_delete(all_labels[i]);
    array_delete(declared_labels);
    array_delete(all_labels);

    return tokens_are_valid;
}

/* Parsing des tokens */
Program* tokens_parse(Token** tokens) {
    if (tokens == NULL)
        return NULL;

    Program* prog = program_create();

    // Se souvenir des étiquettes à l'instruction N
    size_t current_inst_index = 0;
    typedef struct {
        string label;
        size_t inst_index;
    } Pair;
    Pair* pairs = array_create(Pair);
    for (size_t i = 0; i < array_size(tokens); ++i) {
        Token* t = tokens[i];
        if (t->type == TT_LABEL_DEF) {
            array_push(pairs, ((Pair){t->label, current_inst_index}));
        } else if (t->type == TT_INST) {
            ++current_inst_index;
        }
    }

    // Ecrire les instructions
    Instruction* current_inst = NULL;
    for (size_t i = 0; i < array_size(tokens); ++i) {
        Token* t = tokens[i];

        switch (t->type) {
            case TT_LABEL_GOTO:
                for (size_t j = 0; j < array_size(pairs); ++j) {
                    Pair pair = pairs[j];
                    if (string_equals(t->label, pair.label)) {
                        current_inst->arg = (int32_t)pair.inst_index;
                        break;
                    }
                }
                break;
            case TT_NUMBER:
            case TT_ADDRESS:
                current_inst->arg = t->value;
                break;
            case TT_INST:
                array_push(prog->inst, ((Instruction){(int32_t)t->value, 0}));
                current_inst = &prog->inst[array_size(prog->inst) - 1];
                break;
            default:
                break;
        }
    }

    array_delete(pairs);

    return prog;
}
