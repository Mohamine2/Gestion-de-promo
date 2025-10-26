/**
 * @file top_students.c
 * @brief Implémentation des fonctions permettant de trier et sélectionner les meilleurs étudiants d’une promotion.
 *
 * Ce fichier contient l’implémentation des algorithmes utilisés pour :
 * - trier les étudiants selon leur moyenne générale (`compareStudents`, `getTopTenStudents`) ;
 * - déterminer les meilleurs étudiants d’un cours donné (`getTopThreeStudentsCourse`).
 *
 * Ces fonctions s’appuient sur le tri rapide de la bibliothèque standard (`qsort`)
 * et utilisent des copies de tableaux pour ne pas modifier l’ordre d’origine des étudiants.
 *
 * @see top_students.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./struct.h"
#include "./file_gestion.h"

/**
 * @brief Compare deux étudiants selon leur moyenne générale (utilisée avec qsort).
 */
int compareStudents(const void* a, const void* b) {
    Student* s1 = *(Student**)a;
    Student* s2 = *(Student**)b;

    if (s1->general_average < s2->general_average)
        return 1;
    if (s1->general_average > s2->general_average)
        return -1;
    return 0;
}

/**
 * @brief Retourne les 10 meilleurs étudiants d’une promotion.
 */
Student** getTopTenStudents(Prom* promo, int* count) {
    if (promo == NULL || promo->num_students <= 0)
        return NULL;

    *count = (promo->num_students < 10) ? promo->num_students : 10;

    Student** copy = malloc(sizeof(Student*) * promo->num_students);
    if (copy == NULL)
        return NULL;

    memcpy(copy, promo->students, sizeof(Student*) * promo->num_students);
    qsort(copy, promo->num_students, sizeof(Student*), compareStudents);

    Student** top_ten = malloc(sizeof(Student*) * (*count));
    if (top_ten == NULL) {
        free(copy);
        return NULL;
    }

    for (int i = 0; i < *count; i++) {
        top_ten[i] = copy[i];
    }

    free(copy);
    return top_ten;
}

/**
 * @struct StudentCourseAvg
 * @brief Structure interne associant un étudiant à sa moyenne dans un cours donné.
 */
typedef struct {
    Student* s;      /**< Pointeur vers l’étudiant */
    float moyenne;   /**< Moyenne obtenue dans le cours */
} StudentCourseAvg;

/**
 * @brief Compare deux structures `StudentCourseAvg` selon leur moyenne.
 */
int compareCourseAvg(const void* a, const void* b) {
    const StudentCourseAvg* sa = (const StudentCourseAvg*)a;
    const StudentCourseAvg* sb = (const StudentCourseAvg*)b;
    if (sa->moyenne < sb->moyenne) return 1;
    if (sa->moyenne > sb->moyenne) return -1;
    return 0;
}

/**
 * @brief Retourne les 3 meilleurs étudiants d’un cours spécifique.
 */
Student** getTopThreeStudentsCourse(Prom* promo, const char* course_name, int* top_count) {
    if (promo == NULL || course_name == NULL || promo->num_students == 0) {
        *top_count = 0;
        return NULL;
    }

    // Étape 1 : compter les étudiants concernés
    int count = 0;
    for (int i = 0; i < promo->num_students; i++) {
        Student* s = promo->students[i];
        for (int j = 0; j < s->num_courses; j++) {
            if (strcmp(s->courses[j]->course_name, course_name) == 0) {
                count++;
                break;
            }
        }
    }

    if (count == 0) {
        *top_count = 0;
        printf("This course doesn't exist\n");
        return NULL;
    }

    // Étape 2 : stocker les moyennes de chaque étudiant dans la matière
    StudentCourseAvg* savg = malloc(sizeof(StudentCourseAvg) * count);
    if (savg == NULL) {
        *top_count = 0;
        return NULL;
    }

    int idx = 0;
    for (int i = 0; i < promo->num_students; i++) {
        Student* s = promo->students[i];
        for (int j = 0; j < s->num_courses; j++) {
            Course* c = s->courses[j];
            if (strcmp(c->course_name, course_name) == 0) {
                savg[idx].s = s;
                savg[idx].moyenne = c->average;
                idx++;
                break;
            }
        }
    }

    // Étape 3 : trier les étudiants par moyenne décroissante
    qsort(savg, count, sizeof(StudentCourseAvg), compareCourseAvg);

    // Étape 4 : sélectionner les 3 meilleurs (ou moins)
    *top_count = (count < 3) ? count : 3;
    Student** top_three = malloc(sizeof(Student*) * (*top_count));
    if (top_three == NULL) {
        free(savg);
        *top_count = 0;
        return NULL;
    }

    for (int i = 0; i < *top_count; i++) {
        top_three[i] = savg[i].s;
    }

    free(savg);
    return top_three;
}
