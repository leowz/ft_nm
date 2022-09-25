
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zweng <zweng@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/19 12:41:50 by zweng             #+#    #+#              #
#    Updated: 2022/09/25 22:01:53 by zweng            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ----- varaibles -----

CC = gcc

NAME = ft_nm

LIB_PATH = libft

LIB = $(LIB_PATH)/libft.a

HEADER_PATH = includes $(LIB_PATH)/includes

C_PATH = srcs

# ---------------- transformation ------------------ #

HEADER = $(HEADER_PATH)/$(NAME).h 

CFILES = $(foreach D, $(C_PATH), $(wildcard $(D)/*.c))

OBJS = $(patsubst %.c, %.o, $(CFILES))

DFILES = $(patsubst %.c, %.d, $(CFILES))

LDFLAGS = -L$(LIB_PATH)

LDLIBS = -lft #-fsanitize=address

DPFLAGS = -MD -MP

CFLAGS =  -Wall -Wextra -Werror $(foreach D, $(HEADER_PATH), -I$(D)) $(DPFLAGS)

# ----- part automatic -----
#SRCS = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
#OBJS = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

# ----- Colors -----
BLACK:="\033[1;30m"
RED:="\033[1;31m"
GREEN:="\033[1;32m"
PURPLE:="\033[1;35m"
CYAN:="\033[1;36m"
WHITE:="\033[1;37m"
EOC:="\033[0;0m"
#  # ==================

# ----- part rules -----
all: $(NAME)

$(NAME): $(LIB) $(OBJS)
	@$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@
	@printf $(GREEN)"$(NAME) Finish linking\n"$(EOC)

$(LIB):
	@make -C $(LIB_PATH) fclean && make -C $(LIB_PATH)

%.o:%.c
	@printf $(GREEN)"compiling %s\n"$(EOC) $@
	@$(CC) $(CFLAGS) -o $@ -c $<

dclean:
	@rm -f $(DFILES)
	@printf $(GREEN)"$(NAME) dclean\n"$(EOC)

clean: dclean
	@rm -f $(OBJS)
	@printf $(GREEN)"$(NAME) clean\n"$(EOC)
	@make -C $(LIB_PATH) clean


fclean: clean dclean
	@/bin/rm -f $(NAME)
	@printf $(GREEN)"$(NAME) fclean\n"$(EOC)
	@/bin/rm -f $(LIB)
	@printf $(GREEN)"$(LIB) fclean\n"$(EOC)

-include $(DFILES)

re: fclean all

norme:
	@norminette $(SRCS)
	@norminette $(HEADER_PATH)/*.h

.PHONY: clean dclean fclean re norme all
