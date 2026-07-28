# Assembly compiler & interpreter for RAM Machine

## Ma première application du TDD

Pour ce projet, j'ai voulu mettre en pratique la méthode du TDD (Test-Driven Development), apprise durant ma première
année de BUT informatique. Au départ, j'étais plutôt sceptique : je voyais surtout ça comme une contrainte
supplémentaire, qui allait me ralentir dans le développement plutôt que m'aider.

En pratique, ça s'est avéré tout l'inverse. Écrire les tests avant (ou en même temps) que le code m'a permis de
savoir avec certitude si mes fonctions fonctionnaient comme prévu, sans avoir à écrire des dizaines de lignes de
code de vérification dans le `main. Dès qu'un test passe, je peux directement passer à la suite, avec la garantie
que ce que j'ai déjà écrit est fiable.

Cette approche s'est révélée particulièrement utile dans un projet comme celui-ci, où le compilateur et
l'interpréteur reposent sur une succession d'étapes (lecture, tokenisation, parsing, exécution). Pouvoir tester
chaque brique indépendamment évite de devoir tout redéboguer à la main à chaque modification.

## Contexte

Durant mon premier TP d'assembleur, notre professeur nous a introduit à un langage assembleur rudimentaire avant de
nous faire réaliser des exercices pratiques. Le problème : il nous était impossible d'exécuter notre code pour
vérifier son exactitude. J'ai donc décidé de créer un programme capable de lire et d'exécuter du code écrit dans cet
assembleur.

J'en ai également profité pour l'implémenter entièrement en C, afin de consolider mes compétences dans ce langage et
d'apprendre la manipulation de fichiers.

## Machine Ram

C'est une machine équivalente à une machine de Turing, et issue du modèle de Von Neumann. Elle est munie d’une
mémoire infinie, d’un registre unique, et d’une unité arithmétique et logique capable d’incrémenter ou décrémenter
un entier, ou de le comparer à zéro.

<p align="center">
    <img src="assets/images/schema_machine_ram.png" alt="Schéma de la machine RAM" width="350">
</p>

Cette machine est équipée du jeu d'instructions suivant :

| Instruction | Description |
|-|-|
| `LOAD #<valeur numérique>` | Chargement direct du registre |
| `LOAD @<adresse mémoire>` | Chargement du registre depuis la mémoire |
| `STORE @<adresse mémoire>` | Rangement du registre vers la mémoire |
| `INCR` | Incrémentation du registre |
| `DECR` | Décrémentation du registre |
| `JUMP <étiquette>` | Saut inconditionnel à l'étiquette |
| `JZ <étiquette>` | Saut à l'étiquette si (registre ≤ 0) |
| `HALT` | Arrêt du programme |

## Compilateur

La compilation est une étape très importante, et sûrement celle qui m'a posé le plus de
difficultés, car j'ai dû me familiariser avec des termes comme `token` ou `parsing`, pour
être sûr que ce que je faisais avait du sens.

### Fichier d'entrée → lexer

La première étape consiste à récupérer le contenu d'un fichier sous forme de string.

Il faut ensuite se débarrasser des commentaires (déclarés avec `#`), car ils ne sont utiles
qu'au(x) développeur(s) du programme. En revanche, il faut veiller à ne pas supprimer les `#` qui indiquent un
chargement direct du registre (comme dans la ligne 2 où `#1` n'indique pas un commentaire). Pour faire cette
distinction, je retiens si je suis en train de lire une instruction (donc entre son début et le `;` ou `:` qui la
termine) : un `#` rencontré à ce moment-là appartient à une instruction (comme `LOAD #1;`) et n'est pas traité
comme un commentaire. Dans le cas contraire, il marque bien le début d'un commentaire, qui est alors ignoré jusqu'à
la fin de la ligne. Ainsi, le code suivant :

```asm
main: #programme principal
    LOAD #1;
# HALT;
    STORE @0; # on enregistre 1 dans @0
    HALT; #fin
```

devient :

```asm
main:
    LOAD #1;
    STORE @0;
    HALT;
```

Une fois que le code est nettoyé, on demande au **Lexer** (analyseur lexical) de le découper en une série de
plusieurs « pré-tokens », qui sont, pour l'instant, sous forme de strings. Nous avons donc :

`"main:"` → `"LOAD"` → `"#1"` → `"STORE"` → `"@0"` → `"HALT"`

### Tokénisation et parsing

La tokénisation consiste à créer une liste de « vrais » tokens à partir des pré-tokens. Ils sont considérés comme
des « vrais » tokens car ils possèdent leur propre structure en mémoire, avec les champs suivants :
- Un type
- Une valeur
- Un libellé (facultatif, utilisé pour les étiquettes)

Certains tokens sont typés dès leur création. C'est le cas par exemple de notre étiquette `main:` : dès sa
création, elle devient un token de type `TT_LABEL_DEF`, dont la valeur est `-1` et dont le libellé correspond
à `main`. Ce type est déterminé directement à la lecture du pré-token, grâce au `:` qui indique une définition
d'étiquette (`TT_LABEL_DEF`), et à distinguer d'un saut vers une étiquette (`TT_LABEL_GOTO`, utilisé par exemple
avec `JUMP main`), qui lui n'a pas de `:`.

D'autres tokens nécessitent en revanche une passe supplémentaire après coup pour être ajustés selon le contexte :
une instruction `LOAD` à elle seule ne permet pas de savoir si l'on parle d'un chargement direct du registre ou
d'un chargement depuis la mémoire. Il faut donc lire la valeur du token suivant, et changer la valeur de l'actuel
(`OP_LOAD_DIRECT` → `OP_LOAD_FROM`) si le token suivant indique une adresse au lieu d'un nombre.

Une fois que la contextualisation est effectuée, il reste une avant dernière étape, la validité sémantique du
code. Il s'agit de répondre à la question suivante : est-ce que le code et l'ordre dans lequel les instructions
sont rédigées ont du sens ?

Si la validation passe, le **Parsing** s'effectue. Des données compréhensibles et exploitables par l'interpréteur
sont créées → c'est le programme compilé.

## Programme compilé

Une fois compilé, le programme est représenté sous la forme d'un tableau de N lignes et 2 colonnes. La première
colonne contient l'instruction à exécuter, et la seconde son opérande éventuel. Chaque valeur est un entier signé
sur 32 bits. Si l'instruction ne prend pas d'opérande, la seconde colonne est ignorée.

Le tableau suivant récapitule les instructions du jeu d'instructions de la machine RAM, avec pour chacune son code
hexadécimal, la présence ou non d'un opérande, et son équivalent assembleur correspondant.

| Instruction (hex) | Opérande ? | Equivalent en assembleur |
|-|-|-|
| `0x00000000` | non | Ne rien faire |
| `0x00000001` | oui | `LOAD #?` |
| `0x00000002` | oui | `LOAD @?` |
| `0x00000003` | oui | `STORE @?` |
| `0x00000004` | non | `INCR` |
| `0x00000005` | non | `DECR` |
| `0x00000006` | oui | `JUMP ?` |
| `0x00000007` | oui | `JZ ?` |
| `0x00000008` | non | `HALT` |

## Interpréteur

L'interpréteur prend en entrée un fichier `.bin` contenant le programme compilé, dont le format est décrit dans la 
section précédente. À la lecture, le fichier est chargé en mémoire sous la forme d'une structure `Program`, 
représentant un tableau de N lignes et 2 colonnes, où N est le nombre d'instructions du programme.

L'interpréteur parcourt ensuite ce tableau séquentiellement, en exécutant chaque instruction sur la machine RAM
(registre + mémoire), jusqu'à rencontrer l'instruction `HALT`. Si l'exécution du programme réussit, la fonction qui
interprète le programme renvoie la valeur 0. Sinon, elle renvoie un code d'erreur.

### Codes d'erreur de l'interpréteur

| Code | Signification |
|-|-|
| 0 | Aucune erreur |
| 1 | Overflow, index de lecture en dehors du programme (`<0` OU `>= taille`) |
| 2 | Instruction inconnue |

## Progression

- [x] Machine RAM (mémoire + registre)
- [x] Instructions de base (load, store, increment, decrement)
- [ ] Compilateur
- [ ] Gestion des étiquettes (compilation)
- [x] Fichier binaire → structure Program exploitable par l'interpréteur
- [x] Interpréteur
- [x] Prise en charge des sauts (interpréteur)
- [ ] Interface utilisateur (terminal)
