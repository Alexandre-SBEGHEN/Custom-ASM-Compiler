/**
 * @file interpreter.c
 * @brief Implémentation de l'interpréteur.
 * @author Alexandre SBEGHEN
 * @date 2026-06-25
 */

#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>

/* Création dynamique d'une structure registre Program */
Program* program_create(size_t size) {
    Program* prog;

    if ((prog = malloc(sizeof(Program))) == NULL)
        return NULL;

    prog->size = size;

    if ((prog->inst = malloc(size * sizeof(Instruction))) == NULL) {
        free(prog);
        return NULL;
    }

    return prog;
}

/* Libération de mémoire d'une structure Program */
void program_delete(Program** prog) {
    if (prog == NULL || *prog == NULL)
        return;

    free((*prog)->inst);
    free(*prog);
    *prog = NULL;
}

/* Exécution d'un programme compilé */
int program_interpret(const Program* prog, Machine* mac) {
    size_t inst_index = 0;

    // Boucle du programme
    while (1) {
        // Index en dehors du programme (erreur 1)
        if (inst_index >= prog->size)
            return 1;

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
                return 0;
            // Instruction inconnue (erreur 2)
            default:
                return 2;
        }

        // Instruction suivante
        ++inst_index;
    }
}

/* Création d'un programme à partir d'un fichier .bin */
Program* file_bin_to_program(const char* filename) {
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
    Program* prog = program_create(number_of_instructions);
    if (prog == NULL) return NULL;

    // Encodage des instructions
    for (long i = 0; i < number_of_instructions; ++i) {
        // On place chaque "partie" (octet) du nombre dans un buffer
        unsigned char instbytes[4], argbytes[4];
        if (fread(instbytes, 1, 4, file) != 4 || fread(argbytes, 1, 4, file) != 4) {
            program_delete(&prog);
            return NULL;
        }

        // On concatène les parties pour obtenir le grand nombre (4 octets)
        const int32_t inst = (int32_t)(
            (int32_t)instbytes[0] << 24 |
            (int32_t)instbytes[1] << 16 |
            (int32_t)instbytes[2] << 8 |
            (int32_t)instbytes[3]
        );
        const int32_t arg = (int32_t)(
            (int32_t)argbytes[0] << 24 |
            (int32_t)argbytes[1] << 16 |
            (int32_t)argbytes[2] << 8 |
            (int32_t)argbytes[3]
        );

        // On encode inst et arg dans le programme
        prog->inst[i].op = (Opcode)(inst);
        prog->inst[i].arg = arg;
    }

    fclose(file);
    return prog;
}