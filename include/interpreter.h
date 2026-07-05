/**
 * @file interpreter.h
 * @brief Interpréteur du code compilé
 *
 * Allocation, suppression, interprétation
 * d'un programme assembleur compilé
 *
 * @author Alexandre SBEGHEN
 * @date 2026-06-25
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdint.h>
#include <stddef.h>

#include "machine.h"

/* --- Constantes ---------------------------------------------------------- */

/**
 * @brief Code des différentes instructions.
 */
enum OpcodeEnum {
    OP_NOTHING,
    OP_LOAD,
    OP_LOAD_DIRECT,
    OP_LOAD_FROM,
    OP_STORE_TO,
    OP_INCR,
    OP_DECR,
    OP_JUMP,
    OP_JZ,
    OP_HALT
};

/* --- Alias --------------------------------------------------------------- */

typedef enum OpcodeEnum Opcode;
typedef struct InstructionStruct Instruction;
typedef struct ProgramStruct Program;

/* --- Structs ------------------------------------------------------------- */

/**
 * @brief Structure d'une instruction.
 * Contient deux champs opération et argument/opérande
 */
struct InstructionStruct {
    Opcode op;
    int32_t arg;
};

/**
 * @brief Structure du programme compilé.
 * Champs:
 * - size : nombre d'instructions
 * - inst : tableau de structures d'instructions Instruction
 */
struct ProgramStruct {
    size_t size;
    Instruction* inst;
};

/* --- Fonctions ----------------------------------------------------------- */

/**
 * @brief Crée dynamiquement une structure programme Program.
 *
 * Valeurs de la matrice initialisées par défaut à 0.
 *
 * @param[in] size Nombre d'instructions (=taille) du programme.
 * @return Pointeur vers la structure allouée ou NULL en cas
 * d'échec d'allocation.
 *
 * @note Penser à libérer la mémoire après utilisation.
 * @see program_delete()
 */
Program* program_create(size_t size);

/**
 * @brief Libération de mémoire d'une structure Program.
 *
 * Utilisation du double pointeur en paramètre pour
 * mettre automatiquement sa valeur à NULL.
 *
 * @param[in, out] prog Adresse du pointeur vers la structure.
 */
void program_delete(Program** prog);

/**
 * @brief Interprète un programme compilé sous la forme d'une structure Program.
 *
 * @param prog Pointeur vers la structure Program.
 * @param mac Pointeur vers la structure Machine.
 * @return 0 en cas de réussite, autre valeur en cas d'échec.
 */
int program_interpret(const Program* prog, Machine* mac);

/**
 * @brief Génère une structure Program à partir d'un fichier binaire (asm compilé).
 *
 * @param filename Chemin vers le fichier.
 * @return Pointeur vers la structure Program, NULL en cas d'échec.
 *
 * @note Penser à libérer la mémoire après utilisation.
 * @see program_delete()
 */
Program* file_bin_to_program(const char* filename);

#endif
