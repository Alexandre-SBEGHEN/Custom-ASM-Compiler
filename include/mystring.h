/**
 * @file mystring.h
 * @brief Création d'un type string dynamique.
 * @author Alexandre SBEGHEN
 * @date 2026-07-23
 * @version 1.0.0
 */

#ifndef MYSTRING_H
#define MYSTRING_H
#include <stddef.h>

/* --- Constantes ---------------------------------------------------------- */

extern const size_t STRING_INIT_CAPACITY;

/* --- Alias --------------------------------------------------------------- */

/**
 * @brief Typedef custom de string.
 *
 * Théoriquement identique à char*.
 *
 * Sémantiquement différent, car en paramètre de fonction, un char*
 * autorise les strings natifs, un string non.
 */
#define string char*

typedef struct StringHeaderStruct StringHeader;

/* --- Structs ------------------------------------------------------------- */

/**
 * @brief Structure du header d'une string dynamique.
 */
struct StringHeaderStruct {
    size_t capacity; /**< Nb max de caractères avant qu'une réallocation soit nécessaire */
    size_t length; /**< Nb de caractères que la string contient */
};

/* --- Fonctions ----------------------------------------------------------- */

/**
 * @brief Crée une string dynamique.
 *
 * @param[in] init Valeur d'initialisation.
 * @return Pointeur vers le début de la chaîne.
 *
 * @note Penser à libérer la mémoire
 * après utilisation.
 *
 * @see string_delete().
 */
string string_create(const char* init);

/**
 * @brief Cette fonction permet d'obtenir la
 * capacité d'une string.
 *
 * @param[in] str Pointeur vers la string.
 * @return La capacité de la string.
 *
 */
size_t string_capacity(const string str);

/**
 * @brief Cette fonction permet d'obtenir la
 * longueur d'une string.
 *
 * Utilise le header plutôt qu'un parcourt
 * caractère par caractère, permettant
 * une complexité O(1).
 *
 * @param[in] str Pointeur vers la string.
 * @return La longueur de la string.
 *
 * @note Cette fonction ne compte PAS le caractère
 * de fin '\0'.
 */
size_t string_length(const string str);

/**
 * @brief Libère la mémoire allouée pour une string.
 *
 * @param[out] str Pointeur vers la string à libérer.
 */
void string_delete(string str);

/**
 * @brief Compare deux string pour savoir si la première
 * est inférieure, égale, ou supérieure à la seconde.
 *
 * @param[in] first Pointeur vers la première string à comparer.
 * @param[in] second Pointeur vers la seconde string à comparer.
 * @return Résultat de la comparaison
 */
int string_compare(const string first, const string second);

/**
 * @brief Compare deux strings entre elles
 * pour savoir si elles sont égales.
 *
 * @param[in] first Pointeur vers la première string à comparer.
 * @param[in] second Pointeur vers la seconde string à comparer.
 * @return Résultat de la comparaison.
 */
int string_equals(const char* first, const char* second);

/**
 * @brief Met tous les caractères d'une string en majuscule.
 *
 * @param[out] str Pointeur vers la string à convertir.
 * @return Pointeur vers la string en majuscule.
 *
 * @note Cette fonction utilise toupper() qui ne supporte
 * uniquemement les caractères ASCII.
 *
 * @see toupper()
 */
string string_toupper(char* str);

/**
 * @brief Met tous les caractères d'une string en minuscule.
 *
 * @param[out] str Pointeur vers la string à convertir.
 * @return Pointeur vers la string en minuscule.
 *
 * @note Cette fonction utilise tolower() qui ne supporte
 * uniquemement les caractères ASCII.
 *
 * @see tolower
 */
string string_tolower(char* str);

/**
 * Rogne une string en retirant des caractères à gauche
 * et à droite.
 *
 * @param[in] str Pointeur vers la string à rogner.
 * @param[in] left Nombre de caractères à retirer à gauche.
 * @param[in] right Nombre de caractères à retirer à droite.
 * @return Pointeur vers la string rognée.
 */
string string_crop(string str, size_t left, size_t right);

/**
 * @brief Rajoute à une string déjà existante le contenu d'une seconde.
 *
 * @param[out] dest Pointeur vers la string à laquelle doit être concaténé le contenu de l'autre.
 * @param[in] source Pointeur vers la string à concaténer.
 * @return Pointeur vers la string concaténée.
 */
string string_concat(string* dest, const char* source);

/**
 * @brief Vide une string pour ne laisser que le caractère '\0'.
 *
 * @param[out] str Pointeur vers la string à vider.
 * @return Pointeur vers la string vidée.
 */
string string_clear(string str);

#endif
