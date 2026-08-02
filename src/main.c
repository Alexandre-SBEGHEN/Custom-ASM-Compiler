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
    EXCODE_UNINITIALIZED, /**< Non initialisé (pas censé apparaître !) */
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

/* --- Commandes & fichiers ---------------------------- */

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

/* --- Affichage --------------------------------------- */

/**
 * @brief Fait un print d'un retour à la ligne.
 */
void print_line_break(void) {
    printf("\n");
}

/**
 * @brief Affiche l'en-tête du programme.
 *
 * Contient des infos sur le programme comme le nom,
 * l'auteur, la date de création, etc.
 */
void print_program_header(void) {
    printf(
        "\n"
        "Assembly compiler & interpreter for RAM Machine, Alexandre SBEGHEN, 2026\n"
        "GitHub: https://github.com/Alexandre-SBEGHEN\n"
        "Repo: https://github.com/Alexandre-SBEGHEN/RAM-Machine-ASM-Compiler-Interpreter\n"
    );
    print_line_break();
}

/**
 * @brief Affiche un guide d'utilisation
 * de l'application.
 *
 * @param[in] executable_name Nom de l'exécutable
 */
void print_application_usage(const char* executable_name) {
    printf(
        "Usage: %s <command> [<file_names>...]\n"
        "\n"
        "<Commands>\n"
        "  %-4s : %s\n"
        "  %-4s : %s\n"
        "  %-4s : %s\n",
        executable_name,
        "c", "Compile a source code into a binary file",
        "cx", "Compile and execute a source code without creating a binary file",
        "x", "Execute a binary file"
    );
}

/**
 * @brief Affiche l'en-tête du message d'erreur
 * de ligne de commande.
 */
void print_command_line_error_header(void) {
    printf("Command line error:\n");
}

/**
 * @brief Affiche l'erreur de la commande
 * comme étant non supportée.
 */
void print_unsopported_command_error(const char* command) {
    printf(
        "Unsupported command:\n"
        "%s\n",
        command
    );
}

/**
 * @brief Affiche l'erreur au sujet du fichier d'entrée.
 *
 * @param[in] input Pointeur vers la string de l'input.
 * @param[in] missing_input Est-ce que le fichier est manquant ?
 * @param[in] input_check Résultat de vérification du fichier.
 * @param[in] kind_of_input Type de fichier (texte, sourcen, binaire, etc.).
 */
void print_input_file_error(const char* input, const bool missing_input, const FileCheckResult input_check, const char* kind_of_input) {
    if (missing_input)
        printf("You need to specify an input file\n");
    else if (input_check == FILE_NOT_FOUND)
        printf("Cannot find '%s' %s\n", input, kind_of_input);
    else if (input_check == FILE_IS_A_DIRECTORY)
        printf("'%s' is a directory\n", input);
}

/**
 * @brief Affiche l'erreur au sujet du fichier de sortie.
 *
 * @param[in] output Pointeur vers la string de l'output.
 * @param[in] missing_output Est-ce que le fichier est manquant ?
 * @param[in] output_check Résultat de vérification du fichier.
 */
void print_output_file_error(const char* output, const bool missing_output, const FileCheckResult output_check) {
    if (missing_output)
        printf("You need to specify an output file\n");
    else if (output_check == FILE_IS_A_DIRECTORY)
        printf("'%s' is a directory\n", output);
}

void print_file_compilation_error(const CompilerErrors error) {
    printf("Compilation error:\n");
    switch (error) {
        case CERR_SUCCESS:
            break;
        case CERR_NO_TOKENS:
            printf("File is empty\n");
            break;
        case CERR_UNKNOWN_INSTRUCTION:
            printf("Unknown instruction\n");
            break;
        case CERR_ORPHAN_OPERAND:
            printf("Orphan operand found\n");
            break;
        case CERR_INST_W_WRONG_OPERAND:
            printf("Instruction with wrong or witout operand\n");
            break;
        case CERR_JUMP_W_O_LABEL:
            printf("Jump found without label\n");
            break;
        case CERR_LABEL_ALREADY_DEFINED:
            printf("Declaration of an already existing label\n");
            break;
        case CERR_LABEL_UNDEFINED:
            printf("Mentionning an undefined label\n");
            break;
    }
}

/**
 * @brief Affiche le message de réussite
 * de compilation du fichier.
 *
 * @param[in] filename Chemin du fichier compilé.
 */
void print_file_compilation_success(const char* filename) {
    printf(
        "File compiled successfully to %s\n"
        "\n",
        filename
    );
}

/* --- Main ---------------------------------------------------------------- */

int main(int argc, char** argv) {
    ExitCode exit_code = EXCODE_UNINITIALIZED;
    const char* executable_name = argv[0];
    const bool entered_no_command = argc == 1;

    // Header de l'application
    print_program_header();

    // [Guide d'utilisation]
    if (entered_no_command) {
        print_application_usage(executable_name);
        print_line_break();
        return EXCODE_SUCCESS;
    }

    // Obtenir les entrées utilisateur
    const char* command = argv[1];
    const ProgramCommand program_command = get_program_command_from_string(command);
    const char* input = (argc >= 3) ? argv[2] : NULL;
    const char* output = (argc >= 4) ? argv[3] : NULL;

    // Vérification des erreurs potentielles en amont
    const bool missing_input = input == NULL;
    const bool missing_output = output == NULL;
    const FileCheckResult input_check = missing_input ? FILE_NOT_FOUND : input_file_check(input);
    const FileCheckResult output_check = missing_output ? FILE_NOT_FOUND : output_file_check(output);
    bool command_line_error;

    // Exécution de la commande
    switch (program_command) {
        // Commande inconnue
        case CMD_UNKNOWN: {
            print_command_line_error_header();
            print_unsopported_command_error(command);
            print_line_break();
            exit_code = EXCODE_UNSUPPORTED_COMMAND;
            break;
        }
        // Compilation
        case CMD_COMPILE: {
            // Erreur(s) de la ligne de commande
            command_line_error = (input_check != FILE_OK || output_check != FILE_OK);
            if (command_line_error) {
                print_command_line_error_header();
                print_input_file_error(input, missing_input, input_check, "source code");
                print_output_file_error(output, missing_output, output_check);
                print_line_break();
                exit_code = EXCODE_INPUT_OUTPUT_ERROR;
                break;
            }

            // Erreur de compilation
            const CompilerErrors compilation_error = program_is_compilable(input);
            if (compilation_error != CERR_SUCCESS) {
                print_file_compilation_error(compilation_error);
                print_line_break();
                break;
            }

            // Succès
            print_file_compilation_success(output);
            exit_code = EXCODE_SUCCESS;
            break;
        }
        // Compilation & exécution
        case CMD_COMPILE_AND_EXECUTE: {
            // Erreur(s) de la ligne de commande
            command_line_error = input_check != FILE_OK;
            if (command_line_error) {
                print_command_line_error_header();
                print_input_file_error(input, missing_input, input_check, "source code");
                print_line_break();
                exit_code = EXCODE_INPUT_OUTPUT_ERROR;
                break;
            }

            // Succès
            exit_code = EXCODE_SUCCESS;
            break;
        }
        // Exécution
        case CMD_EXECUTE_COMPILED: {
            // Erreur(s) de la ligne de commande
            command_line_error = (input_check != FILE_OK);
            if (command_line_error) {
                print_command_line_error_header();
                print_input_file_error(input, missing_input, input_check, "binary");
                print_line_break();
                exit_code = EXCODE_INPUT_OUTPUT_ERROR;
                break;
            }

            // Succès
            exit_code = EXCODE_SUCCESS;
            break;
        }
    }

    return exit_code;
}
