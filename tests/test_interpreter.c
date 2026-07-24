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
#include "myarray.h"

/**
 * @brief Test de la fonction program_create().
 *
 * @see program_create()
 */
void test_program_create() {
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
void test_program_interpret() {
    Machine* mac = machine_create(2);
    Program* prog = program_create(16);

    // Programme 1 (a = 1)

    // Programme 2 (a = a + 10)

    // Programme 3 (a = |a|)

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
void test_file_bin_to_program() {
    Program* prog;

    // Programme 1 (a = 1)

    // Programme 2 (a = a + 10)

    // Programme 3 (a = |a|)

}

int main() {
    test_program_create();
    // test_program_interpret();
    // test_file_bin_to_program();

    return 0;
}