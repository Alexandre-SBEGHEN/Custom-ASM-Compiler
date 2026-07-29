/**
 * @file mystring.c
 * @brief Implémentation des manipulations de strings.
 * @author Alexandre SBEGHEN
 * @date 2026-07-23
 */

#include "mystring.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const size_t STRING_INIT_CAPACITY = 8;

/** Création dynamique d'une string */
string string_create(const char* init) {
    size_t baselen = strlen(init) + 1;

    size_t capacity = STRING_INIT_CAPACITY;
    while (capacity < baselen)
        capacity *= 2;

    size_t allocsize = sizeof(StringHeader) + capacity * sizeof(char);
    StringHeader* h = calloc(1, allocsize);

    if (!h)
        return NULL;

    h->capacity = capacity;
    h->length = baselen - 1;

    string str = (string)(h+1);
    strcpy(str, init);

    return str;
}

/** Obtenir la capacité d'une string */
size_t string_capacity(const string str) {
    return ((StringHeader*)str-1)->capacity;
}

/** Obtenir la longueur d'une string */
size_t string_length(const string str) {
    return ((StringHeader*)str-1)->length;
}

/** Libérer la mémoire allouée pour une string */
void string_delete(string str) {
    free((StringHeader*)str-1);
}

/** Comparer deux strings */
int string_compare(const string first, const string second) {
    return strcmp(first, second);
}

/** Vérifier si deux strings sont égales */
int string_equals(const char* first, const char* second) {
    return strcmp(first, second) == 0;
}

/** Mettre une string en majuscule */
string string_toupper(char* str) {
    for (size_t i = 0; str[i] != '\0'; ++i)
        str[i] = (char)toupper(str[i]);

    return str;
}

/** Mettre une string en minuscule */
string string_tolower(char* str) {
    for (size_t i = 0; str[i] != '\0'; ++i)
        str[i] = (char)tolower(str[i]);

    return str;
}

/** Rogner une string */
string string_crop(string str, const size_t left, const size_t right) {
    StringHeader* h = (StringHeader*)str - 1;

    // Vide
    if (left + right >= h->length) {
        h->length = 0;
        str[0] = '\0';
        return str;
    }

    size_t newlen = h->length - left - right;
    memmove(str, str + left, newlen);
    h->length = newlen;
    str[h->length] = '\0';

    return str;
}

/** Concaténation de strings */
string string_concat(string* dest, const char* source) {
    StringHeader* h = (StringHeader*)(*dest) - 1;

    for (size_t i = 0; source[i] != '\0'; ++i) {
        if (h->length >= h->capacity - 1) {
            size_t newcapacity = h->capacity * 2;
            size_t newsize = sizeof(StringHeader) + newcapacity * sizeof(char);
            StringHeader* newh = realloc(h, newsize);
            if (!newh)
                return *dest;
            newh->capacity = newcapacity;
            (*dest) = (char*)(newh+1);
            h = newh;
        }

        (*dest)[h->length] = source[i];
        h->length += 1;
    }
    (*dest)[h->length] = '\0';

    return *dest;
}

/** Vider une string */
string string_clear(string str) {
    StringHeader* h = (StringHeader*)str - 1;
    h->length = 0;
    str[0] = '\0';

    return str;
}