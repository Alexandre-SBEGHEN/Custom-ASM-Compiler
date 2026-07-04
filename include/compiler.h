/**
 * @file compiler.h
 * @brief Compilateur de code ASM
 *
 * Lecture de fichiers, nettoyage des
 * lignes de code, tokenisation,
 * puis parsing.
 *
 * Ça va pas être de la rigolade
 *
 * @author Alexandre SBEGHEN
 * @date 2026-06-26
 */

#ifndef COMPILER_H
#define COMPILER_H

/**
 * @brief Récupère le contenu d'un fichier dans une string.
 *
 * @param[in] filename Chemin vers le fichier.
 * @return Pointeur vers la string, NULL en cas d'échec.
 *
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire avec free().
 */
char* file_to_string(const char* filename);

/**
 * @brief Retire les commentaires d'un code.
 *
 * Remplace les commentaires (# jusqu'à fin de ligne)
 * par des espaces.
 * N'affecte les # qui servent d'opérande (ex: LOAD #0).
 *
 * @param[in] str Code sur lequel effectuer les modifications
 * @return Pointeur vers la string, NULL en cas d'échec.
 *
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire avec free().
 */
char* code_without_comments(const char* str);



#endif