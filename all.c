#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Grades{
    float* grades_array;
    int size;
}Grades;


typedef struct Course{
    Grades* grades;
    char* course_name;
    float coeff;
    float average;
}Course;

typedef struct Student{
    Course** courses;           // 8 octets(pointeur)
    char* first_name;          // 8 octets (pointeur)
    char* last_name;           // 8 octets (pointeur)
    float general_average;     // 4 octets
    int student_id;            // 4 octets
    int num_courses;           // 4 octets
    int age;
}Student;

typedef struct Prom{
    Student** students;        // 8 octets (pointeur)
    int num_students;          // 4 octets
    int capacity;              // 4 octets
    // Total: 16 octets
} Prom;



Grades* createGrade(int size){
    Grades* g = NULL;
    g = malloc(sizeof(Grades));
    if (g == NULL){
        exit(0);
    }
    g->size = size;
    g->grades_array = NULL;
    if (size > 0){
        g->grades_array = calloc(size, sizeof(float));
        if (g-> grades_array == NULL){
            free(g);
            exit(1); // On free pas c dans le cas où on est dans un Course :(
        }
    }
    
    return g;
}


void destroyGrade(Grades* g){
    if (g == NULL){
        return;
    }
    free(g->grades_array);
    free(g);
}

Course* createCourse(char* name, float coeff){
    Course* c = NULL;
    c = malloc(sizeof(Course));
    if (c == NULL){
        exit(1);
    }
    c->course_name = malloc(strlen(name) + 1);
    if (c->course_name == NULL){
        free(c);
        exit(1);
    }
    strcpy(c->course_name, name);

    c->coeff = coeff;
    c->grades = createGrade(0);
    /*c->grades.grades_array = NULL;
    c->grades.size = 0; */
    c->average = 0.0f;

    return c;
}

void destroyCourse(Course* c){
    if (c == NULL){
        return;
    }
    destroyGrade(c->grades);
    free(c->course_name);
    free(c);
}

Student* createStudent(int id, char* first_name, char* last_name, int age, int num_courses){
    if (first_name == NULL || last_name == NULL || num_courses < 0){
        return NULL;
    }
    
    Student* s = malloc(sizeof(Student));
    if (s == NULL){
        return NULL;
    }
    
    // Allocation du prénom
    s->first_name = malloc(strlen(first_name) + 1);
    if (s->first_name == NULL){
        free(s);
        return NULL;
    }
    strcpy(s->first_name, first_name);
    
    // Allocation du nom
    s->last_name = malloc(strlen(last_name) + 1);
    if (s->last_name == NULL){
        free(s->first_name);
        free(s);
        return NULL;
    }
    strcpy(s->last_name, last_name);
    
    // Initialisation des champs
    s->student_id = id;
    s->age = age;
    s->num_courses = num_courses;
    s->general_average = 0.0f;
    
    // Allocation du tableau de cours
    s->courses = NULL;
    if (num_courses > 0){
        s->courses = calloc(num_courses, sizeof(Course*));
        if (s->courses == NULL){
            free(s->first_name);
            free(s->last_name);
            free(s);
            return NULL;
        }
    }
    
    return s;
}

void destroyStudent(Student* s){
    if (s == NULL){
        return;
    }
    
    // Libérer tous les cours
    if (s->courses != NULL){
        for (int i = 0; i < s->num_courses; i++){
            destroyCourse(s->courses[i]);
        }
        free(s->courses);
    }
    
    // Libérer les chaînes de caractères
    free(s->first_name);
    free(s->last_name);
    
    // Libérer la structure
    free(s);
}


Prom* createProm(int initial_capacity, int nb_students){
    if (initial_capacity < 0){
        return NULL;
    }
    
    Prom* p = malloc(sizeof(Prom));
    if (p == NULL){
        return NULL;
    }
    
    p->num_students = nb_students;
    p->capacity = initial_capacity;
    p->students = NULL;
    
    if (initial_capacity > 0){
        p->students = calloc(initial_capacity, sizeof(Student*));
        if (p->students == NULL){
            free(p);
            return NULL;
        }
    }
    
    return p;
}

void destroyProm(Prom* p){
    if (p == NULL){
        return;
    }
    
    // Libérer tous les étudiants
    if (p->students != NULL){
        for (int i = 0; i < p->num_students; i++){
            destroyStudent(p->students[i]);
        }
        free(p->students);
    }
    
    // Libérer la structure
    free(p);
}








void checkArguments(int argc){
    argc--; // Doesn't count the first parameter

    if ( argc == 0 ){
        printf("You must add the 'data.txt' file in parameter\n");
        exit(0);
    }
}

Course* parseCourseLine(char* line){
    if( line == NULL){
        return NULL;
    }

    char name[128];
    float coeff;

    if (sscanf(line, "%[^;];%f", name, &coeff) != 2){//
        return NULL;
    }

    return createCourse(name,coeff);
}

Student* parseStudentLine(char* line){
    if (line == NULL){
        return NULL;
    }

    int id, age;
    char firstname[128];
    char lastname[128];

    if (sscanf(line, "%d;%[^;];%[^;];%d", &id, firstname, lastname, &age) != 4){
        return NULL;
    }

    return createStudent(id, firstname, lastname, age, 0);
}

void parseGradeLine(char* line, Prom* promo, Course* allCourses[], int nbCourses){
    if (line == NULL){
        return;
    }

    int id;
    char course_name[128];
    float grade;

    if (sscanf(line, "%d;%[^;];%f", &id, course_name, &grade) != 3){
        return;
    }

    Student* s = NULL;
    for (int i = 0; i < promo->num_students; i++){
        if (promo->students[i]->student_id == id){
            s = promo->students[i];
            break;
        }
    }
    if (s == NULL){
        return;
    }

    Course* c = NULL;
    for (int i = 0; i < s->num_courses; i++){
        if (strcmp(course_name, s->courses[i]->course_name) == 0){
            c = s->courses[i];
            break;
        }
    }

    if (c == NULL){ // If the course doesnt exit in 'all_courses', we create it
        float coeff =0.0f;
        int found = 0;
        for (int j = 0; j < nbCourses; j++){
            if (strcmp(allCourses[j]->course_name, course_name) == 0){
                coeff = allCourses[j]->coeff;
                found = 1;
                break;
            }
        }
        if (!found){
            return;
        }
        c = createCourse(course_name, coeff);
        Course** tmp1 = realloc(s->courses, sizeof(Course*) * (s->num_courses +1));

        if (tmp1 == NULL){
            return;
        }
        s->courses = tmp1;
        s->courses[s->num_courses] = c;
        s->num_courses++;
    }


    float* tmp2 = realloc(c->grades->grades_array, sizeof(float) * (c->grades->size + 1));
    if (tmp2 == NULL){
        return;
    
    }

    c->grades->grades_array = tmp2;
    c->grades->grades_array[c->grades->size] = grade;
    c->grades->size ++;

    float sum = 0;

    // Calculate the course average
    for (int n = 0; n < c->grades->size; n++){
        sum += c->grades->grades_array[n];   
    }

    c->average = sum/c->grades->size;

    float total = 0;
    float total_coeff = 0;
    // Calculate the general average
    for (int i = 0; i < s->num_courses; i++){
        total += s->courses[i]->average * s->courses[i]->coeff;
        total_coeff += s->courses[i]->coeff;
    }
    s->general_average = total/total_coeff;

}

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

int compareStudents(const void* a, const void* b){ // Generic pointers in parameters to work with the qsort function
    Student* s1 = *(Student**)a; // Cast to Student** and deference it to obtain a Student*
    Student* s2 = *(Student**)b;

    if (s1-> general_average < s2->general_average){
        return 1;
    }
    if (s1->general_average > s2->general_average){
        return -1;
    }
    return 0;
}


Student** getTopTenStudents(Prom* promo, int* count){
    if (promo ==  NULL || promo->num_students <= 0){
        return NULL;
    }

    if (promo->num_students <= 10){
        *count = promo->num_students;
    }
    else{
        *count = 10;
    }

    Student** copy = malloc(sizeof(Student*) * promo->num_students);
    if (copy == NULL){
        return NULL;
    }

    memcpy(copy, promo->students, sizeof(Student*) * promo->num_students); // Copy the adress of the Students_array to manipulate the pointers' order without altering the original
    
    qsort(copy, promo->num_students, sizeof(Student*), compareStudents); // Call the qsort function to sort the array

    Student** top_ten = malloc(sizeof(Student*) * (*count));

    if(top_ten == NULL){
        free(copy);
        return NULL;
    }

    for (int i = 0; i < *count; i++) { 
        top_ten[i] = copy[i];
    }

    free(copy);
    return top_ten;
}


typedef struct {
    Student* s;
    float moyenne;
} StudentCourseAvg;


int compareCourseAvg(const void* a, const void* b) {
    const StudentCourseAvg* sa = (const StudentCourseAvg*)a;
    const StudentCourseAvg* sb = (const StudentCourseAvg*)b;
    if (sa->moyenne < sb->moyenne) return 1;
    if (sa->moyenne > sb->moyenne) return -1;
    return 0;
}


Student** getTopThreeStudentsCourse(Prom* promo, const char* course_name, int* top_count) {
    if (promo == NULL || course_name == NULL || promo->num_students == 0) {
        *top_count = 0;
        return NULL;
    }

    // Count how many students are related to the course
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
        return NULL; // No students found
    }

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

    qsort(savg, count, sizeof(StudentCourseAvg), compareCourseAvg);


    if (count < 3){
        *top_count = count;
    }
    else{
        *top_count = 3;
    }

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


int main(int argc, char* argv[]) {

    checkArguments(argc);
    Prom* p = loadPromotionFromFile(argv[1]);
    //if (p != NULL){
        // printPromotion(p);
    //}
    saveInBinaryFile("test.bin", p);
    //Prom* p = loadPromotionFromBinaryFile("test.bin");
    if (p == NULL){
        printf("Error in data loading\n");
    }

    int count = 0;
    Student** top_ten = getTopTenStudents(p, &count);
    // printPromotion(p);

    printf("--- Top 10 students ---\n");
    printf("\n");
    for(int i =0; i < count; i++){
        printf("%s: ", top_ten[i]->first_name);
        printf("%f",top_ten[i]->general_average);
        printf("\n");
        printf("\n");
    }

    printf("\n");
    printf("\n");

    char* course = "Geographie";

    Student** top_three = getTopThreeStudentsCourse(p, course, &count);

    printf("--- Top %d Students in %s ---\n", count, course);
    printf("\n");
    for(int i = 0; i < count; i++){
        printf("%s: ", top_three[i]->first_name);
        printf("%f",top_three[i]->courses[0]->average);
        printf("\n");
        printf("\n");
    }
    return 0;
}
