
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
BUILD_DIR = build

SRC = $(wildcard *.c)              # Tous les fichiers .c
OBJ = $(SRC:.c=.o)                 # Fichiers .o correspondants
OBJ2 = $(addprefix $(BUILD_DIR)/,$(OBJ)) # Fichiers .o avec le chemin build/
INC = $(wildcard *.h)              # Tous les .h

EXEC = exec                        # Nom de l’exécutable final
DOXYFILE = Doxyfile                # Fichier de configuration Doxygen



.PHONY: all
all: $(BUILD_DIR) $(EXEC) documentation


# Compilation
$(EXEC): $(OBJ2)
	@echo "Édition des liens..."
	$(CC) $^ -o $@
	@echo "Compilation terminée : ./$(EXEC)"


$(BUILD_DIR)/%.o: %.c $(INC)
	@echo " Compilation de $<..."
	$(CC) $(CFLAGS) -c $< -o $@


# Création du dossier build s’il n’existe pas
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)



.PHONY: documentation
documentation: $(EXEC)
	@echo " Génération de la documentation Doxygen..."
	@if [ -f $(DOXYFILE) ]; then doxygen $(DOXYFILE); else echo "⚠️  Aucun fichier Doxyfile trouvé."; fi



.PHONY: clean
clean:
	@echo "🧽 Nettoyage du projet..."
	@rm -rf $(BUILD_DIR) $(EXEC)
	@echo "✅ Espace de travail propre."


#Debug
.PHONY: debug
debug:
	@echo "SRC  = $(SRC)"
	@echo "OBJ  = $(OBJ)"
	@echo "OBJ2 = $(OBJ2)"
