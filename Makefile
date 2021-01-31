#!/bin/bash

include config
export DOXYGEN_OUTPUT DOXYGEN_INPUT


# Compiler l'application
$(APPNAME): $(OBJ)
	@ $(shell mkdir -p $(OBJDIR))
	$(CC) $(FLAGS) -o $@ $^;

# Créer les fichiers objets des fichiers .c
%.o:
# On ajoute l'option "-D TEST" si le fichier est pile.c et quon souhaite tester
	@ if [ $(TEST) ] && [ "$(@:$(OBJDIR)%.o=$(SRCDIR)%.c)" = "$(SRCDIR)/$(FILEWITHTEST)" ]; then	\
		$(CC) $(FLAGS) -o $@ -c $(@:$(OBJDIR)%.o=$(SRCDIR)%.c) $(CFLAGS); 									\
		echo $(CC) -o $@ -c $(@:$(OBJDIR)%.o=$(SRCDIR)%.c) $(CFLAGS); 								\
	else 																							\
		$(CC) $(FLAGS) -o $@ -c $(@:$(OBJDIR)%.o=$(SRCDIR)%.c); 												\
		echo $(CC) -o $@ -c $(@:$(OBJDIR)%.o=$(SRCDIR)%.c); 										\
	fi


.PHONY: run clean valgrind

# Lancer l'application a partir d'une commande prédéfinie
run: $(APPNAME)
ifdef FILE
	$(RUN_CMD) "$(FILE)"
else
	$(RUN_CMD)
endif 


# Nettoyer le projet
clean:
	rm $(OBJ) $(APPNAME)

valgrind: $(APPNAME)
ifdef FILE
	@ valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(RUN_CMD) "$(FILE)"
else
	@ valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(RUN_CMD)
endif 
	


.PHONY: doc
# Nécessaire pour utilisation dans le fichier config Doxygen
export DOXYGEN_PNAME DOXYGEN_OUTPUT DOXYGEN_INPUT

# Lancer la création de la documentation
doc: $(APPNAME)
	@ doxygen $(DOXYGEN_CONFIG)
	@ cd $(DOXYGEN_OUTPUT)/latex && make
	@ cp $(DOXYGEN_OUTPUT)/latex/refman.pdf $(DOXYGEN_OUTPUT)/$(DOXYGEN_PDFNAME)