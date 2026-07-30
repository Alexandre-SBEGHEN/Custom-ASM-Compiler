/**
 * @file main.c
 * @brief Point d'entrée du programme
 * @author Alexandre SBEGHEN
 * @date 2026-06-24
 */

#include <stdio.h>
#include <stdbool.h>
#include "mystring.h"

/* --- Alias --------------------------------------------------------------- */

typedef enum ProgramCommandEnum ProgramOption;
typedef struct PairStringCommandStruct PairStringOption;
typedef enum ExitCodeEnum ExitCode;

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
    EXIT_SUCCESS,  /**< Aucune erreur */
    EXIT_UNSUPPORTED_COMMAND, /**< Commande non supportée */
    EXIT_FILE_NOT_FOUND /**< Fichier(s) non trouvé(s) */
};

/* --- Constantes ---------------------------------------------------------- */

const PairStringOption OPTIONS[] = {
    {"c", CMD_COMPILE},
    {"cx", CMD_COMPILE_AND_EXECUTE},
    {"x", CMD_EXECUTE_COMPILED}
};
const size_t OPTIONS_COUNT = sizeof(OPTIONS) / sizeof(PairStringOption);

/* --- Fonctions ----------------------------------------------------------- */

/**
 * @brief Permet d'obtenir une commande à partir d'une string.
 *
 * @param[in] str Pointeur vers la string.
 * @return Commande associée, CMD_NONE si commande non trouvée.
 */
ProgramOption get_program_option_from_string(const char* str) {
    for (size_t i = 0; i < OPTIONS_COUNT; ++i)
        if (string_equals(str, OPTIONS[i].str))
            return OPTIONS[i].option;
    return CMD_NONE;
}

/**
 * @brief Vérifie si un fichier existe.
 *
 * @param[in] filename Chemin vers le fichier.
 * @return Existence du fichier.
 */
bool file_exists(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}

/* --- Main ---------------------------------------------------------------- */

int main(int argc, char** argv) {
    ExitCode exit_code = EXIT_SUCCESS;
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
        ProgramOption option = get_program_option_from_string(argv[1]);

        // Exécution de la commande
        switch (option) {
            // Commande inconnue
            case CMD_NONE: {
                printf(
                    "Command line error:\n"
                    "Unsupported command:\n"
                    "%s\n"
                    "\n",
                    argv[1]
                );
                exit_code = EXIT_UNSUPPORTED_COMMAND;
                break;
            }
            // Compilation
            case CMD_COMPILE: {
                bool input_exists = file_exists(argv[2]);
                bool output_exists = argc >= 4;
                bool error = (argc < 3) || !input_exists || !output_exists;
                if (error) {
                    printf("Command line error:\n");
                    if (argc < 3)
                        printf("You need to specify an input file\n");
                    else if (!input_exists)
                        printf("Cannot find '%s' source code\n", argv[2]);
                    if (!output_exists)
                        printf("You need to specify an output file\n");
                    printf("\n");
                    exit_code = EXIT_FILE_NOT_FOUND;
                    break;
                }
                break;
            }
            // Compilation & exécution
            case CMD_COMPILE_AND_EXECUTE: {
                bool input_exists = file_exists(argv[2]);
                bool error = (argc < 3) || !input_exists ;
                if (error) {
                    printf("Command line error:\n");
                    if (argc < 3)
                        printf("You need to specify an input file\n");
                    else if (!input_exists)
                        printf("Cannot find '%s' source code\n", argv[2]);
                    printf("\n");
                    exit_code = EXIT_FILE_NOT_FOUND;
                    break;
                }
                break;
            }
            // Exécution
            case CMD_EXECUTE_COMPILED: {
                bool input_exists = file_exists(argv[2]);
                bool error = (argc < 3) || !input_exists ;
                if (error) {
                    printf("Command line error:\n");
                    if (argc < 3)
                        printf("You need to specify an input file\n");
                    else if (!input_exists)
                        printf("Cannot find '%s' binary\n", argv[2]);
                    printf("\n");
                    exit_code = EXIT_FILE_NOT_FOUND;
                    break;
                }
                break;
            }
        }

    }

    return exit_code;
}