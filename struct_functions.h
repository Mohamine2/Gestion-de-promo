/**
 * @file struct_functions.h
 * @brief Fonctions de création et de destruction pour les structures du projet.
 *
 * Ce module fournit un ensemble de fonctions utilitaires pour allouer, initialiser
 * et libérer la mémoire associée aux structures :
 * - `Grades`
 * - `Course`
 * - `Student`
 * - `Prom`
 *
 * Ces fonctions garantissent une gestion propre de la mémoire dynamique et doivent
 * être utilisées à la place des appels directs à `malloc()` ou `free()`.
 *
 * @see struct.h
 */

#ifndef STRUCT_FUNCTIONS_H
#define STRUCT_FUNCTIONS_H

#include "./struct.h"

/**
 * @brief Crée une structure `Grades` et initialise son tableau de notes.
 * 
 * @param size Nombre initial de notes à allouer (0 pour un tableau vide).
 * @return Pointeur vers la structure allouée, ou NULL en cas d’échec.
 */
Grades* createGrade(int size);

/**
 * @brief Libère la mémoire associée à une structure `Grades`.
 * 
 * @param g Pointeur vers la structure à détruire.
 */
void destroyGrade(Grades* g);

/**
 * @brief Crée une structure `Course` et initialise son nom, coefficient et tableau de notes.
 * 
 * @param name   Nom du cours.
 * @param coeff  Coefficient associé au cours.
 * @return Pointeur vers la structure allouée, ou NULL en cas d’erreur.
 */
Course* createCourse(char* name, float coeff);

/**
 * @brief Libère la mémoire associée à un cours.
 * 
 * @param c Pointeur vers le cours à détruire.
 */
void destroyCourse(Course* c);

/**
 * @brief Crée une structure `Student` avec ses informations personnelles et ses cours.
 * 
 * @param id           Identifiant de l’étudiant.
 * @param first_name   Prénom de l’étudiant.
 * @param last_name    Nom de l’étudiant.
 * @param age          Âge de l’étudiant.
 * @param num_courses  Nombre de cours suivis (0 pour aucun).
 * @return Pointeur vers la structure allouée, ou NULL en cas d’échec.
 */
Student* createStudent(int id, char* first_name, char* last_name, int age, int num_courses);

/**
 * @brief Libère toute la mémoire associée à un étudiant (cours, chaînes, structure).
 * 
 * @param s Pointeur vers l’étudiant à détruire.
 */
void destroyStudent(Student* s);

/**
 * @brief Crée une structure `Prom` représentant une promotion d’étudiants.
 * 
 * @param initial_capacity Taille initiale du tableau d’étudiants.
 * @param nb_students      Nombre d’étudiants déjà enregistrés.
 * @return Pointeur vers la structure allouée, ou NULL en cas d’échec.
 */
Prom* createProm(int initial_capacity, int nb_students);

/**
 * @brief Libère toute la mémoire associée à une promotion (étudiants, tableau, structure).
 * 
 * @param p Pointeur vers la promotion à détruire.
 */
void destroyProm(Prom* p);

#endif // STRUCT_FUNCTIONS_H
