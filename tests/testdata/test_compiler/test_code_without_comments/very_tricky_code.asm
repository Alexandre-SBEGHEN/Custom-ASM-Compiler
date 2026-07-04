#tout premier commentaire du fichier, en debut absolu
main: #commentaire juste apres un label
    LOAD #-9;
        STORE @2;
for:
        LOAD @0; INCR; STORE @0; # 0 -> 0
#blabla
     LOAD @2; # on charge
        INCR;
STORE @2;

        JZ for;HALT;

# commentaire contenant une fausse instruction complete : STORE @0; LOAD #5; JZ for;
# encore un piege : #42 ne doit PAS etre lu comme un operande ici

LOAD #7;
LOAD #0;
LOAD #-3;

STORE @1;#collé direct sans espace
#
#           (commentaire avec juste des espaces après le #)

	LOAD @1; 	# commentaire précédé d'une tabulation

JZ main;#fin ; avec un ; dedans et un #99 aussi

# derniere ligne du fichier, sans retour a la ligne final
LOAD #123;