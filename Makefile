# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/09 17:32:17 by qpupier           #+#    #+#              #
#    Updated: 2026/02/19 14:47:56 by qpupier          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME		=	computor-v2
DIR_SRC		=	src
DIR_OBJ		=	.obj
DIR_INC		=	inc
LST_SRC		=	main.cpp			\
				parser.cpp			\
				Node.cpp			\
				Token.cpp			\
				ast.cpp				\
				interpret.cpp		\
				types/IType.cpp		\
				types/Operator.cpp	\
				types/Variable.cpp	\
				types/Number.cpp
LST_OBJ		=	$(LST_SRC:.cpp=.o)
LST_DEP		=	$(LST_OBJ:.o=.d)
LST_INC		= 	$(DIR_INC)\
				$(DIR_INC)/types
SRC			=	$(addprefix $(DIR_SRC)/, $(LST_SRC))
OBJ			=	$(addprefix $(DIR_OBJ)/, $(LST_OBJ))
DEP			=	$(addprefix $(DIR_OBJ)/, $(LST_DEP))
INC			=	$(addprefix -I./, $(LST_INC))
DIRS		=	$(DIR_OBJ)/types
CC			=	c++
CXXFLAGS	=	-W -Wall -Wextra -Werror -Wshadow -Wold-style-cast -Wcast-qual -Wconversion -Wsign-conversion -Wstrict-aliasing -g3
CXXFLAGS	+=	-O2 # Optimization
# CXXFLAGS	+=	-fsanitize=address # Debugging
CDEP		=	-MMD -MP

ERASE		=	\033[2K\r
GREY		=	\033[30m
RED			=	\033[31m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
PINK		=	\033[35m
CYAN		=	\033[36m
WHITE		=	\033[37m
BOLD		=	\033[1m
UNDER		=	\033[4m
SUR			=	\033[7m
END			=	\033[0m

all: $(NAME)
	@printf "$(BLUE)> $(NAME): $(YELLOW)Project ready!$(END)\n"

$(NAME): $(OBJ)
	$(CC) $(CXXFLAGS) $^ -o $@
	@printf "$(ERASE)$(BLUE)> $@: $(GREEN)Success!$(END)\n\n"
-include $(DEP)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp Makefile
	mkdir -p $(DIR_OBJ) $(DIRS)
	$(CC) $(CXXFLAGS) $(CDEP) $(INC) -c $< -o $@
	@printf "$(ERASE)$(BLUE)> Compilation:$(END) $<"

clean:
	@rm -rf $(DIR_OBJ)
	@printf "$(ERASE)$(ERASE)$(BLUE)> Deleted: $(RED)$(DIR_OBJ)$(END)\n"
	
fclean: clean
	@rm -rf $(NAME)
	@printf "$(ERASE)$(ERASE)$(BLUE)> Deleted: $(RED)$(NAME)$(END)\n"

re: fclean all

run: $(NAME)
	./$<

valgrind: $(NAME)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s ./$<

.PHONY: all clean fclean re run valgrind
.SILENT:
