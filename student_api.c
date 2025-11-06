
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
CLASS_DATA* API_load_students(char* filename) {
    FILE* data = fopen(filename, "r");
    if (data == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s.\n", filename);
        return NULL;
    }

    CLASS_DATA* promo = createProm(200, 0);
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
            char firstname[128], lastname[128];

            if (sscanf(line, "%d;%[^;];%[^;];%d", &id, firstname, lastname, &age) != 4){
                continue;
            }

            Student* s = createStudent(id, firstname, lastname, age, 0);
            if (promo->num_students >= promo->capacity) {
                promo->capacity *= 2;
                Student** tmp = realloc(promo->students, sizeof(Student*) * promo->capacity);
                if (!tmp) {
                    destroyStudent(s);
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

            Course* c = createCourse(name, coeff);

            if (c && nbCourses < 50)
                allCourses[nbCourses++] = c;
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

                c = createCourse(course_name, coeff);
                if (c == NULL){
                    return NULL;
                }

                Course** tmp = realloc(s->courses, sizeof(Course*) * (s->num_courses + 1));
                if (tmp == NULL) {
                    destroyCourse(c);
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