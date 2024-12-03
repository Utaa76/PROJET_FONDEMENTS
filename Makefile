CC = gcc
OBJ = ./obj/*.o
HEADER = ./include/image.h ./include/merge.h ./include/classe.h ./include/exit_if.h ./include/moments.h ./include/move_type.h ./include/objet.h ./include/point.h ./include/RAG.h ./include/type_obj.h
CFLAGS = -Wall -ansi -pedantic -c -g
SRC = ./src/
CPPFLAGS = -I./include/
PATH_OBJ = ./obj/

all: app

app:$(OBJ)
	@echo "Création du fichier exécutable app (situé dans ./bin/)."
	@$(CC) $^ -o ./bin/$@ -lm

clean:
	rm $(OBJ)

${PATH_OBJ}%.o: ${SRC}%.c $(HEADER)
	@echo "Utilisation des options : $(CFLAGS)."
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

.PHONY: all clean