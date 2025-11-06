
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
int API_save_from_binary_file(CLASS_DATA* pClass, char* filePath) {
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
            if (c == NULL){
                continue;
            }

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



/**
 * @brief Charge une promotion depuis un fichier binaire.
 */
CLASS_DATA* API_restore_from_binary_file(char* filePath){ // This function read the binaryFile to restore the memory context
    FILE* data = fopen(filePath, "rb");
    if (data == NULL){
        return NULL;
    }

    int nb_students;
    if (fread(&nb_students, sizeof(int), 1, data) != 1){
        fprintf(stderr, "Cannot read the number of students\n");
        fclose(data);
        return NULL;
    }

    if (nb_students < 0){
        fprintf(stderr, "Invalid number of students\n");
        fclose(data);
        return NULL;
    }

    CLASS_DATA* promo = malloc(sizeof(CLASS_DATA));
    if (promo == NULL){
        return NULL;
    }

    promo->num_students = nb_students;
    promo->capacity = nb_students;
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
    
    size_t len = 0;
    for(int i = 0; i < nb_students; i++){
        Student* s = malloc(sizeof(Student));
        if (s == NULL){
            fprintf(stderr, "Cannot allocate student\n");
            fclose(data);
            return NULL;
        }

        if (fread(&s->general_average, sizeof(float), 1, data) != 1 ||
            fread(&s->student_id, sizeof(int), 1, data) != 1 ||
            fread(&s->num_courses, sizeof(int), 1, data) != 1 ||
            fread(&s->age, sizeof(int), 1, data) != 1) {
            fprintf(stderr, "Error reading student basic data\n");
            free(s);
            fclose(data);
            return NULL;
        }
        
        if (fread(&len, sizeof(int), 1, data) != 1){
            fprintf(stderr, "Error reading first_name length\n");
            free(s);
            fclose(data);
            return NULL;
        }

        if (len <= 0 || len > 256){
            fprintf(stderr, "Invalid first_name length: %ld\n", len);
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
            fprintf(stderr, "Error reading first_name\n");
            free(s->first_name);
            free(s);
            fclose(data);
            return NULL;
        }
        

        if (fread(&len, sizeof(int), 1, data) != 1){
            fprintf(stderr, "Error reading last_name length\n");
            free(s->first_name);
            free(s);
            fclose(data);
            return NULL;
        }
        if (len <= 0 || len > 256){
            fprintf(stderr, "Invalid last_name length: %ld\n", len);
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
            fprintf(stderr, "Error reading last_name\n");
            free(s->first_name);
            free(s->last_name);
            free(s);
            fclose(data);
            return NULL;
        }

        promo->students[i] = s;

        s->courses = malloc(sizeof(Course*) * s->num_courses);
        if (s->courses == NULL && s->num_courses > 0){
            fprintf(stderr, "Cannot allocate courses array\n");
            free(s->first_name);
            free(s->last_name);
            free(s);
            fclose(data);
            return NULL;
        }

        for(int j = 0; j < s->num_courses; j++){
            Course* c = malloc(sizeof(Course));

            if (c == NULL){
                fprintf(stderr, "Cannot allocate course\n");
                //destroyStudent(s);
                fclose(data);
                return NULL;
            }

            c->grades = malloc(sizeof(Grades));

            if (c->grades == NULL){
                fprintf(stderr, "Cannot allocate grades\n");
                free(c);
                // destroyStudent(s);
                fclose(data);
                return NULL;
            }

            if (fread(&len, sizeof(int), 1, data) != 1){
                fprintf(stderr, "Error reading course_name length\n");
                free(c->grades);
                free(c);
                // destroyStudent(s);
                fclose(data);
                return NULL;
            }
            if (len <= 0 || len > 256){
                fprintf(stderr, "Invalid course_name length: %ld\n", len);
                free(c->grades);
                free(c);
                //destroyStudent(s);
                fclose(data);
                return NULL;
            }
            
            c->course_name = malloc(sizeof(char) * len);

            if (c->course_name == NULL){
                free(c->grades);
                free(c);
                // destroyStudent(s);
                fclose(data);
                return NULL;
            }
            if (fread(c->course_name, sizeof(char), len, data) != len){
                fprintf(stderr, "Error reading course_name\n");
                free(c->course_name);
                free(c->grades);
                free(c);
                // destroyStudent(s);
                fclose(data);
                return NULL;
            }


            if (fread(&c->coeff, sizeof(float), 1, data) != 1 ||
                fread(&c->average, sizeof(float), 1, data) != 1 ||
                fread(&c->grades->size, sizeof(int), 1, data) != 1){
                fprintf(stderr, "Error reading course data\n");
                free(c->course_name);
                free(c->grades);
                free(c);
                // destroyStudent(s);
                fclose(data);
                return NULL;
            }
            
            if (c->grades->size < 0){
                fprintf(stderr, "Invalid grades size: %d\n", c->grades->size);
                free(c->course_name);
                free(c->grades);
                free(c);
                // destroyStudent(s);
                fclose(data);
                return NULL;
            }

            c->grades->grades_array = malloc(sizeof(float)  * c->grades->size);

            if (c->grades->grades_array == NULL && c->grades->size > 0){
                free(c->course_name);
                free(c->grades);
                free(c);
                // destroyStudent(s);
                fclose(data);
                return NULL;
            }

            if ((int)fread(c->grades->grades_array, sizeof(float), c->grades->size, data) != c->grades->size){
                fprintf(stderr, "Error reading grades array\n");
                free(c->grades->grades_array);
                free(c->course_name);
                free(c->grades);
                free(c);
                // destroyStudent(s);
                fclose(data);
                return NULL;
            }

            s->courses[j] = c;
        }
    }

    fclose(data);

    return promo;
}