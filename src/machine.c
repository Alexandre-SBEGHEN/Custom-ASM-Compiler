/**
 * @file machine.c
 * @brief Implémentation de la machine RAM et ses opérations.
 * @author Alexandre SBEGHEN
 * @date 2026-06-24
 */

#include "machine.h"
#include "myarray.h"

/** Création dynamique d'une structure registre Register */
Register* register_create() {
    Register* reg;

    // Allocation, return NULL si échec
    if ((reg = malloc(sizeof(Register))) == NULL)
        return NULL;

    // Valeur 0 par défaut
    reg->val = 0;

    return reg;
}

/** Libération de mémoire d'une structure Register */
void register_delete(Register* reg) {
    free(reg);
}

/** Création dynamique d'une structure mémoire Memory */
Memory* memory_create(const size_t size) {
    Memory* mem;

    // Allocation, return NULL si échec
    if ((mem = malloc(sizeof(Memory))) == NULL)
        return NULL;


    // Allocation des données, echec -> free reg puis return NULL
    mem->data = array_create(int32_t);
    for (size_t i = 0; i < size; ++i)
        array_push(mem->data, 0);

    return mem;
}

/** Libération de mémoire d'une structure Memory */
void memory_delete(Memory* mem) {
    array_delete(mem->data);
    free(mem);
}

/** Création dynamique d'une structure machine Machine */
Machine* machine_create(const size_t memsize) {
    Machine* mac;

    if ((mac = malloc(sizeof(Machine))) == NULL)
        return NULL;

    if ((mac->mem = memory_create(memsize)) == NULL) {
        free(mac);
        return NULL;
    }

    if ((mac->reg = register_create()) == NULL) {
        memory_delete(mac->mem);
        free(mac);
        return NULL;
    }

    return mac;
}

/** Libération de mémoire d'une structure Machine */
void machine_delete(Machine* mac) {
    if (mac == NULL)
        return;

    memory_delete(mac->mem);
    register_delete(mac->reg);
    free(mac);
}

/** Chargement direct du registre */
void ram_load_direct(Machine* mac, int32_t val) {
    if (mac == NULL)
        return;

    mac->reg->val = val;
}

/** Chargement du registre depuis la mémoire */
void ram_load_from(Machine* mac, size_t index) {
    if (mac == NULL)
        return;

    mac->reg->val = mac->mem->data[index];
}

/** Rangement du registre vers la mémoire */
void ram_store_to(Machine* mac, size_t index) {
    if (mac == NULL)
        return;

    mac->mem->data[index] = mac->reg->val;
}

/** Incrémentation du registre */
void ram_increment(Machine* mac) {
    if (mac == NULL)
        return;

    mac->reg->val += 1;
}

/** Décrémentation du registre */
void ram_decrement(Machine* mac) {
    if (mac == NULL)
        return;

    mac->reg->val -= 1;
}