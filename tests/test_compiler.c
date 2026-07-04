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
    char* complex_code_with_comments = "#tout premier commentaire du fichier, en debut absolu\nmain: #commentaire juste apres un label\n    LOAD #-9;\n        STORE @2;\nfor:\n        LOAD @0; INCR; STORE @0; # 0 -> 0\n#blabla\n     LOAD @2; # on charge\n        INCR;\nSTORE @2;\n\n        JZ for;HALT;\n\n# commentaire contenant une fausse instruction complete : STORE @0; LOAD #5; JZ for;\n# encore un piege : #42 ne doit PAS etre lu comme un operande ici\n\nLOAD#7;\nLOAD #0;\nLOAD#-3;\n\nSTORE @1;#collé direct sans espace\n#\n#           (commentaire avec juste des espaces après le #)\n\n	LOAD @1; 	# commentaire précédé d'une tabulation\n\nJZ main;#fin ; avec un ; dedans et un #99 aussi\n\n# derniere ligne du fichier, sans retour a la ligne final\nLOAD #123;";
    //remove_comments(complex_code_with_comments);

    char* expected_result = "";

    assert(strlen(expected_result) == strlen(complex_code_with_comments));
    for (size_t i = 0; i < strlen(expected_result); i++)
        assert(expected_result[i] == complex_code_with_comments[i]);
}

int main() {
    test_file_to_string();
    test_remove_comments();

    return 0;
}