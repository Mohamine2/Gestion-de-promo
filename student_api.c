
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
typedef struct CLASS_DATA {
    Student** students;     /**< Tableau dynamique de pointeurs vers les étudiants. */
    int num_students;       /**< Nombre total d’étudiants dans la promotion. */
    int capacity;           /**< Capacité actuelle du tableau d’étudiants (pour realloc). */
} CLASS_DATA;


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Charge la promotion complète à partir d’un fichier texte.
 */
CLASS_DATA* API_load_students(char* filePath) {
    FILE* data = fopen(filePath, "r");
    if (data == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s.\n", filePath);
        return NULL;
    }

    CLASS_DATA* promo = malloc(sizeof(CLASS_DATA));
    if (promo == NULL){
        return NULL;
    }

    promo->num_students = 0;
    promo->capacity = 200;
    promo->students = NULL;

    promo->students = calloc(promo->capacity, sizeof(Student*));
    if (promo->students == NULL) {
        free(promo);
        return NULL;
    }

    if (promo == NULL) {
        fclose(data);
        fprintf(stderr, "Erreur : allocation échouée pour la promotion.\n");
        return NULL;
    }

    Course* allCourses[50];
    int nbCourses = 0;
    char line[256];
    int mode = 0;

    while (fgets(line, sizeof(line), data)) {
        if (strncmp(line, "ETUDIANTS", 9) == 0) { mode = 1; fgets(line, sizeof(line), data); continue; }
        if (strncmp(line, "MATIERES", 8) == 0) { mode = 2; fgets(line, sizeof(line), data); continue; }
        if (strncmp(line, "NOTES", 5) == 0) { mode = 3; fgets(line, sizeof(line), data); continue; }

        if (mode == 1) {
            if (line == NULL){
                continue;
            }
            int id, age;
            char first_name[128], last_name[128];

            if (sscanf(line, "%d;%[^;];%[^;];%d", &id, first_name, last_name, &age) != 4){
                continue;
            }

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
            s->num_courses = nbCourses;
            s->general_average = 0.0f;
            s->courses = NULL;

            if (nbCourses > 0) {
                s->courses = calloc(nbCourses, sizeof(Course*));
                if (s->courses == NULL) {
                    free(s->first_name);
                    free(s->last_name);
                    free(s);
                    return NULL;
                }
            }

            if (promo->num_students >= promo->capacity) {
                promo->capacity *= 2;
                Student** tmp = realloc(promo->students, sizeof(Student*) * promo->capacity);
                if (tmp == NULL) {
                    fclose(data);
                    return NULL;
                }
                promo->students = tmp;
            }
            promo->students[promo->num_students] = s;
            promo->num_students++;
        }
        else if (mode == 2) {

            if (line == NULL){
                continue;
            }

            char name[128];
            float coeff;

            if (sscanf(line, "%[^;];%f", name, &coeff) != 2){
                continue;
            }

            Course* c = malloc(sizeof(Course));
            if (c == NULL) {
                return NULL;
            }

            c->course_name = malloc(strlen(name) + 1);
            if (c->course_name == NULL) {
                free(c);
                return NULL;
            }
            strcpy(c->course_name, name);

            c->coeff = coeff;

            c->grades = malloc(sizeof(Grades));
            if (c->grades == NULL) {
                return NULL;
            }

            c->grades->size = 0;
            c->grades->grades_array = NULL;

            c->average = 0.0f;

            if (c != NULL && nbCourses < 50){
                allCourses[nbCourses++] = c;
            }
        }
        else if (mode == 3) {

            if (line == NULL){
                continue;
            }

            int id;
            char course_name[128];
            float grade;

            if (sscanf(line, "%d;%[^;];%f", &id, course_name, &grade) != 3)
                continue;

            Student* s = NULL;
            for (int i = 0; i < promo->num_students; i++) {
                if (promo->students[i]->student_id == id) {
                    s = promo->students[i];
                    break;
                }
            }
            if (s == NULL){
                fprintf(stderr, "Erreur lors du traitement d'une ligne de note.\n");
                return NULL;
            }

            Course* c = NULL;
            for (int i = 0; i < s->num_courses; i++) {
                if (strcmp(course_name, s->courses[i]->course_name) == 0) {
                    c = s->courses[i];
                    break;
                }
            }

            // Si le cours n’existe pas encore → création
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
                if (!found){
                    return NULL;
                }

                Course* c = malloc(sizeof(Course));
                if (c == NULL) {
                    exit(1);
                }

                c->course_name = malloc(strlen(course_name) + 1);
                if (c->course_name == NULL) {
                    free(c);
                    exit(1);
                }
                strcpy(c->course_name, course_name);

                c->coeff = coeff;

                c->grades = malloc(sizeof(Grades));
                    if (c->grades == NULL) {
                        return NULL;
                }

                c->grades->size = 0;
                c->grades->grades_array = NULL;
                
                c->average = 0.0f;


                Course** tmp = realloc(s->courses, sizeof(Course*) * (s->num_courses + 1));
                if (tmp == NULL) {
                    return NULL;
                }

                s->courses = tmp;
                s->courses[s->num_courses++] = c;
            }

            // Ajout de la note
            float* tmp2 = realloc(c->grades->grades_array, sizeof(float) * (c->grades->size + 1));
            if (tmp2 == NULL){
                return NULL;
            }

            c->grades->grades_array = tmp2;
            c->grades->grades_array[c->grades->size++] = grade;

            // Calcul des moyennes
            float sum = 0;
            for (int n = 0; n < c->grades->size; n++)
                sum += c->grades->grades_array[n];
            c->average = sum / c->grades->size;

            float total = 0, total_coeff = 0;
            for (int i = 0; i < s->num_courses; i++) {
                total += s->courses[i]->average * s->courses[i]->coeff;
                total_coeff += s->courses[i]->coeff;
            }
            if (total_coeff > 0){
                s->general_average = total / total_coeff;
            }
        }
    }

    fclose(data);
    return promo;
}


/**
 * @brief Sauvegarde la promotion dans un fichier binaire.
 * @return 0 si succès, -1 sinon.
 */
int saveInBinaryFile(CLASS_DATA* pClass, char* filePath) {
    if (filePath == NULL || pClass == NULL) {
        fprintf(stderr, "Erreur : arguments invalides.\n");
        return 0;
    }

    FILE* data = fopen(filePath, "wb");
    if (data == NULL) {
        fprintf(stderr, "Erreur : impossible d’écrire dans le fichier %s.\n", filePath);
        return 0;
    }

    if (fwrite(&pClass->num_students, sizeof(int), 1, data) != 1) {
        fprintf(stderr, "Erreur lors de l’écriture du nombre d’étudiants.\n");
        fclose(data);
        return 0;
    }

    for (int i = 0; i < pClass->num_students; i++) {
        Student* s = pClass->students[i];
        if (s == NULL) continue;

        if (fwrite(&s->general_average, sizeof(float), 1, data) != 1 ||
            fwrite(&s->student_id, sizeof(int), 1, data) != 1 ||
            fwrite(&s->num_courses, sizeof(int), 1, data) != 1 ||
            fwrite(&s->age, sizeof(int), 1, data) != 1) {
            fprintf(stderr, "Erreur lors de l’écriture des infos de l’étudiant %d.\n", s->student_id);
            fclose(data);
            return 0;
        }

        int len = strlen(s->first_name) + 1;
        if (fwrite(&len, sizeof(int), 1, data) != 1 ||
            fwrite(s->first_name, sizeof(char), len, data) != (size_t)len) {
            fprintf(stderr, "Erreur lors de l’écriture du prénom de %d.\n", s->student_id);
            fclose(data);
            return 0;
        }

        len = strlen(s->last_name) + 1;
        if (fwrite(&len, sizeof(int), 1, data) != 1 ||
            fwrite(s->last_name, sizeof(char), len, data) != (size_t)len) {
            fprintf(stderr, "Erreur lors de l’écriture du nom de %d.\n", s->student_id);
            fclose(data);
            return 0;
        }

        for (int j = 0; j < s->num_courses; j++) {
            Course* c = s->courses[j];
            if (c == NULL) continue;

            len = strlen(c->course_name) + 1;
            if (fwrite(&len, sizeof(int), 1, data) != 1 ||
                fwrite(c->course_name, sizeof(char), len, data) != (size_t)len ||
                fwrite(&c->coeff, sizeof(float), 1, data) != 1 ||
                fwrite(&c->average, sizeof(float), 1, data) != 1 ||
                fwrite(&c->grades->size, sizeof(int), 1, data) != 1) {
                fprintf(stderr, "Erreur lors de l’écriture du cours '%s' de %s.\n",
                        c->course_name, s->first_name);
                fclose(data);
                return 0;
            }

            if (c->grades->size > 0) {
                if (fwrite(c->grades->grades_array, sizeof(float), c->grades->size, data) != (size_t)c->grades->size) {
                    fprintf(stderr, "Erreur lors de l’écriture des notes du cours '%s'.\n", c->course_name);
                    fclose(data);
                    return 0;
                }
            }
        }
    }

    fclose(data);
    return 1;
}