/**
 * @file main.c
 * @brief Point d'entrée du programme
 * @author Alexandre SBEGHEN
 * @date 2026-06-24
 */

#include <stdio.h>
#include <stdbool.h>
#include "mystring.h"

typedef enum ProgramOptionEnum ProgramOption;
typedef struct PairStringOptionStruct PairStringOption;
typedef enum ExitCodeEnum ExitCode;

enum ProgramOptionEnum {
    OPTN_NONE,
    OPTN_COMPILE,
    OPTN_COMPILE_AND_EXECUTE,
    OPTN_EXECUTE_COMPILED
};
struct PairStringOptionStruct {
    char str[6];
    ProgramOption option;
};
enum ExitCodeEnum {
    EXIT_SUCCESS,
    EXIT_UNSUPPORTED_COMMAND,
};

const PairStringOption OPTIONS[] = {
    {"c", OPTN_COMPILE},
    {"cx", OPTN_COMPILE_AND_EXECUTE},
    {"x", OPTN_EXECUTE_COMPILED}
};
const size_t OPTIONS_COUNT = sizeof(OPTIONS) / sizeof(PairStringOption);

ProgramOption get_program_option_from_string(char* str) {
    for (size_t i = 0; i < OPTIONS_COUNT; ++i)
        if (string_equals(str, OPTIONS[i].str))
            return OPTIONS[i].option;
    return OPTN_NONE;
}
bool file_exists(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}

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
            case OPTN_NONE: {
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
            case OPTN_COMPILE: {
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
                    break;
                }
                break;
            }
            // Compilation & exécution
            case OPTN_COMPILE_AND_EXECUTE: {
                bool input_exists = file_exists(argv[2]);
                bool error = (argc < 3) || !input_exists ;
                if (error) {
                    printf("Command line error:\n");
                    if (argc < 3)
                        printf("You need to specify an input file\n");
                    else if (!input_exists)
                        printf("Cannot find '%s' source code\n", argv[2]);
                    printf("\n");
                    break;
                }
                break;
            }
            // Exécution
            case OPTN_EXECUTE_COMPILED: {
                bool input_exists = file_exists(argv[2]);
                bool error = (argc < 3) || !input_exists ;
                if (error) {
                    printf("Command line error:\n");
                    if (argc < 3)
                        printf("You need to specify an input file\n");
                    else if (!input_exists)
                        printf("Cannot find '%s' binary\n", argv[2]);
                    printf("\n");
                    break;
                }
                break;
            }
        }
    }

    return exit_code;
}