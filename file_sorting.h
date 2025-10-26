/**
 * @file file_sorting.h
 * @brief Fonctions de tri et de sélection des meilleurs étudiants d’une promotion.
 *
 * Ce module regroupe les fonctions permettant de :
 * - Trier les étudiants d’une promotion selon leur moyenne générale.
 * - Obtenir les 10 meilleurs étudiants de la promotion.
 * - Obtenir les 3 meilleurs étudiants d’un cours donné.
 *
 * Il repose sur la bibliothèque standard C (qsort, memcpy) et les structures définies
 * dans `struct.h`.
 *
 * @author  Mohamed
 * @date    26 octobre 2025
 * @version 1.0
 */

#ifndef TOP_STUDENTS_H
#define TOP_STUDENTS_H

#include "./struct.h"

/**
 * @brief Fonction de comparaison utilisée pour trier les étudiants selon leur moyenne générale.
 * 
 * Cette fonction est utilisée avec `qsort()`.  
 * Elle compare deux pointeurs vers des structures `Student` et renvoie :
 * - 1 si la moyenne du premier est inférieure à celle du second ;
 * - -1 si elle est supérieure ;
 * - 0 si elles sont égales.
 * 
 * @param a Pointeur vers un pointeur de `Student` (const void* requis par qsort).
 * @param b Pointeur vers un pointeur de `Student` (const void* requis par qsort).
 * @return Un entier indiquant l’ordre de tri.
 */
int compareStudents(const void* a, const void* b);

/**
 * @brief Retourne les 10 meilleurs étudiants d’une promotion, triés par moyenne générale décroissante.
 *
 * Si la promotion contient moins de 10 étudiants, tous sont retournés.  
 * La fonction alloue dynamiquement un tableau de pointeurs vers les étudiants triés.
 *
 * @param promo Pointeur vers la structure `Prom` représentant la promotion.
 * @param count Pointeur vers un entier où sera stocké le nombre d’étudiants retournés.
 * @return Un tableau dynamique de pointeurs vers les étudiants triés, ou NULL en cas d’erreur.
 *
 * @note Le tableau retourné doit être libéré avec `free()` par l’appelant.
 */
Student** getTopTenStudents(Prom* promo, int* count);

/**
 * @brief Retourne les 3 meilleurs étudiants d’un cours donné.
 *
 * La fonction recherche les étudiants inscrits au cours spécifié, trie leurs moyennes
 * pour ce cours dans l’ordre décroissant, puis renvoie les 3 meilleurs (ou moins s’il y en a moins de 3).
 *
 * @param promo        Pointeur vers la promotion complète.
 * @param course_name  Nom du cours concerné.
 * @param top_count    Pointeur vers un entier où sera stocké le nombre d’étudiants trouvés (≤ 3).
 * @return Un tableau dynamique de pointeurs vers les meilleurs étudiants du cours, ou NULL si aucun n’est trouvé.
 *
 * @note Le tableau retourné doit être libéré avec `free()` par l’appelant.
 */
Student** getTopThreeStudentsCourse(Prom* promo, const char* course_name, int* top_count);

#endif // TOP_STUDENTS_H
