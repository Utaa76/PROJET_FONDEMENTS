CC = gcc
OBJ = ./obj/*.o
HEADER = ./include/*.h
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

$(PATH_OBJ)%.o: $(SRC)%.c $(HEADER)
	@echo "Utilisation des options : $(CFLAGS)."
	@$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

.PHONY: all clean