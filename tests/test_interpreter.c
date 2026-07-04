/**
 * @file test_interpreter.c
 * @brief Test des implémentations de interpreter.h / interpreter.c
 * @author Alexandre SBEGHEN
 * @date 2026-06-25
 */

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "machine.h"
#include "interpreter.h"

/**
 * @brief Test de la fonction program_create().
 *
 * Vérifie que :
 * - Le registre créé n'est pas NULL ;
 * - Sa taille est correcte ;
 * - Sa liste d'instructions n'est pas NULL ;
 * - Les op/arg des instructions sont nuls.
 *
 * @see program_create()
 */
void test_program_create() {
    for (size_t size = 1; size < 128; ++size) {
        Program* prog = program_create(size);

        assert(prog != NULL);
        assert(prog->size == size);
        assert(prog->inst != NULL);
        for (size_t i = 0; i < prog->size; ++i) {
            assert(prog->inst[i].op == 0);
            assert(prog->inst[i].arg == 0);
        }

        program_delete(&prog);
    }
}

/**
 * @brief Test de la fonction program_delete().
 *
 * Vérifie que :
 * - Le pointeur est défini à NULL.
 *
 * @see program_delete()
 */
void test_program_delete() {
    for (size_t size = 0; size < 128; ++size) {
        Program* prog = program_create(size);
        program_delete(&prog);

        assert(prog == NULL);
    }
}

/**
 * @brief Test de la fonction program_interpret().
 *
 * Exécute plusieurs programmes et vérifie les valeurs de la
 * mémoire en résultat.
 *
 * @see program_interpret()
 */
void test_program_interpret() {
    Machine* mac = machine_create(2);
    Program* prog = program_create(16);

    // Programme 1 (a = 1)
    Instruction prog_a_equals_1[3] = {
        {OP_LOAD_DIRECT, 1},
        {OP_STORE_TO, 0},
        {OP_HALT, 0}
    };
    memcpy(prog->inst, prog_a_equals_1, sizeof(prog_a_equals_1));
    program_interpret(prog, mac);
    assert(mac->mem->data[0] == 1);

    // Programme 2 (a = a + 10)
    Instruction prog_a_equals_a_plus_10[10] = {
        {OP_LOAD_DIRECT, -9},
        {OP_STORE_TO, 2},
        {OP_LOAD_FROM, 0},
        {OP_INCR, 0},
        {OP_STORE_TO, 0},
        {OP_LOAD_FROM, 2},
        {OP_INCR, 0},
        {OP_STORE_TO, 2},
        {OP_JZ, 2},
        {OP_HALT, 0}
    };
    memcpy(prog->inst, prog_a_equals_a_plus_10, sizeof(prog_a_equals_a_plus_10));
    mac->mem->data[0] = 0;
    program_interpret(prog, mac);
    assert(mac->mem->data[0] == 10);
    mac->mem->data[0] = 5;
    program_interpret(prog, mac);
    assert(mac->mem->data[0] == 15);
    mac->mem->data[0] = -5;
    program_interpret(prog, mac);
    assert(mac->mem->data[0] == 5);
    mac->mem->data[0] = -10;
    program_interpret(prog, mac);
    assert(mac->mem->data[0] == 0);

    // Programme 3 (a = |a|)
    Instruction prog_a_equals_abs_of_a[16] = {
        {OP_LOAD_FROM, 0},
        {OP_INCR, 0},
        {OP_STORE_TO, 1},
        {OP_LOAD_FROM, 0},
        {OP_INCR, 0},
        {OP_JZ, 7},
        {OP_JUMP, 15},
        {OP_LOAD_FROM, 0},
        {OP_INCR, 0},
        {OP_INCR, 0},
        {OP_STORE_TO, 0},
        {OP_LOAD_FROM, 1},
        {OP_INCR, 0},
        {OP_STORE_TO, 1},
        {OP_JZ, 7},
        {OP_HALT, 0}
    };
    memcpy(prog->inst, prog_a_equals_abs_of_a, sizeof(prog_a_equals_abs_of_a));
    mac->mem->data[0] = 0;
    program_interpret(prog, mac);
    assert(mac->mem->data[0] == 0);
    mac->mem->data[0] = 5;
    program_interpret(prog, mac);
    assert(mac->mem->data[0] == 5);
    mac->mem->data[0] = -1;
    program_interpret(prog, mac);
    assert(mac->mem->data[0] == 1);
    mac->mem->data[0] = -67;
    program_interpret(prog, mac);
    assert(mac->mem->data[0] == 67);

    program_delete(&prog);
    machine_delete(&mac);
}

/**
 * @brief Test de la fonction file_bin_to_program().
 *
 * Lit plusieurs fichiers et vérifie que la structure générée
 * en résultat est la bonne.
 *
 * @see file_bin_to_program()
 */
void test_file_bin_to_program() {
    Program* prog;

    // Programme 1 (a = 1)
    prog = file_bin_to_program(
    PROJECT_ROOT "/tests/testdata/test_interpreter/test_file_bin_to_program/test_a_equals_1.bin"
    );
    assert(prog != NULL);
    Instruction prog_a_equals_1[3] = {
        {OP_LOAD_DIRECT, 1},
        {OP_STORE_TO, 0},
        {OP_HALT, 0}
    };
    for (size_t i = 0; i < prog->size; ++i) {
        assert(prog->inst[i].op == prog_a_equals_1[i].op);
        assert(prog->inst[i].arg == prog_a_equals_1[i].arg);
    }
    program_delete(&prog);

    // Programme 2 (a = a + 10)
    prog = file_bin_to_program(
    PROJECT_ROOT "/tests/testdata/test_interpreter/test_file_bin_to_program/test_a_equals_a_plus_10.bin"
    );
    assert(prog != NULL);
    Instruction prog_a_equals_a_plus_10[10] = {
        {OP_LOAD_DIRECT, -9},
        {OP_STORE_TO, 2},
        {OP_LOAD_FROM, 0},
        {OP_INCR, 0},
        {OP_STORE_TO, 0},
        {OP_LOAD_FROM, 2},
        {OP_INCR, 0},
        {OP_STORE_TO, 2},
        {OP_JZ, 2},
        {OP_HALT, 0}
    };
    for (size_t i = 0; i < prog->size; ++i) {
        assert(prog->inst[i].op == prog_a_equals_a_plus_10[i].op);
        assert(prog->inst[i].arg == prog_a_equals_a_plus_10[i].arg);
    }
    program_delete(&prog);

    // Programme 3 (a = |a|)
    prog = file_bin_to_program(
    PROJECT_ROOT "/tests/testdata/test_interpreter/test_file_bin_to_program/test_a_equals_abs_of_a.bin"
    );
    assert(prog != NULL);
    Instruction prog_a_equals_abs_of_a[16] = {
        {OP_LOAD_FROM, 0},
        {OP_INCR, 0},
        {OP_STORE_TO, 1},
        {OP_LOAD_FROM, 0},
        {OP_INCR, 0},
        {OP_JZ, 7},
        {OP_JUMP, 15},
        {OP_LOAD_FROM, 0},
        {OP_INCR, 0},
        {OP_INCR, 0},
        {OP_STORE_TO, 0},
        {OP_LOAD_FROM, 1},
        {OP_INCR, 0},
        {OP_STORE_TO, 1},
        {OP_JZ, 7},
        {OP_HALT, 0}
    };
    for (size_t i = 0; i < prog->size; ++i) {
        assert(prog->inst[i].op == prog_a_equals_abs_of_a[i].op);
        assert(prog->inst[i].arg == prog_a_equals_abs_of_a[i].arg);
    }
    program_delete(&prog);
}

int main() {
    test_program_create();
    test_program_delete();
    test_program_interpret();
    test_file_bin_to_program();

    return 0;
}