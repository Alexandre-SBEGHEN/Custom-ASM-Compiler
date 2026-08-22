# Codes de sortie du compilateur

| Code | Signification |
|-|-|
| 0 | Aucune erreur |
| 1 | Tokens inexistants |
| 2 | Instruction inconnue |
| 3 | Opérande orphelin (ex: `#?` ou `@?` sans leur instruction `LOAD` ou `STORE`) |
| 4 | Instruction avec mauvais ou sans opérande (ex: un `LOAD` seul ou `STORE #0`) |
| 5 | Instruction de saut sans étiquette (ex: un `JUMP` seul) |
| 6 | Etiquette sans instruction de saut lui précédant |
| 7 | Définition d'une étiquette déjà existante |
| 8 | Saut vers une étiquette inexistante |