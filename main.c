/**
 * \mainpage Gestion de Promotion Étudiante
 *
 * \section intro_sec Introduction
 *
 * Ce projet en langage C a pour objectif de gérer une promotion d’étudiants :
 * chargement des données depuis un fichier texte, calcul des moyennes, 
 * sauvegarde en binaire, et tri des meilleurs étudiants.
 *
 * \section modules_sec Structure du projet
 *
 * - **struct.h / struct_functions.c** : définition et gestion des structures principales (`Student`, `Course`, `Prom`, etc.)
 * - **file_gestion.c** : lecture et écriture des fichiers (texte et binaire)
 * - **file_sorting.c** : fonctions de tri et de classement des étudiants
 * - **main.c** : fonction principale, lancement du programme
 *
 * \section usage_sec Compilation et exécution
 *
 * Utiliser la commande :
 * ```
 * make
 * ```
 * Puis exécuter le programme avec :
 * ```
 * ./exec data.txt
 * ```
 *
 * \section doxy_sec Documentation
 *
 * Cette documentation a été générée automatiquement avec **Doxygen**.
 */



#include <stdio.h>

#include "./struct.h"
#include "./file_gestion.h"
#include "./file_sorting.h"


int main(int argc, char* argv[]) {

    checkArguments(argc);
    Prom* p = loadPromotionFromFile(argv[1]);
    if (p != NULL){
        printPromotion(p);
    }
    saveInBinaryFile("save.bin", p);
    p = loadPromotionFromBinaryFile("save.bin");
    if (p == NULL){
        printf("Error in data loading\n");
    }

    int count = 0;
    Student** top_ten = getTopTenStudents(p, &count);
    printPromotion(p);

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
    return 0;
}
