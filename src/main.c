/**
 * @file main.c
 * @brief Point d'entrée du programme
 * @author Alexandre SBEGHEN
 * @date 2026-06-24
 */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include "mystring.h"
#include "machine.h"
#include "compiler.h"
#include "interpreter.h"

/* --- Alias --------------------------------------------------------------- */

typedef enum ProgramCommandEnum ProgramOption;
typedef struct PairStringCommandStruct PairStringOption;
typedef enum ExitCodeEnum ExitCode;
typedef enum FileCheckResultEnum FileCheckResult;

/* --- Enums et Structs ---------------------------------------------------- */

/**
 * @brief Commandes possibles dans le programme.
 */
enum ProgramCommandEnum {
    CMD_NONE, /**< Commande inconnue */
    CMD_COMPILE, /**< Compilation d'un fichier source */
    CMD_COMPILE_AND_EXECUTE, /**< Compilation & exécution d'un fichier source */
    CMD_EXECUTE_COMPILED /**< Exécution d'un binaire */
};

/**
 * @brief Paire string / commande.
 */
struct PairStringCommandStruct {
    char str[6]; /**< String */
    ProgramOption option; /**< La commande associée. */
};

/**
 * @brief Code de sortie du programme.
 */
enum ExitCodeEnum {
    EXCODE_SUCCESS,  /**< Aucune erreur */
    EXCODE_UNSUPPORTED_COMMAND, /**< Commande non supportée */
    EXCODE_INPUT_OUTPUT_ERROR, /**< Erreur liée aux fichiers d'entrée/sortie */
};

/**
 * @brief Code de vérification d'un fichier.
 */
enum FileCheckResultEnum {
    FILE_OK, /**< Fichier OK */
    FILE_NOT_FOUND, /**< Fichier non trouvé */
    FILE_IS_A_DIRECTORY, /**< Le fichier est un répertoire */
};

/* --- Constantes ---------------------------------------------------------- */

const PairStringOption COMMANDS[] = {
    {"c", CMD_COMPILE},
    {"cx", CMD_COMPILE_AND_EXECUTE},
    {"x", CMD_EXECUTE_COMPILED}
};
const size_t COMMANDS_COUNT = sizeof(COMMANDS) / sizeof(PairStringOption);

/* --- Fonctions ----------------------------------------------------------- */

/**
 * @brief Permet d'obtenir une commande à partir d'une string.
 *
 * @param[in] str Pointeur vers la string.
 * @return Commande associée, CMD_NONE si commande non trouvée.
 */
ProgramOption get_program_command_from_string(const char* str) {
    for (size_t i = 0; i < COMMANDS_COUNT; ++i)
        if (string_equals(str, COMMANDS[i].str))
            return COMMANDS[i].option;
    return CMD_NONE;
}

/**
 * @brief Cette fonction permet d'obtenir la taille
 * (en octets) d'un fichier.
 *
 * @param[in] filename Chemin vers le fichier.
 * @return Taille du fichier, -1 si fichier non trouvé.
 */
long file_size(const char* filename) {
    struct stat info;
    if (stat(filename, &info) != 0)
        return -1; // Pas trouvé

    return info.st_size;
}

/**
 * @brief Vérification de fichier d'entrée.
 *
 * Vérifie si le chemin mène à un fichier d'entrée valide :
 * - Existe
 * - N'est pas un dossier
 *
 * @param[in] filename Chemin vers le fichier.
 * @return Résultat de la vérification.
 */
FileCheckResult input_file_check(const char* filename) {
    struct stat info;
    if (stat(filename, &info) != 0)
        return FILE_NOT_FOUND; // Pas trouvé

    if (!S_ISREG(info.st_mode))
        return FILE_IS_A_DIRECTORY; // C'est un répertoire

    return FILE_OK;
}

/**
 * @brief Vérification de fichier de sortie.
 *
 * Vérifie si le chemin mène à un fichier de sortie valide :
 * - Existe ou non
 * - N'est pas un dossier
 *
 * @param[in] filename Chemin vers le fichier.
 * @return Résultat de la vérification.
 */
FileCheckResult output_file_check(const char* filename) {
    struct stat info;
    if (stat(filename, &info) != 0)
        return FILE_OK; // Existe pas, sera créé donc OK

    if (S_ISDIR(info.st_mode))
        return FILE_IS_A_DIRECTORY; // C'est un répertoire

    return FILE_OK; // Existe déjà, sera peut-être écrasé
}

/* --- Main ---------------------------------------------------------------- */

int main(int argc, char** argv) {
    ExitCode exit_code = EXCODE_SUCCESS;
    const char* exec_name = argv[0];

    // Présentation du programme
    printf(
        "\n"
        "Assembly compiler & interpreter for RAM Machine, Alexandre SBEGHEN, 2026\n"
        "GitHub: https://github.com/Alexandre-SBEGHEN\n"
        "Repo: https://github.com/Alexandre-SBEGHEN/RAM-Machine-ASM-Compiler-Interpreter\n"
        "\n"
    );

    if (argc <= 1) {
        // Guide d'utilisation
        printf(
            "Usage: %s <command> [<file_names>...]\n"
            "\n"
            "<Commands>\n"
            "  %-4s : %s\n"
            "  %-4s : %s\n"
            "  %-4s : %s\n"
            "\n",
            exec_name,
            "c", "Compile a source code into a binary file",
            "cx", "Compile and execute a source code without creating a binary file",
            "x", "Execute a binary file"
        );

    } else {
        // Obtenir la commande entrée par l'utilisateur
        ProgramOption cmd = get_program_command_from_string(argv[1]);

        // Exécution de la commande
        switch (cmd) {
            // Commande inconnue
            case CMD_NONE: {
                printf(
                    "Command line error:\n"
                    "Unsupported command:\n"
                    "%s\n"
                    "\n",
                    argv[1]
                );
                exit_code = EXCODE_UNSUPPORTED_COMMAND;
                break;
            }
            // Compilation
            case CMD_COMPILE: {
                //Erreur(s)
                bool missing_input = argc < 3;
                bool missing_output = argc < 4;
                FileCheckResult input_check = missing_input ? FILE_NOT_FOUND : input_file_check(argv[2]);
                FileCheckResult output_check = missing_output ? FILE_NOT_FOUND : output_file_check(argv[3]);
                bool error = (input_check != FILE_OK || output_check != FILE_OK);
                if (error) {
                    printf("Command line error:\n");

                    // Erreur d'input
                    if (missing_input)
                        printf("You need to specify an input file\n");
                    else if (input_check == FILE_NOT_FOUND)
                        printf("Cannot find '%s' source code\n", argv[2]);
                    else if (input_check == FILE_IS_A_DIRECTORY)
                        printf("'%s' is a directory\n", argv[2]);

                    // Erreur d'output
                    if (missing_output)
                        printf("You need to specify an output file\n");
                    else if (output_check == FILE_IS_A_DIRECTORY)
                        printf("'%s' is a directory\n", argv[3]);

                    exit_code = EXCODE_INPUT_OUTPUT_ERROR;
                    break;
                }

                // Compilation
                Program* prog = program_compile(argv[2]);



                // Succès
                printf(
                    "File compiled successfully to %s\n"
                    "\n",
                    argv[3]
                );
                break;
            }
            // Compilation & exécution
            case CMD_COMPILE_AND_EXECUTE: {
                // Erreur(s)
                bool missing_input = argc < 3;
                FileCheckResult input_check = missing_input ? FILE_NOT_FOUND : input_file_check(argv[2]);
                bool error = (input_check != FILE_OK);
                if (error) {
                    printf("Command line error:\n");

                    // Erreur d'input
                    if (missing_input)
                        printf("You need to specify an input file\n");
                    else if (input_check == FILE_NOT_FOUND)
                        printf("Cannot find '%s' source code\n", argv[2]);
                    else if (input_check == FILE_IS_A_DIRECTORY)
                        printf("'%s' is a directory\n", argv[2]);

                    exit_code = EXCODE_INPUT_OUTPUT_ERROR;
                    break;
                }


                break;
            }
            // Exécution
            case CMD_EXECUTE_COMPILED: {
                // Erreur(s)
                bool missing_input = argc < 3;
                FileCheckResult input_check = missing_input ? FILE_NOT_FOUND : input_file_check(argv[2]);
                bool error = (input_check != FILE_OK);
                if (error) {
                    printf("Command line error:\n");

                    // Erreur d'input
                    if (missing_input)
                        printf("You need to specify an input file\n");
                    else if (input_check == FILE_NOT_FOUND)
                        printf("Cannot find '%s' source code\n", argv[2]);
                    else if (input_check == FILE_IS_A_DIRECTORY)
                        printf("'%s' is a directory\n", argv[2]);

                    exit_code = EXCODE_INPUT_OUTPUT_ERROR;
                    break;
                }
            }
        }
    }

    return exit_code;
}