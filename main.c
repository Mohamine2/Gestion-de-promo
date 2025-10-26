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
#include <stdlib.h>

#include "./struct.h"
#include "./struct_functions.h"
#include "./file_gestion.h"
#include "./file_sorting.h"


int main(int argc, char* argv[]) {
    checkArguments(argc);

    Prom* p = loadPromotionFromFile(argv[1]);
    if (p == NULL) {
        fprintf(stderr, "Erreur critique de chargement.\n");
        return EXIT_FAILURE;
    }

    if (saveInBinaryFile("save.bin", p) == -1) {
        fprintf(stderr, "Erreur lors de l'écriture dans le fichier binaire\n");
        destroyProm(p); // Libère la mémoire avant de quitter
        return EXIT_FAILURE;
    }

    destroyProm(p);

    Prom* p_loaded = loadPromotionFromBinaryFile("save.bin");
    if (p_loaded == NULL) {
        fprintf(stderr, "Erreur critique de chargement\n");
        destroyProm(p);
        return EXIT_FAILURE;
    }

    int count = 0;
    Student** top_ten = getTopTenStudents(p_loaded, &count);
    if (top_ten == NULL) {
        fprintf(stderr, "Erreur Allocation Mémoire\n");
        destroyProm(p);
        destroyProm(p_loaded);
        return EXIT_FAILURE;
    }

    printPromotion(p_loaded);

    printf("\n--- Top 10 students ---\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %.2f\n", top_ten[i]->first_name, top_ten[i]->general_average);
    }

    char* course = "Geographie";
    Student** top_three = getTopThreeStudentsCourse(p_loaded, course, &count);
    if (top_three == NULL) {
        fprintf(stderr, "Erreur Allocation Mémoire\n");
        free(top_ten);
        return EXIT_FAILURE;
    }

    printf("\n--- Top %d Students in %s ---\n", count, course);
    for (int i = 0; i < count; i++) {
        printf("%s: %.2f\n", top_three[i]->first_name, top_three[i]->courses[0]->average);
    }

    // Libération finale
    free(top_ten);
    free(top_three);

    return EXIT_SUCCESS;
}
