
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zweng <zweng@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/19 12:41:50 by zweng             #+#    #+#              #
#    Updated: 2023/02/08 17:26:56 by vagrant          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ----- varaibles -----

CC 			= gcc
NAME 		= ft_nm
LIB_PATH 	= Libft
LIB 		= $(LIB_PATH)/libft.a
HEADER_PATH = includes $(LIB_PATH)/includes
C_PATH 		= srcs
OBJ_PATH	= obj

# ---------------- transformation ------------------ #

HEADER 		= $(HEADER_PATH)/$(NAME).h 

CFILES      = $(notdir $(foreach D, $(C_PATH), $(wildcard $(D)/*.c)))

OBJS_NAME	= $(patsubst %.c, %.o, $(CFILES)) \
	     	  $(patsubst %.c, %.o, $(LIBFILES))
DFILES_NAME	= $(patsubst %.c, %.d, $(CFILES)) \
			  $(patsubst %.c, %.d, $(LIBFILES))


LDFLAGS 	= -L$(LIB_PATH) -lft 

DPFLAGS 	= -MD -MP

CFLAGS 		=  $(foreach D, $(HEADER_PATH), -I$(D)) $(DPFLAGS) \
				#-Wall -Wextra -Werror \

DEBUGF 		= #-fsanitize=address -g

# ----- part automatic -----
SRCS 		= $(addprefix $(C_PATH)/,$(CFILES)) 
OBJS 		= $(addprefix $(OBJ_PATH)/,$(OBJS_NAME))
DFLS 		= $(addprefix $(OBJ_PATH)/,$(DFILES_NAME))

# ----- Colors -----
BLACK		:="\033[1;30m"
RED			:="\033[1;31m"
GREEN		:="\033[1;32m"
PURPLE		:="\033[1;35m"
CYAN		:="\033[1;36m"
WHITE		:="\033[1;37m"
EOC			:="\033[0;0m"
#  # ==================

# ----- part rules -----
all: $(NAME)

$(NAME): $(LIB) $(OBJS)
	@$(CC) $(OBJS) $(LDFLAGS) $(DEBUGF) -o $@
	@printf $(GREEN)"$(NAME) Finish linking\n"$(EOC)

$(LIB):
	@make -C $(LIB_PATH) fclean && make -C $(LIB_PATH)

$(OBJ_PATH)/%.o:$(C_PATH)/%.c | $(OBJ_PATH)
	@printf $(GREEN)"compiling %s\n"$(EOC) $@
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_PATH):
	@mkdir $(OBJ_PATH) 2> /dev/null

clean: 
	@rm -f $(OBJS) $(DFLS)
	@rm -rf $(OBJ_PATH) 2> /dev/null
	@printf $(GREEN)"$(NAME) clean\n"$(EOC)
	@make -C $(LIB_PATH) clean


fclean: clean
	@/bin/rm -f $(NAME)
	@printf $(GREEN)"$(NAME) fclean\n"$(EOC)
	@/bin/rm -f $(LIB)
	@printf $(GREEN)"$(LIB) fclean\n"$(EOC)
	@/bin/rm -f t1.*

-include $(DFILES)

diffy: $(NAME)
	./$(NAME) ${arg1} $(arg2) > t1.my
	nm ${arg1} $(arg2) > t1.nm
	diff -y t1.my t1.nm

diff: $(NAME)
	./$(NAME) ${arg1} $(arg2) > t1.my
	nm ${arg1} $(arg2) > t1.nm
	diff t1.my t1.nm

adiffy: $(NAME)
	./$(NAME) ${arg1} $(arg2) > t1.my
	nm ${arg1} $(arg2) > t1.nm
	diff -y t1.my t1.nm

test:
	echo $(arg1)

re: fclean all

norme:
	@norminette $(SRCS)
	@norminette $(HEADER_PATH)/*.h

.PHONY: clean fclean re norme all
