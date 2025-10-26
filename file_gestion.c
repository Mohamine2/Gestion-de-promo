/**
 * @file file_gestion.c
 * @brief Implémentation des fonctions de gestion de fichiers pour la promotion d’étudiants.
 *
 * Ce fichier contient toutes les fonctions nécessaires pour :
 * - lire un fichier texte de données et charger une promotion en mémoire ;
 * - sauvegarder et restaurer cette promotion au format binaire ;
 * - afficher les informations stockées en mémoire (étudiants, cours, notes, moyennes).
 *
 * @author  Mohamed-Amine
 * @date    26 octobre 2025
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./struct.h"
#include "./struct_functions.h"
#include "./file_gestion.h"

/**
 * @brief Vérifie la présence du fichier de données en argument.
 */
void checkArguments(int argc) {
    argc--;
    if (argc == 0) {
        printf("You must add the 'data.txt' file in parameter\n");
        exit(0);
    }
}

/**
 * @brief Analyse une ligne contenant les informations d’un cours.
 */
Course* parseCourseLine(char* line) {
    if (line == NULL) return NULL;

    char name[128];
    float coeff;

    if (sscanf(line, "%[^;];%f", name, &coeff) != 2) return NULL;

    return createCourse(name, coeff);
}

/**
 * @brief Analyse une ligne contenant les informations d’un étudiant.
 */
Student* parseStudentLine(char* line) {
    if (line == NULL) return NULL;

    int id, age;
    char firstname[128];
    char lastname[128];

    if (sscanf(line, "%d;%[^;];%[^;];%d", &id, firstname, lastname, &age) != 4)
        return NULL;

    return createStudent(id, firstname, lastname, age, 0);
}

/**
 * @brief Analyse une ligne de note et met à jour les structures correspondantes.
 *
 * Recherche l’étudiant et le cours concernés, ajoute la note et met à jour les moyennes.
 */
void parseGradeLine(char* line, Prom* promo, Course* allCourses[], int nbCourses) {
    if (line == NULL) return;

    int id;
    char course_name[128];
    float grade;

    if (sscanf(line, "%d;%[^;];%f", &id, course_name, &grade) != 3) return;

    // Recherche de l’étudiant concerné
    Student* s = NULL;
    for (int i = 0; i < promo->num_students; i++) {
        if (promo->students[i]->student_id == id) {
            s = promo->students[i];
            break;
        }
    }
    if (s == NULL) return;

    // Recherche du cours
    Course* c = NULL;
    for (int i = 0; i < s->num_courses; i++) {
        if (strcmp(course_name, s->courses[i]->course_name) == 0) {
            c = s->courses[i];
            break;
        }
    }

    // Création du cours s’il n’existe pas
    if (c == NULL) {
        float coeff = 0.0f;
        int found = 0;
        for (int j = 0; j < nbCourses; j++) {
            if (strcmp(allCourses[j]->course_name, course_name) == 0) {
                coeff = allCourses[j]->coeff;
                found = 1;
                break;
            }
        }
        if (!found) return;

        c = createCourse(course_name, coeff);
        Course** tmp = realloc(s->courses, sizeof(Course*) * (s->num_courses + 1));
        if (tmp == NULL) return;
        s->courses = tmp;
        s->courses[s->num_courses] = c;
        s->num_courses++;
    }

    // Ajout de la note
    float* tmp2 = realloc(c->grades->grades_array, sizeof(float) * (c->grades->size + 1));
    if (tmp2 == NULL) return;
    c->grades->grades_array = tmp2;
    c->grades->grades_array[c->grades->size] = grade;
    c->grades->size++;

    // Mise à jour des moyennes
    float sum = 0;
    for (int n = 0; n < c->grades->size; n++)
        sum += c->grades->grades_array[n];
    c->average = sum / c->grades->size;

    float total = 0, total_coeff = 0;
    for (int i = 0; i < s->num_courses; i++) {
        total += s->courses[i]->average * s->courses[i]->coeff;
        total_coeff += s->courses[i]->coeff;
    }
    s->general_average = total / total_coeff;
}

/**
 * @brief Charge la promotion complète à partir d’un fichier texte.
 */

Prom* loadPromotionFromFile(char* filename){
    FILE* data = fopen(filename, "r");
    if (data == NULL){
        printf("Cannot read the file\n");
        return NULL;
    }

    Prom* promo = createProm(200, 0);
    Course* allCourses[50];
    int nbCourses = 0;

    char line[256];
    int mode = 0;

    while (fgets(line, sizeof(line), data)) {
        if (strncmp(line, "ETUDIANTS", 9) == 0){ // Check the wording
            mode = 1;
            fgets(line, sizeof(line), data); // Skip the wording line to go to the data
            continue;
        }
        if (strncmp(line, "MATIERES", 8) == 0){ // Check the wording
            mode = 2;
            fgets(line, sizeof(line), data); // Skip the wording line to go to the data
            continue;
        }
        if (strncmp(line, "NOTES", 5) == 0){ // Check the wording
            mode = 3;
            fgets(line, sizeof(line), data); // Skip the wording line to go to the data
            continue;
        }

        if (mode == 1){
            Student* s = parseStudentLine(line);
            if (s){
                if (promo->num_students >= promo->capacity){
                    promo->capacity *= 2;
                    promo->students = realloc(promo->students, sizeof(Student*) * promo->capacity);
                    if (promo->students == NULL){
                        return NULL;
                    }
                }
                promo->students[promo->num_students] = s;
                promo->num_students++;
            }
        }

        else if (mode == 2){
            Course* c = parseCourseLine(line);
            if (c){
                allCourses[nbCourses] = c;
                nbCourses++;
            }
        }

        else if (mode == 3){
            parseGradeLine(line, promo, allCourses, nbCourses);
        }
    }

    fclose(data);
    return promo;
}

/**
 * @brief Affiche toutes les informations de la promotion.
 */
void printPromotion(Prom* p) {
    printf("=== PROMOTION ===\n");
    for (int i = 0; i < p->num_students; i++) {
        Student* s = p->students[i];
        printf("\n%d - %s %s, %d ans\n", s->student_id, s->first_name, s->last_name, s->age);
        printf("Moyenne générale : %.2f\n", s->general_average);
        for (int j = 0; j < s->num_courses; j++) {
            Course* c = s->courses[j];
            printf("  %s (coeff %.2f) - Moy: %.2f - Notes: ", c->course_name, c->coeff, c->average);
            for (int k = 0; k < c->grades->size; k++) {
                printf("%.1f ", c->grades->grades_array[k]);
            }
            printf("\n");
        }
    }
}

/**
 * @brief Sauvegarde la promotion dans un fichier binaire.
 */
void saveInBinaryFile(char* filename, Prom* promo){ // This function store the memory context in a binary file
    FILE* data = fopen(filename, "wb");
    if (data == NULL){
        printf("Allocation Error\n");
        return;
    }

    fwrite(&promo->num_students, sizeof(int), 1, data);
    int len = 0;
    for(int i = 0; i < promo->num_students; i++){
        Student* s = promo->students[i];
        fwrite(&s->general_average, sizeof(float), 1, data);
        fwrite(&s->student_id, sizeof(int), 1, data);
        fwrite(&s->num_courses, sizeof(int), 1, data);
        fwrite(&s->age, sizeof(int), 1, data);
        len = (strlen(s->first_name)) + 1; // +1 for the '\0"
        fwrite(&len, sizeof(int), 1, data); // To save a char* we need to save the length first
        fwrite(s->first_name, sizeof(char), len, data);
        len = (strlen(s->last_name)) + 1;
        fwrite(&len, sizeof(int), 1, data);
        fwrite(s->last_name, sizeof(char), len, data);
        for (int j =0; j < s->num_courses; j++){
            Course* c = s->courses[j];
            len = (strlen(c->course_name)) + 1;
            fwrite(&len, sizeof(int), 1, data);
            fwrite(c->course_name, sizeof(char), len, data);
            fwrite(&c->coeff, sizeof(float), 1, data);
            fwrite(&c->average, sizeof(float), 1, data);
            fwrite(&c->grades->size, sizeof(int), 1, data);
            fwrite(c->grades->grades_array, sizeof(float), c->grades->size, data); // Save the grades_array
        }

    }
    fclose(data);
}


/**
 * @brief Charge une promotion depuis un fichier binaire.
 */
Prom* loadPromotionFromBinaryFile(char* filename){ // This function read the binaryFile to restore the memory context
    FILE* data = fopen(filename, "rb");
    if (data == NULL){
        printf("Allocation error\n");
        return NULL;
    }

    int nb_students;
    if (fread(&nb_students, sizeof(int), 1, data) != 1){
        printf("Cannot read the number of students\n");
        fclose(data);
        return NULL;
    }

    if (nb_students < 0){
        printf("Invalid number of students\n");
        fclose(data);
        return NULL;
    }

    Prom* promo = createProm(nb_students, nb_students);
    if (promo == NULL){
        printf("Allocation error\n");
        fclose(data);
        return NULL;
    }
    
    int len = 0;
    for(int i = 0; i < nb_students; i++){
        Student* s = malloc(sizeof(Student));
        if (s == NULL){
            printf("Cannot allocate student\n");
            fclose(data);
            return NULL;
        }

        if (fread(&s->general_average, sizeof(float), 1, data) != 1 ||
            fread(&s->student_id, sizeof(int), 1, data) != 1 ||
            fread(&s->num_courses, sizeof(int), 1, data) != 1 ||
            fread(&s->age, sizeof(int), 1, data) != 1) {
            printf("Error reading student basic data\n");
            free(s);
            fclose(data);
            return NULL;
        }
        
        if (fread(&len, sizeof(int), 1, data) != 1){
            printf("Error reading first_name length\n");
            free(s);
            fclose(data);
            return NULL;
        }

        if (len <= 0 || len > 256){
            printf("Invalid first_name length: %d\n", len);
            free(s);
            fclose(data);
            return NULL;
        }

        s->first_name = malloc(sizeof(char) * len);
        if (s->first_name == NULL){
            free(s);
            fclose(data);
            return NULL;
        }

        if (fread(s->first_name, sizeof(char), len, data) != len){
            printf("Error reading first_name\n");
            free(s->first_name);
            free(s);
            fclose(data);
            return NULL;
        }
        

        if (fread(&len, sizeof(int), 1, data) != 1){
            printf("Error reading last_name length\n");
            free(s->first_name);
            free(s);
            fclose(data);
            return NULL;
        }
        if (len <= 0 || len > 256){
            printf("Invalid last_name length: %d\n", len);
            free(s->first_name);
            free(s);
            fclose(data);
            return NULL;
        }
        s->last_name = malloc(len);
        if (s->last_name == NULL){
            free(s->first_name);
            free(s);
            fclose(data);
            return NULL;
        }
        if (fread(s->last_name, sizeof(char), len, data) != len){
            printf("Error reading last_name\n");
            free(s->first_name);
            free(s->last_name);
            free(s);
            fclose(data);
            return NULL;
        }

        promo->students[i] = s;

        s->courses = malloc(sizeof(Course*) * s->num_courses);
        if (s->courses == NULL && s->num_courses > 0){
            printf("Cannot allocate courses array\n");
            free(s->first_name);
            free(s->last_name);
            free(s);
            fclose(data);
            return NULL;
        }

        for(int j = 0; j < s->num_courses; j++){
            Course* c = malloc(sizeof(Course));

            if (c == NULL){
                printf("Cannot allocate course\n");
                destroyStudent(s);
                fclose(data);
                return NULL;
            }

            c->grades = malloc(sizeof(Grades));

            if (c->grades == NULL){
                printf("Cannot allocate grades\n");
                free(c);
                destroyStudent(s);
                fclose(data);
                return NULL;
            }

            if (fread(&len, sizeof(int), 1, data) != 1){
                printf("Error reading course_name length\n");
                free(c->grades);
                free(c);
                destroyStudent(s);
                fclose(data);
                return NULL;
            }
            if (len <= 0 || len > 256){
                printf("Invalid course_name length: %d\n", len);
                free(c->grades);
                free(c);
                destroyStudent(s);
                fclose(data);
                return NULL;
            }
            
            c->course_name = malloc(sizeof(char) * len);

            if (c->course_name == NULL){
                free(c->grades);
                free(c);
                destroyStudent(s);
                fclose(data);
                return NULL;
            }
            if (fread(c->course_name, sizeof(char), len, data) != len){
                printf("Error reading course_name\n");
                free(c->course_name);
                free(c->grades);
                free(c);
                destroyStudent(s);
                fclose(data);
                return NULL;
            }


            if (fread(&c->coeff, sizeof(float), 1, data) != 1 ||
                fread(&c->average, sizeof(float), 1, data) != 1 ||
                fread(&c->grades->size, sizeof(int), 1, data) != 1){
                printf("Error reading course data\n");
                free(c->course_name);
                free(c->grades);
                free(c);
                destroyStudent(s);
                fclose(data);
                return NULL;
            }
            
            if (c->grades->size < 0){
                printf("Invalid grades size: %d\n", c->grades->size);
                free(c->course_name);
                free(c->grades);
                free(c);
                destroyStudent(s);
                fclose(data);
                return NULL;
            }

            c->grades->grades_array = malloc(sizeof(float)  * c->grades->size);

            if (c->grades->grades_array == NULL && c->grades->size > 0){
                free(c->course_name);
                free(c->grades);
                free(c);
                destroyStudent(s);
                fclose(data);
                return NULL;
            }

            if (fread(c->grades->grades_array, sizeof(float), c->grades->size, data) != c->grades->size){
                printf("Error reading grades array\n");
                free(c->grades->grades_array);
                free(c->course_name);
                free(c->grades);
                free(c);
                destroyStudent(s);
                fclose(data);
                return NULL;
            }

            s->courses[j] = c;
        }
    }

    fclose(data);

    return promo;
}
