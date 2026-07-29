/**
 * @file myarray.c
 * @brief Définition des constantes et des fonctions de manipulation des tableaux.
 * @author Alexandre SBEGHEN
 * @date 2026-07-23
 */

#include "myarray.h"
#include <stdlib.h>

const size_t ARRAY_INIT_CAPACITY = 4;

/** Allocation d'un tableau dynamique */
void* arr_init(const size_t itemsize, const size_t capacity) {
    const size_t allocsize = itemsize * capacity + sizeof(ArrayHeader);
    ArrayHeader* h = calloc(1, allocsize);

    if (!h)
        return NULL;

    h->capacity = capacity;
    h->size = 0;
    h->itemsize = itemsize;
    return h+1;
}

/** Obtenir la capacité d'un tableau */
size_t array_capacity(const void* array) {
    return ((ArrayHeader*)array - 1)->capacity;
}

/** Obtenir la taille d'un tableau */
size_t array_size(const void* array) {
    return ((ArrayHeader*)array - 1)->size;
}