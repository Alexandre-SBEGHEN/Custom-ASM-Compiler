/**
 * @file main.c
 * @brief Point d'entrée du programme
 * @author Alexandre SBEGHEN
 * @date 2026-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine.h"
#include "compiler.h"
#include "interpreter.h"

int main(void) {
    char *str = "#tout premier commentaire du fichier, en debut absolu\nmain: #commentaire juste apres un label\n    LOAD #-9;\n        STORE @2;\nfor:\n        LOAD @0; INCR; STORE @0; # 0 -> 0\n#blabla\n     LOAD @2; # on charge\n        INCR;\nSTORE @2;\n\n        JZ for;HALT;\n\n# commentaire contenant une fausse instruction complete : STORE @0; LOAD #5; JZ for;\n# encore un piege : #42 ne doit PAS etre lu comme un operande ici\n\nLOAD#7;\nLOAD #0;\nLOAD#-3;\n\nSTORE @1;#collé direct sans espace\n#\n#           (commentaire avec juste des espaces après le #)\n\n	LOAD @1; 	# commentaire précédé d'une tabulation\n\nJZ main;#fin ; avec un ; dedans et un #99 aussi\n\n# derniere ligne du fichier, sans retour a la ligne final\nLOAD #123;";

    printf("%s\n————————————————————\n", str);

    //free(str);
    return 0;
}