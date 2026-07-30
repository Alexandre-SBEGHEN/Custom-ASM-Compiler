/**
 * @file interpreter.c
 * @brief Implémentation de l'interpréteur.
 * @author Alexandre SBEGHEN
 * @date 2026-06-25
 */

#include "interpreter.h"
#include <stdio.h>
#include "myarray.h"
#include "stdbool.h"

/* Création dynamique d'une structure registre Program */
Program* program_create() {
    Program* prog;

    if ((prog = malloc(sizeof(Program))) == NULL)
        return NULL;

    prog->inst = array_create(Instruction);

    return prog;
}

/* Libération de mémoire d'une structure Program */
void program_delete(Program* prog) {
    if (prog == NULL)
        return;

    array_delete(prog->inst);
    free(prog);
}

/* Exécution d'un programme compilé */
InterpreterErrors program_interpret(const Program* prog, Machine* mac) {
    if (prog == NULL || mac == NULL)
        return IERR_MISSING_MAC_OR_REG;

    size_t inst_index = 0;

    // Boucle du programme
    while (true) {
        // Index en dehors du programme (erreur 1)
        if (inst_index >= array_size(prog->inst))
            return IERR_OVERFLOW;

        // Obtenir la paire opération / argument
        const Opcode op = prog->inst[inst_index].op;
        const int32_t arg = prog->inst[inst_index].arg;

        // Action selon l'instruction
        switch (op) {
            // Ne rien faire
            case OP_NOTHING:
                break;
            // LOAD #?
            case OP_LOAD_DIRECT:
                ram_load_direct(mac, arg);
                break;
            // LOAD @?
            case OP_LOAD_FROM:
                ram_load_from(mac, arg);
            // STORE @?
            case OP_STORE_TO:
                ram_store_to(mac, arg);
                break;
            // INCR
            case OP_INCR:
                ram_increment(mac);
                break;
            // DECR
            case OP_DECR:
                ram_decrement(mac);
                break;
            // JUMP ?
            case OP_JUMP:
                // Gérer l'overflow (erreur 1)
                if (arg < 0)
                    return 1;
                inst_index = arg;
                continue;
            // JZ ?
            case OP_JZ:
                // Gérer l'overflow (erreur 1)
                if (arg < 0)
                    return 1;
                if (mac->reg->val <= 0) {
                    inst_index = arg;
                    continue;
                }
                break;
            // HALT
            case OP_HALT:
                return IERR_SUCCESS;
            // Instruction inconnue (erreur 2)
            default:
                return IERR_UNKNOWN_INSTRUCTION;
        }

        // Instruction suivante
        ++inst_index;
    }
}

/* Création d'un programme à partir d'un fichier .bin */
Program* file_bin_to_program(const char* filename) {
    if (filename == NULL)
        return NULL;

    FILE* file = fopen(filename, "rb");
    if (file == NULL)
        return NULL;

    //Vérifier que le fichier contient des blocs de 8 octets (paire instruction / opérande)
    fseek(file, 0, SEEK_END);
    const long file_size = ftell(file);
    rewind(file);
    if (file_size % 8 != 0) return NULL;

    // Le nombre d'instructions est la taille du fichier / 8
    const long number_of_instructions = file_size / 8;
    Program* prog = program_create();
    if (prog == NULL) return NULL;

    // Encodage des instructions
    for (long i = 0; i < number_of_instructions; ++i) {
        // On place chaque "partie" (octet) du nombre dans un buffer
        unsigned char opbytes[4], argbytes[4];
        if (fread(opbytes, 1, 4, file) != 4 || fread(argbytes, 1, 4, file) != 4) {
            program_delete(prog);
            return NULL;
        }

        // On concatène les parties pour obtenir le grand nombre (4 octets)
        const int32_t op = (int32_t)(
            (int32_t)opbytes[0] << 24 |
            (int32_t)opbytes[1] << 16 |
            (int32_t)opbytes[2] << 8 |
            (int32_t)opbytes[3]
        );
        const int32_t arg = (int32_t)(
            (int32_t)argbytes[0] << 24 |
            (int32_t)argbytes[1] << 16 |
            (int32_t)argbytes[2] << 8 |
            (int32_t)argbytes[3]
        );

        // On encode inst et arg dans le programme
        array_push(prog->inst, ((Instruction){(Opcode)(op), arg}));
    }

    fclose(file);
    return prog;
}