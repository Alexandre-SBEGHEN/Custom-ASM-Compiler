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
 * @see string_to_keywords()
 */
void test_string_to_keywords() {
    char* complex_code_with_comments = file_to_string(
        PROJECT_ROOT "/tests/testdata/test_compiler/test_remove_comments/very_tricky_code.asm"
    );
    char* complex_code_without_comments = remove_comments(complex_code_with_comments);

    char expected_keywords[33][10] = {
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
    string* keywords = string_to_keywords(complex_code_without_comments);

    assert(keywords != NULL);
    for (size_t i = 0; i < 33; ++i) {
        assert(string_equals(keywords[i], expected_keywords[i]));
        string_delete(keywords[i]);
    }

    array_delete(keywords);
    free(complex_code_without_comments);
    free(complex_code_with_comments);
}

/**
 * @brief Test de la fonction token_create().
 *
 * @see token_create()
 */
void test_token_create() {
    Token* token = token_create(TT_INST, 4, NULL);

    assert(token != NULL);

    assert(token->type == TT_INST);
    assert(token->value == 4);
    assert(token->label == NULL);
    token_delete(token);

    token = token_create(TT_LABEL_GOTO, -1, "main");
    assert(token != NULL);

    assert(token->type == TT_LABEL_GOTO);
    assert(token->value == -1);
    assert(string_equals(token->label, "main"));

    token_delete(token);
}

/**
 * @brief Test de la fonction get_opcode_from_keyword().
 *
 * @see get_opcode_from_keyword()
 */
void test_get_opcode_from_keyword() {
    assert(get_opcode_from_keyword("LOAD") == OP_LOAD_DIRECT);
    assert(get_opcode_from_keyword("INCR") == OP_INCR);
    assert(get_opcode_from_keyword("SIXSEVEN") == OP_NOTHING);
}

/**
 * @brief Test de la fonction string_is_a_label().
 *
 * @see string_is_a_label()
 */
void test_string_is_a_label() {
    assert(string_is_a_label("etiquette"));
    assert(string_is_a_label("etiquette_"));
    assert(string_is_a_label("_etiquette"));
    assert(string_is_a_label("eti_quette"));
    assert(string_is_a_label("__eti_quette__"));
    assert(string_is_a_label("_"));
    assert(string_is_a_label("e"));
    assert(!string_is_a_label("?"));
    assert(!string_is_a_label("etiquette:"));
    assert(!string_is_a_label("_:_"));
    assert(!string_is_a_label("etiquette "));
}

/**
 * @brief Test de la fonction keyword_to_token().
 *
 * @see keyword_to_token()
 */
void test_keyword_to_token() {
    string keywords[19] = {
        string_create("etiquette:"),
        string_create(":etiquette"),
        string_create(":etiquette:"),
        string_create("etiquette::"),
        string_create(":"),
        string_create("#0"),
        string_create("#67"),
        string_create("#-104"),
        string_create("@0"),
        string_create("@69"),
        string_create("@-123"),
        string_create("LOAD"),
        string_create("STORE"),
        string_create("INCR"),
        string_create("JUMP"),
        string_create("HALT"),
        string_create("etiquette"),
        string_create("main"),
        string_create("sixseven")
    };
    Token* tokens[19];
    for (size_t i = 0; i < 19; ++i) {
        tokens[i] = keyword_to_token(keywords[i]);
        string_delete(keywords[i]);
    }
    Token expected_tokens[19] = {
        {TT_LABEL_DEF, -1, "etiquette"},
        {TT_NOTHING, -1, NULL},
        {TT_NOTHING, -1, NULL},
        {TT_NOTHING, -1, NULL},
        {TT_NOTHING, -1, NULL},
        {TT_NUMBER, 0, NULL},
        {TT_NUMBER, 67, NULL},
        {TT_NUMBER, -104, NULL},
        {TT_ADDRESS, 0, NULL},
        {TT_ADDRESS, 69, NULL},
        {TT_ADDRESS, -123, NULL},
        {TT_INST, (int32_t)OP_LOAD_DIRECT, NULL},
        {TT_INST, (int32_t)OP_STORE_TO, NULL},
        {TT_INST, (int32_t)OP_INCR, NULL},
        {TT_INST, (int32_t)OP_JUMP, NULL},
        {TT_INST, (int32_t)OP_HALT, NULL},
        {TT_LABEL_GOTO, -1, "etiquette"},
        {TT_LABEL_GOTO, -1, "main"},
        {TT_LABEL_GOTO, -1, "sixseven"}
    };

    for (size_t i = 0; i < 19; ++i) {
        assert(tokens[i] != NULL);
        assert(tokens[i]->type == expected_tokens[i].type);
        assert(tokens[i]->value == expected_tokens[i].value);
        if (expected_tokens[i].label == NULL)
            assert(tokens[i]->label == NULL);
        else
            assert(strcmp(tokens[i]->label, expected_tokens[i].label) == 0);

        token_delete(tokens[i]);
    }
}



int main() {
    test_file_to_string();
    test_remove_comments();
    test_string_to_keywords();

    test_token_create();
    test_get_opcode_from_keyword();
    test_string_is_a_label();

    test_keyword_to_token();

    return 0;
}