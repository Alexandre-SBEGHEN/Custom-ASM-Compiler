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
 * @brief Découpe une string en tableau de lignes.
 *
 * Le séparateur est donc le caractère Retour à la ligne '\\n'.
 *
 * @param[in] str La string à découper.
 * @return Pointeur vers le tableau de strings, NULL en cas d'échec.
 *
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire avec free().
 */
char** string_to_array(const char* str);

/**
 * @brief Retire les caractères inutiles d'une ligne de code.
 *
 * Les modifications suivantes sont apportées :
 * - Les lignes commençant par # sont remplacées par '\\0' ;
 * - Les espaces/tabulations en début de ligne sont supprimées ;
 * - Les caractères après un point virgule ';' sont remplacés par '\\0'.
 *
 * @param[in, out] str Ligne à nettoyer.
 * @return Pointeur vers le tableau de strings, NULL en cas d'échec.
 *
 * @note Cette fonction effectue une allocation dynamique
 * de mémoire, penser à libérer la mémoire avec free().
 */
char* string_clean(char* str);

/**
 * Crée une liste de tokens sous la forme de string.
 *
 * Etape nécessaire avant la vraie tokénisation.
 *
 * @param[in] array Le tableau de lignes.
 * @return Pointeur vers le nouveau tableau, NULL en cas d'échec.
 */
char** array_to_string_tokens(const char** array);

#endif