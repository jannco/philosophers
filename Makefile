# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/14 18:17:53 by yadereve          #+#    #+#              #
#    Updated: 2024/08/20 15:27:50 by yadereve         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				=	philo
CC					=	cc
CFLAGS				=	-Wall -Wextra -Werror -g3 -pthread
RM					=	rm -rf

SRC_DIR				=	sources
SRC					=	$(addprefix $(SRC_DIR)/,$(addsuffix .c, $(FILES_SOURCES)))
FILES_SOURCES		=	main philo routine utils utils2
OBJ_DIR				=	objects
OBJ					=	$(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.c=.o)))

GREEN				=	\033[32m
ORANGE				=	\033[31m\033[33m
BLUE				=	\033[1;34m
NC					=	\033[0m
PECKED				=	-----------

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@echo "$(BLUE)        __     __ __                           __"
	@echo ".-----.|  |--.|__|  |.-----.-----.-----.-----.|  |--.-----.----.-----."
	@echo "|  _  ||     ||  |  ||  _  |__ --|  _  |  _  ||     |  -__|   _|__ --|"
	@echo "|   __||__|__||__|__||_____|_____|_____|   __||__|__|_____|__| |_____|"
	@echo "|__|                                   |__|$(NC)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "\n$(NAME) $(GREEN)compiled and ready! $(NC)"
#	@echo "\n./philo 4 310 200 10"

clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(ORANGE)\n$(PECKED) program objects cleaned $(PECKED)$(NC)"


fclean: clean
	@$(RM) $(NAME)
	@echo "$(ORANGE)\n $(PECKED) program name cleaned $(PECKED)$(NC)"

re: fclean all

v:	all
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes --track-origins=yes ./philo

.PHONY: all clean fclean re
