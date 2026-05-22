# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/09 17:32:17 by qpupier           #+#    #+#              #
#    Updated: 2026/05/22 21:12:33 by qpupier          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME		=	computor-v2
DIR_SRC		=	src
DIR_OBJ		=	.obj
DIR_INC		=	inc
LST_SRC		=	quadratic_equation_resolution/print_solutions/exact_solutions.cpp					\
				quadratic_equation_resolution/print_solutions/exact_solutions_imaginary_part.cpp	\
				quadratic_equation_resolution/print_solutions/exact_solutions_real_part.cpp			\
				quadratic_equation_resolution/print_solutions/rounded_solutions.cpp					\
				quadratic_equation_resolution/print_solutions/utils.cpp								\
				quadratic_equation_resolution/complex_solutions_structure.cpp						\
				quadratic_equation_resolution/Computor-V1.cpp										\
				quadratic_equation_resolution/imaginary_part.cpp									\
				quadratic_equation_resolution/real_part.cpp											\
				quadratic_equation_resolution/simplify_factors.cpp									\
				types/IType.cpp																		\
				types/Operator.cpp																	\
				types/Rational.cpp																	\
				types/Complex.cpp																	\
				types/Matrix.cpp																	\
				types/Polynomial.cpp																\
				AST.cpp																				\
				InfiniteDouble.cpp																	\
				InfiniteInt.cpp																		\
				Token.cpp																			\
				main.cpp																			\
				build_ast.cpp																		\
				equation.cpp																		\
				errors.cpp																			\
				parser.cpp																			\
				utils.cpp
LST_OBJ		=	$(LST_SRC:.cpp=.o)
LST_DEP		=	$(LST_OBJ:.o=.d)
LST_INC		= 	$(DIR_INC)	\
				$(DIR_INC)/types
SRC			=	$(addprefix $(DIR_SRC)/, $(LST_SRC))
OBJ			=	$(addprefix $(DIR_OBJ)/, $(LST_OBJ))
DEP			=	$(addprefix $(DIR_OBJ)/, $(LST_DEP))
INC			=	$(addprefix -I./, $(LST_INC))
DIRS		=	$(DIR_OBJ)/quadratic_equation_resolution					\
				$(DIR_OBJ)/quadratic_equation_resolution/print_solutions	\
				$(DIR_OBJ)/types
# CC			=	c++
CC			=	g++
CXXFLAGS	=	-W -Wall -Wextra -Werror -Wshadow -Wold-style-cast -Wcast-qual -Wconversion -Wsign-conversion -Wstrict-aliasing -g3
# CXXFLAGS	+=	-O2 # Optimization
CXXFLAGS	+=	-fsanitize=address # Debugging
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
	@printf "$(ERASE)$(BLUE)> Compilation:$(END) $<	"
	$(CC) $(CXXFLAGS) $(CDEP) $(INC) -c $< -o $@

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

test: $(NAME)
	@echo &> /dev/null
	./unit_tests/run_tests.sh

test_debug: $(NAME)
	@echo &> /dev/null
	./unit_tests/run_tests.sh debug

nb_lines: fclean
	@echo -n "Total lines of code: "
	find . -type f ! -path '*/.*' -exec wc -l {} + | tail -n 1 | sed "s/  //" | sed "s/ total//"

.PHONY: all clean fclean re run valgrind test test_debug nb_lines
.SILENT:
