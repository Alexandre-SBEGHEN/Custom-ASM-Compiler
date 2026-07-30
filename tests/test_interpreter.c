/**
 * @file test_interpreter.c
 * @brief Test des implémentations de interpreter.h / interpreter.c
 * @author Alexandre SBEGHEN
 * @date 2026-06-25
 */

#include <assert.h>
#include <stddef.h>
#include "machine.h"
#include "interpreter.h"
#include "myarray.h"

/**
 * @brief Test de la fonction program_create().
 *
 * @see program_create()
 */
void test_program_create(void) {
    Program* prog = program_create();

    assert(prog != NULL);
    assert(prog->inst != NULL);

    program_delete(prog);
}

/**
 * @brief Test de la fonction program_interpret().
 *
 * @see program_interpret()
 */
void test_program_interpret(void) {
    Machine* mac = machine_create(2);
    Program* prog = program_create();

    // Programme 1 (a = 1)
    Instruction insts_a_equals_1[] = {
        {OP_LOAD_DIRECT, 1},
        {OP_STORE_TO, 0},
        {OP_HALT, 0}
    };
    size_t insts_len = sizeof(insts_a_equals_1) / sizeof(Instruction);
    for (size_t i = 0; i < insts_len; ++i)
        array_push(prog->inst, insts_a_equals_1[i]);

    program_interpret(prog, mac);
    assert(mac->mem->data[0] == 1);

    program_delete(prog);
    prog = program_create();


    // Programme 2 (a = a + 10)
    Instruction insts_a_equals_a_plus_10[] = {
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
    insts_len = sizeof(insts_a_equals_a_plus_10) / sizeof(Instruction);
    for (size_t i = 0; i < insts_len; ++i)
        array_push(prog->inst, insts_a_equals_a_plus_10[i]);

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

    program_delete(prog);
    prog = program_create();


    // Programme 3 (a = |a|)
    Instruction insts_a_equals_abs_of_a[] = {
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
    insts_len = sizeof(insts_a_equals_abs_of_a) / sizeof(Instruction);
    for (size_t i = 0; i < insts_len; ++i)
        array_push(prog->inst, insts_a_equals_abs_of_a[i]);

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

    program_delete(prog);
    machine_delete(mac);
}

/**
 * @brief Test de la fonction file_bin_to_program().
 *
 * Lit plusieurs fichiers et vérifie que la structure générée
 * en résultat est la bonne.
 *
 * @see file_bin_to_program()
 */
void test_file_bin_to_program(void) {
    Program* prog;

    // --- Programme 1 : a = 1
    {
        Instruction insts_a_equals_1[] = {
            {OP_LOAD_DIRECT, 1},
            {OP_STORE_TO, 0},
            {OP_HALT, 0}
        };
        prog = file_bin_to_program(
            PROJECT_ROOT "/tests/testdata/test_interpreter/test_file_bin_to_program/test_a_equals_1.bin"
        );
        assert(prog != NULL);
        for (size_t i = 0; i < array_size(prog->inst); ++i) {
            assert(prog->inst[i].op == insts_a_equals_1[i].op);
            assert(prog->inst[i].arg == insts_a_equals_1[i].arg);
        }

        program_delete(prog);
    }

    // --- Programme 2 : a = a + 10
    {
        Instruction insts_a_equals_a_plus_10[] = {
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
        prog = file_bin_to_program(
            PROJECT_ROOT "/tests/testdata/test_interpreter/test_file_bin_to_program/test_a_equals_a_plus_10.bin"
        );
        assert(prog != NULL);
        for (size_t i = 0; i < array_size(prog->inst); ++i) {
            assert(prog->inst[i].op == insts_a_equals_a_plus_10[i].op);
            assert(prog->inst[i].arg == insts_a_equals_a_plus_10[i].arg);
        }

        program_delete(prog);
    }

    // --- Programme 3 : a = |a|
    {
        Instruction insts_a_equals_abs_of_a[] = {
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
        prog = file_bin_to_program(
            PROJECT_ROOT "/tests/testdata/test_interpreter/test_file_bin_to_program/test_a_equals_abs_of_a.bin"
        );
        assert(prog != NULL);
        for (size_t i = 0; i < array_size(prog->inst); ++i) {
            assert(prog->inst[i].op == insts_a_equals_abs_of_a[i].op);
            assert(prog->inst[i].arg == insts_a_equals_abs_of_a[i].arg);
        }

        program_delete(prog);
    }
}

int main(void) {
    test_program_create();
    test_program_interpret();
    test_file_bin_to_program();

    return 0;
}