# Codes de sortie de l'interpréteur

| Code | Signification |
|-|-|
| 0 | Aucune erreur |
| 1 | Machine et/ou registre manquant(s) |
| 2 | Overflow, index de lecture en dehors du programme (`<0` OU `>= taille`) |
| 3 | Instruction inconnue |
| 4 | Accès mémoire hors des bornes (`LOAD @` / `STORE @` avec un indice invalide) |