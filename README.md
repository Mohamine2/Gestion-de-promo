# Gestion de Promotion Étudiante

## Introduction

Ce projet en langage C a pour objectif de gérer une promotion d’étudiants.  
Les fonctionnalités principales incluent :  
- Chargement des données depuis un fichier texte  
- Calcul des moyennes des étudiants  
- Sauvegarde des données en binaire  
- Tri et classement des meilleurs étudiants  

## Structure du projet

- **struct.h / struct_functions.c** : définition et gestion des structures principales (`Student`, `Course`, `Prom`, etc.)  
- **file_gestion.c** : lecture et écriture des fichiers (texte et binaire)  
- **file_sorting.c** : fonctions de tri et de classement des étudiants  
- **main.c** : fonction principale, lancement du programme  

## Compilation et exécution

Pour compiler le projet, utiliser la commande :  
```bash
make

Puis éxecuter le programme avec :
```bash
./exec data.txt
## Documentation

La documentation du projet est générée automatiquement à l’aide de **Doxygen**, un outil permettant de créer une documentation claire et structurée à partir des commentaires dans le code source.

### Génération de la documentation

Pour générer la documentation, il faut d’abord disposer d’un fichier de configuration `Doxyfile`.  
S’il n’existe pas encore, vous pouvez le créer à l’aide de la commande suivante :
```bash
doxygen -g


