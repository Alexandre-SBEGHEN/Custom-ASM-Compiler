/**
 * @file test_machine.c
 * @brief Test des implémentations de machine.h / machine.c
 * @author Alexandre SBEGHEN
 * @date 2026-06-24
 */

#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "machine.h"

/**
 * @brief Test de la fonction register_create().
 *
 * @see register_create()
 */
void test_register_create() {
    Register* reg = register_create();

    assert(reg != NULL);
    assert(reg->val == 0);

    register_delete(reg);
}

/**
 * @brief Test de la fonction memory_create().
 *
 * @see memory_create()
 */
void test_memory_create() {
    for (size_t size = 1; size < 128; ++size) {
        Memory* mem = memory_create(size);

        assert(mem != NULL);
        assert(mem->size == size);
        for (size_t i = 0; i < size; i++)
            assert(mem->data[i] == 0);

        memory_delete(mem);
    }
}

/**
 * @brief Test de la fonction machine_create().
 *
 * @see machine_create()
 */
void test_machine_create() {
    Machine* mac = machine_create(1);

    assert(mac != NULL);
    assert(mac->mem != NULL);
    assert(mac->reg != NULL);

    machine_delete(mac);
}

/**
 * @brief Test de la fonction ram_load_direct().
 *
 * @see ram_load_direct()
 */
void test_ram_load_direct() {
    Machine* mac = machine_create(1);

    ram_load_direct(mac, -1);
    assert(mac->reg->val == -1);
    ram_load_direct(mac, 1);
    assert(mac->reg->val == 1);
    ram_load_direct(mac, 3);
    assert(mac->reg->val == 3);
    ram_load_direct(mac, 2);
    assert(mac->reg->val == 2);
    ram_load_direct(mac, -67);
    assert(mac->reg->val == -67);

    machine_delete(mac);
}

/**
 * @brief Test de la fonction ram_load_from().
 *
 * @see ram_load_from()
 */
void test_ram_load_from() {
    Machine* mac = machine_create(5);

    const int32_t testval[] = {0, 1, 3, 2, -5};
    memcpy(mac->mem->data, testval, sizeof(testval));

    for (size_t i = 0; i < 5; ++i) {
        ram_load_from(mac, i);
        assert(mac->reg->val == testval[i]);
    }

    machine_delete(mac);
}

/**
 * @brief Test de la fonction ram_store_to().
 *
 * @see ram_store_to()
 */
void test_ram_store_to() {
    Machine* mac = machine_create(5);

    const int32_t testval[] = {0, 1, 3, 2, -5};

    for (size_t i = 0; i < 5; ++i) {
        mac->reg->val = testval[i];
        ram_store_to(mac, i);
        assert(mac->mem->data[i] == testval[i]);
    }

    machine_delete(mac);
}

/**
 * @brief Test de la fonction ram_increment().
 *
 * @see register_increment()
 */
void test_ram_increment() {
    Machine* mac = machine_create(1);

    ram_increment(mac);
    assert(mac->reg->val == 1);
    ram_increment(mac);
    ram_increment(mac);
    assert(mac->reg->val == 3);
    ram_increment(mac);
    ram_increment(mac);
    ram_increment(mac);
    ram_increment(mac);
    assert(mac->reg->val == 7);
    ram_increment(mac);
    ram_increment(mac);
    ram_increment(mac);
    assert(mac->reg->val == 10);

    machine_delete(mac);
}

/**
 * @brief Test de la fonction ram_decrement().
 *
 * @see register_decrement()
 */
void test_ram_decrement() {
    Machine* mac = machine_create(1);

    ram_decrement(mac);
    assert(mac->reg->val == -1);
    ram_decrement(mac);
    ram_decrement(mac);
    assert(mac->reg->val == -3);
    ram_decrement(mac);
    ram_decrement(mac);
    ram_decrement(mac);
    ram_decrement(mac);
    assert(mac->reg->val == -7);
    ram_decrement(mac);
    ram_decrement(mac);
    ram_decrement(mac);
    assert(mac->reg->val == -10);

    machine_delete(mac);
}

int main() {
    test_register_create();
    test_memory_create();
    test_machine_create();
    test_ram_load_direct();
    test_ram_load_from();
    test_ram_store_to();
    test_ram_increment();
    test_ram_decrement();

    return 0;
}