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

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "machine.h"

/* --- Alias --------------------------------------------------------------- */

typedef enum OpcodeEnum Opcode;
typedef enum InterpreterErrorsEnum InterpreterErrors;
typedef struct InstructionStruct Instruction;
typedef struct ProgramStruct Program;

/* --- Enums et Structs ---------------------------------------------------- */

/**
 * @brief Code des différentes instructions.
 */
enum OpcodeEnum {
    OP_NOTHING,
    OP_LOAD_DIRECT,
    OP_LOAD_FROM,
    OP_STORE_TO,
    OP_INCR,
    OP_DECR,
    OP_JUMP,
    OP_JZ,
    OP_HALT,
    OP_PRINT_CHAR,
    OP_PRINT_INT
};

/**
 * @brief Codes d'erreur de l'interpréteur lors de
 * l'exécution d'un programme.
 */
enum InterpreterErrorsEnum {
    IERR_SUCCESS, /**< Aucune erreur */
    IERR_MISSING_MAC_OR_REG, /**< Machine et/ou registre manquant(s) */
    IERR_OVERFLOW, /**< Index de lecture en dehors du programme  */
    IERR_UNKNOWN_INSTRUCTION /**< Instruction inconnue */
};

/**
 * @brief Structure d'une instruction.
 */
struct InstructionStruct {
    Opcode op; /**< Opération */
    int32_t arg; /**< Argument / Opérande */
};

/**
 * @brief Structure du programme compilé.
 */
struct ProgramStruct {
    Instruction* inst; /**< Liste des instructions */
};

/* --- Fonctions ----------------------------------------------------------- */

/**
 * @brief Crée dynamiquement une structure programme Program.
 *
 * @return Pointeur vers la structure allouée ou NULL en cas
 * d'échec d'allocation.
 *
 * @note Penser à libérer la mémoire après utilisation.
 * @see program_delete()
 */
Program* program_create();

/**
 * @brief Libération de mémoire d'une structure Program.
 *
 * @param[in, out] prog Adresse du pointeur vers la structure.
 */
void program_delete(Program* prog);

/**
 * @brief Interprète un programme compilé sous la forme d'une structure Program.
 *
 * @param prog Pointeur vers la structure Program.
 * @param mac Pointeur vers la structure Machine.
 * @return Code d'erreur (ou réussite).
 */
InterpreterErrors program_interpret(const Program* prog, Machine* mac);

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

/**
 * @brief Sauvegarde un program compilé vers
 * un fichier binaire.
 *
 * @param[in] prog Pointeur vers la structure du programme.
 * @param[in] filename Chemin vers le fichier.
 */
void program_save_to(const Program* prog, const char* filename);

#endif
