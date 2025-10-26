/**
 * @file file_gestion.h
 * @brief Fonctions de gestion de fichiers pour le chargement, la sauvegarde et l'affichage d'une promotion d'étudiants.
 *
 * Ce fichier contient les déclarations des fonctions permettant :
 * - de lire un fichier texte et de charger une promotion en mémoire ;
 * - de sauvegarder une promotion au format binaire ;
 * - de restaurer cette promotion depuis un fichier binaire ;
 * - d'afficher les informations stockées en mémoire.
 *
 * @author  Mohamed
 * @date    26 octobre 2025
 * @version 1.0
 */

#ifndef FILE_GESTION_H
#define FILE_GESTION_H

#include "./struct.h"

/**
 * @brief Vérifie que le fichier de données est passé en argument au programme.
 * @param argc Nombre d’arguments passés au programme.
 */
void checkArguments(int argc);

/**
 * @brief Analyse une ligne décrivant un cours et crée la structure correspondante.
 * @param line Ligne du fichier contenant le nom du cours et son coefficient.
 * @return Un pointeur vers une structure Course allouée dynamiquement, ou NULL en cas d’erreur.
 */
Course* parseCourseLine(char* line);

/**
 * @brief Analyse une ligne décrivant un étudiant et crée la structure correspondante.
 * @param line Ligne du fichier contenant les informations de l'étudiant (id, prénom, nom, âge).
 * @return Un pointeur vers une structure Student allouée dynamiquement, ou NULL en cas d’erreur.
 */
Student* parseStudentLine(char* line);

/**
 * @brief Analyse une ligne décrivant une note et met à jour les structures correspondantes.
 * 
 * Cette fonction recherche l'étudiant et le cours concernés,
 * ajoute la note, puis met à jour la moyenne du cours et la moyenne générale de l’étudiant.
 *
 * @param line Ligne du fichier contenant l’identifiant de l’étudiant, le nom du cours et la note.
 * @param promo Pointeur vers la promotion en mémoire.
 * @param allCourses Tableau de pointeurs vers les cours disponibles.
 * @param nbCourses Nombre total de cours enregistrés.
 */
void parseGradeLine(char* line, Prom* promo, Course* allCourses[], int nbCourses);

/**
 * @brief Charge une promotion complète à partir d’un fichier texte.
 * 
 * Ce fichier doit être structuré en trois sections : 
 * - `ETUDIANTS`
 * - `MATIERES`
 * - `NOTES`
 *
 * @param filename Nom du fichier texte à charger.
 * @return Un pointeur vers la structure Prom allouée dynamiquement, ou NULL en cas d’erreur.
 */
Prom* loadPromotionFromFile(char* filename);

/**
 * @brief Affiche les informations d'une promotion.
 * 
 * Affiche les étudiants, leurs cours, leurs notes,
 * les moyennes par matière et les moyennes générales.
 *
 * @param p Pointeur vers la promotion à afficher.
 */
void printPromotion(Prom* p);

/**
 * @brief Sauvegarde la promotion en mémoire dans un fichier binaire.
 * 
 * Les informations sauvegardées incluent les étudiants,
 * leurs cours, leurs notes et leurs moyennes.
 *
 * @param filename Nom du fichier binaire de destination.
 * @param promo Pointeur vers la promotion à sauvegarder.
 */
void saveInBinaryFile(char* filename, Prom* promo);

/**
 * @brief Restaure une promotion à partir d’un fichier binaire.
 * 
 * Recrée en mémoire toutes les structures à partir des données binaires
 * précédemment sauvegardées avec `saveInBinaryFile()`.
 *
 * @param filename Nom du fichier binaire à lire.
 * @return Un pointeur vers la structure Prom restaurée, ou NULL en cas d’erreur.
 */
Prom* loadPromotionFromBinaryFile(char* filename);

#endif // FILE_GESTION_H
