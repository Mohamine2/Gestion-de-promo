/**
 * @file struct_functions.c
 * @brief Implémentation des fonctions de gestion mémoire pour les structures de données.
 *
 * Ce fichier contient les fonctions permettant :
 * - la création et la suppression de notes (`Grades`),
 * - la création et la suppression de cours (`Course`),
 * - la création et la suppression d’étudiants (`Student`),
 * - la création et la suppression de promotions (`Prom`).
 *
 * Ces fonctions assurent une gestion sûre de la mémoire dynamique
 * et facilitent la manipulation des structures dans le reste du programme.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./struct.h"

/**
 * @brief Crée une structure `Grades` et initialise son tableau de notes.
 */
Grades* createGrade(int size) {
    Grades* g = malloc(sizeof(Grades));
    if (g == NULL) {
        exit(0);
    }

    g->size = size;
    g->grades_array = NULL;

    if (size > 0) {
        g->grades_array = calloc(size, sizeof(float));
        if (g->grades_array == NULL) {
            free(g);
            exit(1);
        }
    }
    return g;
}

/**
 * @brief Libère la mémoire associée à une structure `Grades`.
 */
void destroyGrade(Grades* g) {
    if (g == NULL)
        return;
    free(g->grades_array);
    free(g);
}

/**
 * @brief Crée une structure `Course` avec un nom, un coefficient et un tableau de notes vide.
 */
Course* createCourse(char* name, float coeff) {
    Course* c = malloc(sizeof(Course));
    if (c == NULL) {
        exit(1);
    }

    c->course_name = malloc(strlen(name) + 1);
    if (c->course_name == NULL) {
        free(c);
        exit(1);
    }
    strcpy(c->course_name, name);

    c->coeff = coeff;
    c->grades = createGrade(0);
    c->average = 0.0f;

    return c;
}

/**
 * @brief Libère la mémoire associée à un cours.
 */
void destroyCourse(Course* c) {
    if (c == NULL)
        return;
    destroyGrade(c->grades);
    free(c->course_name);
    free(c);
}

/**
 * @brief Crée un étudiant avec ses informations personnelles et initialise ses cours.
 */
Student* createStudent(int id, char* first_name, char* last_name, int age, int num_courses) {
    if (first_name == NULL || last_name == NULL || num_courses < 0)
        return NULL;

    Student* s = malloc(sizeof(Student));
    if (s == NULL)
        return NULL;

    s->first_name = malloc(strlen(first_name) + 1);
    if (s->first_name == NULL) {
        free(s);
        return NULL;
    }
    strcpy(s->first_name, first_name);

    s->last_name = malloc(strlen(last_name) + 1);
    if (s->last_name == NULL) {
        free(s->first_name);
        free(s);
        return NULL;
    }
    strcpy(s->last_name, last_name);

    s->student_id = id;
    s->age = age;
    s->num_courses = num_courses;
    s->general_average = 0.0f;
    s->courses = NULL;

    if (num_courses > 0) {
        s->courses = calloc(num_courses, sizeof(Course*));
        if (s->courses == NULL) {
            free(s->first_name);
            free(s->last_name);
            free(s);
            return NULL;
        }
    }

    return s;
}

/**
 * @brief Libère la mémoire associée à un étudiant et à ses cours.
 */
void destroyStudent(Student* s) {
    if (s == NULL)
        return;

    if (s->courses != NULL) {
        for (int i = 0; i < s->num_courses; i++)
            destroyCourse(s->courses[i]);
        free(s->courses);
    }

    free(s->first_name);
    free(s->last_name);
    free(s);
}

/**
 * @brief Crée une structure `Prom` représentant une promotion.
 */
Prom* createProm(int initial_capacity, int nb_students) {
    if (initial_capacity < 0)
        return NULL;

    Prom* p = malloc(sizeof(Prom));
    if (p == NULL)
        return NULL;

    p->num_students = nb_students;
    p->capacity = initial_capacity;
    p->students = NULL;

    if (initial_capacity > 0) {
        p->students = calloc(initial_capacity, sizeof(Student*));
        if (p->students == NULL) {
            free(p);
            return NULL;
        }
    }

    return p;
}

/**
 * @brief Libère la mémoire associée à une promotion et à tous ses étudiants.
 */
void destroyProm(Prom* p) {
    if (p == NULL)
        return;

    if (p->students != NULL) {
        for (int i = 0; i < p->num_students; i++)
            destroyStudent(p->students[i]);
        free(p->students);
    }

    free(p);
}
