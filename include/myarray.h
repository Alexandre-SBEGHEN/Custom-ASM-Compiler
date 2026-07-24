/**
 * @file myarray.h
 * @brief Interface et macros des tableaux dynamiques.
 * @author Alexandre SBEGHEN
 * @date 2026-07-23
 * @version 1.0.0
 */

#ifndef MYARRAY_H
#define MYARRAY_H

#include <stdlib.h>

/* --- Structs ------------------------------------------------------------- */

/**
 * @brief Structure d'un header de tableau dynamique.
 *
 * Se trouve en en-tête de chaque tableau dynamique.
 */
struct ArrayHeaderStruct {
    size_t capacity; /**< Nb max d'éléments avant qu'une réallocation soit nécessaire */
    size_t size; /**< Nb d'éléments que le tableau contient */
    size_t itemsize; /**< Taille d'un élément du tableau */
};

/* --- Alias --------------------------------------------------------------- */

typedef struct ArrayHeaderStruct ArrayHeader;

/* --- Constantes ---------------------------------------------------------- */

extern const size_t ARRAY_INIT_CAPACITY;

/* --- Fonctions ----------------------------------------------------------- */

/**
 * @brief Crée un tableau dynamique.
 *
 * Cette macro effectue un cast de la structure allouée
 * par arr_init() qui vaut void* par défaut, vers le
 * type de données du tableau.
 *
 * La capacité par défaut est définie par une constante.
 *
 * @param[in] type Le type de données que contient le tableau.
 *
 * @note Il ne s'agit pas d'une vraie fonction, mais
 * d'une macro-définition qui agit comme telle.
 * @note Penser à libérer la mémoire après utilisation.
 *
 * @see arr_init()
 * @see array_delete()
 */
#define array_create(type) (type*)arr_init(sizeof(type), ARRAY_INIT_CAPACITY)

/**
 * @brief Pousse une valeur dans un tableau.
 *
 * Si le tableau est déjà rempli, une réallocation
 * s'effectue et sa capacité double.
 *
 * @param[out] array Le tableau.
 * @param[in] val La valeur à pousser dans le tableau.
 *
 * @note Il ne s'agit pas d'une vraie fonction, mais
 * d'une macro-définition qui agit comme telle.
 */
#define array_push(array, val) do { \
    ArrayHeader* h = (ArrayHeader*)array - 1; \
    if (h->size >= h->capacity) { \
        size_t newcapacity = h->capacity * 2; \
        size_t newsize = newcapacity * h->itemsize + sizeof(ArrayHeader); \
        ArrayHeader* newh = realloc(h, newsize); \
        if (newh) { \
            newh->capacity = newcapacity; \
            array = (void*)(newh+1); \
            h = newh; \
        } \
    } \
    array[h->size] = val; \
    h->size += 1; \
} while (0) \

/**
 * @brief Retire le dernier élément d'un tableau.
 *
 * Ne fait rien si le tableau est déjà vide.
 *
 * @param[out] array Le tableau.
 *
 * @note Il ne s'agit pas d'une vraie fonction, mais
 * d'une macro-définition qui agit comme telle.
 */
#define array_remove_last(array) do { \
    ArrayHeader* h = (ArrayHeader*)array - 1; \
    if (h->size == 0) \
        break; \
    h->size -= 1; \
} while (0)

/**
 * @brief Retire le dernier élément d'un tableau,
 * puis sauvegarde sa valeur à une adresse.
 *
 * Ne fait rien si le tableau est déjà vide.
 *
 * @warning S'assurer que la destination vaut quelque
 * chose de réel et surtout pas NULL, sinon cela
 * causera un plantage du programme.
 *
 * @param[out] array Le tableau.
 * @param[out] dest Adresse de rangement de la valeur.
 *
 * @note Il ne s'agit pas d'une vraie fonction, mais
 * d'une macro-définition qui agit comme telle.
 */
#define array_pop(array, dest) do { \
    ArrayHeader* h = (ArrayHeader*)array - 1; \
    if (h->size == 0) \
        break; \
    h->size -= 1; \
    *dest = array[h->size]; \
} while (0)

/**
 * @brief Alloue dynamiquement un tableau avec son header.
 *
 * @param[in] itemsize La taille d'un élément du tableau.
 * @param[in] capacity Capacité initiale du tableau
 * @return L'adresse du header + 1, donc du premier élément.
 *
 * @note Cette fonction n'a pas pour vocation à
 * être utilisée directement, elle sert de passage
 * pour la macro-définition array_create().
 *
 * @see array_create()
 */
void* arr_init(size_t itemsize, size_t capacity);

/**
 * @brief Cette fonction permet de connaître la capacité d'un tableau.
 *
 * @param[in] array Pointeur général vers le tableau.
 * @return La capacité du tableau.
 */
size_t array_capacity(const void* array);

/**
 * @brief Cette fonction permet de connaître la taille d'un tableau.
 *
 * @param[in] array Pointeur général vers le tableau.
 * @return La taille du tableau.
 */
size_t array_size(const void* array);

/**
 * @brief Libère la mémoire allouée par un tableau.
 *
 * Utilisation du double pointeur en paramètre pour
 * mettre automatiquement sa valeur à NULL.
 *
 * @param[out] array Adresse du pointeur général vers le tableau.
 *
 * @note Il ne s'agit pas d'une vraie fonction, mais
 * d'une macro-définition qui agit comme telle.
 * @note Si les éléments du tableau sont des pointeurs
 * vers d'autres structures allouées dynamiquement,
 * penser à les libérer en premier.
 */
#define array_delete(array) do { \
    ArrayHeader* h = (ArrayHeader*)array - 1; \
    free(h); \
    array = NULL; \
} while (0)

#endif