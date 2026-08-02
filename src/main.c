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

typedef enum ProgramCommandEnum ProgramCommand;
typedef struct PairStringCommandStruct PairStringCommand;
typedef enum ExitCodeEnum ExitCode;
typedef enum FileCheckResultEnum FileCheckResult;

/* --- Enums et Structs ---------------------------------------------------- */

/**
 * @brief Commandes possibles dans le programme.
 */
enum ProgramCommandEnum {
    CMD_UNKNOWN, /**< Commande inconnue */
    CMD_COMPILE, /**< Compilation d'un fichier source */
    CMD_COMPILE_AND_EXECUTE, /**< Compilation & exécution d'un fichier source */
    CMD_EXECUTE_COMPILED /**< Exécution d'un binaire */
};

/**
 * @brief Paire string / commande.
 */
struct PairStringCommandStruct {
    char str[6]; /**< String */
    ProgramCommand option; /**< La commande associée. */
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

const PairStringCommand COMMANDS[] = {
    {"c", CMD_COMPILE},
    {"cx", CMD_COMPILE_AND_EXECUTE},
    {"x", CMD_EXECUTE_COMPILED}
};
const size_t COMMANDS_COUNT = sizeof(COMMANDS) / sizeof(PairStringCommand);

/* --- Fonctions ----------------------------------------------------------- */

/**
 * @brief Permet d'obtenir une commande à partir d'une string.
 *
 * @param[in] str Pointeur vers la string.
 * @return Commande associée, CMD_NONE si commande non trouvée.
 */
ProgramCommand get_program_command_from_string(const char* str) {
    for (size_t i = 0; i < COMMANDS_COUNT; ++i)
        if (string_equals(str, COMMANDS[i].str))
            return COMMANDS[i].option;
    return CMD_UNKNOWN;
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
    const char* executable_name = argv[0];

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
            executable_name,
            "c", "Compile a source code into a binary file",
            "cx", "Compile and execute a source code without creating a binary file",
            "x", "Execute a binary file"
        );

    } else {
        // Obtenir les entrées utilisateur
        const char* command_str = argv[1];
        const ProgramCommand command = get_program_command_from_string(command_str);
        const char* input = (argc >= 3) ? argv[2] : NULL;
        const char* output = (argc >= 4) ? argv[3] : NULL;

        const bool missing_input = input == NULL;
        const bool missing_output = input == NULL;

        // Exécution de la commande
        switch (command) {
            // Commande inconnue
            case CMD_UNKNOWN: {
                printf(
                    "Command line error:\n"
                    "Unsupported command:\n"
                    "%s\n"
                    "\n",
                    command_str
                );
                exit_code = EXCODE_UNSUPPORTED_COMMAND;
                break;
            }
            // Compilation
            case CMD_COMPILE: {
                //Erreur(s)
                const FileCheckResult input_check = missing_input ? FILE_NOT_FOUND : input_file_check(input);
                const FileCheckResult output_check = missing_output ? FILE_NOT_FOUND : output_file_check(output);
                const bool error = (input_check != FILE_OK || output_check != FILE_OK);
                if (error) {
                    printf("Command line error:\n");

                    // Erreur d'input
                    if (missing_input)
                        printf("You need to specify an input file\n");
                    else if (input_check == FILE_NOT_FOUND)
                        printf("Cannot find '%s' source code\n", input);
                    else if (input_check == FILE_IS_A_DIRECTORY)
                        printf("'%s' is a directory\n", input);

                    // Erreur d'output
                    if (missing_output)
                        printf("You need to specify an output file\n");
                    else if (output_check == FILE_IS_A_DIRECTORY)
                        printf("'%s' is a directory\n", output);

                    exit_code = EXCODE_INPUT_OUTPUT_ERROR;
                    break;
                }

                // Compilation
                // Program* prog = program_compile(input);
                // program_delete(prog);

                // Succès
                printf(
                    "File compiled successfully to %s\n"
                    "\n",
                    output
                );
                break;
            }
            // Compilation & exécution
            case CMD_COMPILE_AND_EXECUTE: {
                // Erreur(s)
                const FileCheckResult input_check = missing_input ? FILE_NOT_FOUND : input_file_check(argv[2]);
                const bool error = (input_check != FILE_OK);
                if (error) {
                    printf("Command line error:\n");

                    // Erreur d'input
                    if (missing_input)
                        printf("You need to specify an input file\n");
                    else if (input_check == FILE_NOT_FOUND)
                        printf("Cannot find '%s' source code\n", input);
                    else if (input_check == FILE_IS_A_DIRECTORY)
                        printf("'%s' is a directory\n", input);

                    exit_code = EXCODE_INPUT_OUTPUT_ERROR;
                    break;
                }
                break;
            }
            // Exécution
            case CMD_EXECUTE_COMPILED: {
                // Erreur(s)
                const FileCheckResult input_check = missing_input ? FILE_NOT_FOUND : input_file_check(argv[2]);
                const bool error = (input_check != FILE_OK);
                if (error) {
                    printf("Command line error:\n");

                    // Erreur d'input
                    if (missing_input)
                        printf("You need to specify an input file\n");
                    else if (input_check == FILE_NOT_FOUND)
                        printf("Cannot find '%s' source code\n", input);
                    else if (input_check == FILE_IS_A_DIRECTORY)
                        printf("'%s' is a directory\n", input);

                    exit_code = EXCODE_INPUT_OUTPUT_ERROR;
                    break;
                }
            }
        }
    }

    return exit_code;
}