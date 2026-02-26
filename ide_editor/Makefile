CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
SRC = main.c ide_editor.c window.c menu.c sidebar.c widget.c blueprint.c editor_file.c syntax_highlight.c
OBJ = $(SRC:.c=.o)

all: ide_editor

ide_editor: $(OBJ)
	$(CC) $(CFLAGS) -o ide_editor $(OBJ)

clean:
	rm -f *.o ide_editor
