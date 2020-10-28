##
## Pineapple PROJECT, 2019
## Makefile
## File description:
## make
##

NAME	=	FileCut

SRC	=	src/main.c	\
		src/cut/parse_cut.c	\
		src/cut/cut.c	\
		src/uncut/parse_uncut.c	\
		src/uncut/uncut.c	\

OBJ	=	$(SRC:.c=.o)

CFLAGS	+=	-Wall -Wextra -I./include -g3

all:	$(NAME)

$(NAME):	$(OBJ)
	make -C lib re
	gcc -o $(NAME) $(OBJ) lib/libmy

clean:
	rm -f $(OBJ)

fclean: clean
	make -C lib/ fclean
	rm -f $(NAME)

re: fclean all
