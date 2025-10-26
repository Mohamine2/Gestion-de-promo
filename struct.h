/**
 * @file struct.h
 * @brief Définitions des structures de données principales utilisées dans la gestion des étudiants, cours et promotions.
 *
 * Ce fichier contient la définition des structures nécessaires à la représentation :
 * - des notes d’un cours (`Grades`),
 * - des cours suivis par un étudiant (`Course`),
 * - des étudiants (`Student`),
 * - et d’une promotion entière (`Prom`).
 *
 * Ces structures servent de base à toutes les fonctions de manipulation des données
 * dans le projet.
 *
 * @author  Mohamed
 * @date    26 octobre 2025
 * @version 1.0
 */

#ifndef STRUCT_H
#define STRUCT_H

/**
 * @struct Grades
 * @brief Représente les notes d’un cours.
 */
typedef struct Grades {
    float* grades_array;  /**< Tableau dynamique contenant les notes. */
    int size;             /**< Nombre de notes stockées dans le tableau. */
} Grades;

/**
 * @struct Course
 * @brief Représente un cours avec son nom, son coefficient et ses notes.
 */
typedef struct Course {
    Grades* grades;       /**< Pointeur vers la structure contenant les notes du cours. */
    char* course_name;    /**< Nom du cours (chaîne de caractères). */
    float coeff;          /**< Coefficient du cours. */
    float average;        /**< Moyenne des notes du cours. */
} Course;

/**
 * @struct Student
 * @brief Représente un étudiant et ses informations personnelles ainsi que ses cours suivis.
 */
typedef struct Student {
    Course** courses;       /**< Tableau dynamique de pointeurs vers les cours suivis. */
    char* first_name;       /**< Prénom de l’étudiant. */
    char* last_name;        /**< Nom de l’étudiant. */
    float general_average;  /**< Moyenne générale calculée sur l’ensemble des cours. */
    int student_id;         /**< Identifiant unique de l’étudiant. */
    int num_courses;        /**< Nombre de cours suivis par l’étudiant. */
    int age;                /**< Âge de l’étudiant. */
} Student;

/**
 * @struct Prom
 * @brief Représente une promotion d’étudiants.
 */
typedef struct Prom {
    Student** students;     /**< Tableau dynamique de pointeurs vers les étudiants. */
    int num_students;       /**< Nombre total d’étudiants dans la promotion. */
    int capacity;           /**< Capacité actuelle du tableau d’étudiants (pour realloc). */
} Prom;

#endif // STRUCT_H
