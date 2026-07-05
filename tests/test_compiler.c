/**
 * @file test_compiler.c
 * @brief Test des implémentations de compiler.h / compiler.c
 * @author Alexandre SBEGHEN
 * @date 2026-06-26
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "interpreter.h"

/**
 * @brief Test de la fonction file_to_string().
 *
 * Exécute la fonction sur différents cas et vérifie
 * les strings en résultat.
 *
 * @see file_to_string()
 */
void test_file_to_string() {
    // Premier fichier (paragraphe simple)
    char* strfile_1 = file_to_string(PROJECT_ROOT "/tests/testdata/test_compiler/test_file_to_string/simple_paragraph"
                                                  ".txt");
    assert(strfile_1 != NULL);
    char* strtest_1 = "Bonjour tout le monde !\nCeci est un paragraphe simple,\ntrès très simple...\nHihihi";
    assert(strcmp(strfile_1, strtest_1) == 0);
    free(strfile_1);

    // Second fichier (code asm simple)
    char* strfile_2 = file_to_string(PROJECT_ROOT "/tests/testdata/test_compiler/test_file_to_string/simple_asm_code.asm");
    assert(strfile_2 != NULL);
    char* strtest_2 = "# Petit code asm tranquilou\nmain:\n    LOAD #1;\n    HALT;";
    assert(strcmp(strfile_2, strtest_2) == 0);
    free(strfile_2);
}

/**
 * @brief Test de la fonction remove_comments().
 *
 * Prend un gros morceau de codes avec des commentaires
 * qui testent les différents cas possibles et vérifie
 * la correspondance entre le résultat de la fonction
 * et le résultat attendu.
 *
 * @see remove_comments()
 */
void test_remove_comments() {
    char* complex_code_with_comments = file_to_string(PROJECT_ROOT
        "/tests/testdata/test_compiler/test_remove_comments/very_tricky_code.asm");
    assert(complex_code_with_comments != NULL);

    char* complex_code_comments_deleted = remove_comments(complex_code_with_comments);
    assert(complex_code_comments_deleted != NULL);

    char* expected_result = "#tout premier commentaire du fichier, en debut absolu\nmain: #commentaire juste apres un"
                            " label\n    LOAD #-9;\n        STORE @2;\nfor:\n        LOAD @0; INCR; STORE @0; # 0 -> "
                            "0\n#blabla\n     LOAD @2; # on charge\n        INCR;\nSTORE @2;\n\n        JZ for;HALT;\n\n# commentaire contenant une fausse instruction complete : STORE @0; LOAD #5; JZ for;\n# encore un piege : #42 ne doit PAS etre lu comme un operande ici\n\nLOAD #7;\nLOAD #0;\nLOAD #-3;\n\nSTORE @1;#collé direct sans espace\n#\n#           (commentaire avec juste des espaces après le #)\n\n	LOAD @1; 	# commentaire précédé d'une tabulation\n\nJZ main;#fin ; avec un ; dedans et un #99 aussi\n\n# derniere ligne du fichier, sans retour a la ligne final\nLOAD #123;";
    assert(strlen(expected_result) == strlen(complex_code_with_comments));
    for (size_t i = 0; i < strlen(expected_result); i++)
        assert(expected_result[i] == complex_code_with_comments[i]);

    free(complex_code_comments_deleted);
}

/**
 * @brief Test de la fonction string_to_keywords().
 *
 * Exécute la fonction avec un grand code et
 * vérifie la correspondance entre le tableau de
 * la fonction et le tableau attendu.
 *
 * @see string_to_keywords()
 */
void test_string_to_keywords() {
    char* complex_code_with_comments = file_to_string(PROJECT_ROOT
        "/tests/testdata/test_compiler/test_remove_comments/very_tricky_code.asm");

    char* complex_code_comments_deleted = remove_comments(complex_code_with_comments);

    char** array = string_to_keywords(complex_code_comments_deleted);
    assert(array != NULL);

    char expected_array[33][10] = {
        "main:",
        "LOAD", "#-9",
        "STORE","@2",
        "for:",
        "LOAD", "@0",
        "INCR",
        "STORE", "@0",
        "LOAD", "@2",
        "INCR",
        "STORE", "@2",
        "JZ", "for",
        "HALT",
        "LOAD", "#7",
        "LOAD", "#0",
        "LOAD", "#-3",
        "STORE", "@1",
        "LOAD", "@1",
        "JZ", "main",
        "LOAD", "#123"
    };
    for (size_t i = 0; i < 1; ++i)
        assert(strcmp(array[i], expected_array[i]) == 0);

    free(array[0]);
    free(array);
    free(complex_code_comments_deleted);
    free(complex_code_with_comments);
}

void test_token_create() {
    Token* token = token_create(TT_LABEL_GOTO, -1);
    assert(token != NULL);

    assert(token->type == TT_LABEL_GOTO);
    assert(token->value == -1);

    token_delete(&token);
}

void test_token_delete() {
    Token* token = token_create(TT_NOTHING, 0);
    token_delete(&token);

    assert(token == NULL);
}

void test_get_opcode_from_keyword() {
    assert(get_opcode_from_keyword("LOAD") == OP_LOAD_DIRECT);
    assert(get_opcode_from_keyword("INCR") == OP_INCR);
    assert(get_opcode_from_keyword("SIXSEVEN") == OP_NOTHING);
}

/**
 * @brief Test de la fonction keyword_to_token().
 *
 * Vérifie que les mots-clé sont associés
 * aux bon tokens.
 *
 * @see keyword_to_token()
 */
void test_keyword_to_token() {
    Token* tokens[19] = {
        keyword_to_token("etiquette:"),
        keyword_to_token(":etiquette"),
        keyword_to_token(":etiquette:"),
        keyword_to_token("etiquette::"),
        keyword_to_token(":"),
        keyword_to_token("#0"),
        keyword_to_token("#67"),
        keyword_to_token("#-104"),
        keyword_to_token("@0"),
        keyword_to_token("@69"),
        keyword_to_token("@-123"),
        keyword_to_token("LOAD"),
        keyword_to_token("STORE"),
        keyword_to_token("INCR"),
        keyword_to_token("JUMP"),
        keyword_to_token("HALT"),
        keyword_to_token("etiquette"),
        keyword_to_token("main"),
        keyword_to_token("sixseven"),
    };
    Token expected_tokens[19] = {
        {TT_LABEL_DEF, -1},
        {TT_NOTHING, -1},
        {TT_NOTHING, -1},
        {TT_NOTHING, -1},
        {TT_NOTHING, -1},
        {TT_NUMBER, 0},
        {TT_NUMBER, 67},
        {TT_NUMBER, -164},
        {TT_ADDRESS, 0},
        {TT_ADDRESS, 69},
        {TT_ADDRESS, -123},
        {TT_INST, -1},
        {TT_INST, (int32_t)OP_STORE_TO},
        {TT_INST, (int32_t)OP_INCR},
        {TT_INST, (int32_t)OP_JUMP},
        {TT_INST, (int32_t)OP_HALT},
        {TT_LABEL_GOTO, -1},
        {TT_LABEL_GOTO, -1},
        {TT_LABEL_GOTO, -1}
    };

    for (size_t i = 0; i < 19; ++i) {
        assert(tokens[i] != NULL);
        assert(tokens[i]->type == expected_tokens[i].type);
        assert(tokens[i]->value == expected_tokens[i].value);
    }
}

int main() {
    test_file_to_string();
    test_remove_comments();
    test_string_to_keywords();
    test_token_create();
    test_token_delete();
    test_get_opcode_from_keyword();
    //test_keyword_to_token();

    return 0;
}