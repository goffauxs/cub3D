# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/13 14:31:55 by sgoffaux          #+#    #+#              #
#    Updated: 2021/10/29 15:51:07 by mdeclerf         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	cub3D
LIBFT		=	libft/
GNL			=	get_next_line/
MLX			=	minilibx/
LIBFT_A		=	$(addprefix $(LIBFT), libft.a)
GNL_A		=	$(addprefix $(GNL), libgnl.a)
MLX_A		=	$(addprefix $(MLX), libmlx.a)

CC			=	gcc
INCLUDE 	=	includes
CFLAGS		=	-Wall -Wextra -Werror -I$(INCLUDE)

ifdef WIDTH
	CFLAGS	+=	-D WIDTH=$(WIDTH)
endif
ifdef HEIGHT
	CFLAGS	+=	-D HEIGHT=$(HEIGHT)
endif

DIR_PARSING =	srcs/parsing/

RM			=	rm -f
SRCS		=	main.c \
				srcs/utils.c \
				srcs/move.c \
				srcs/look.c \
				srcs/hooks.c \
				srcs/pixel.c \
				srcs/draw.c \
				srcs/ray_casting.c \
				srcs/minimap.c \
				$(DIR_PARSING)parsing_map_utils.c \
				$(DIR_PARSING)parsing.c \
				$(DIR_PARSING)parsing_map.c \
				$(DIR_PARSING)parsing_rgb.c \
				$(DIR_PARSING)parsing_text.c \
				$(DIR_PARSING)parsing_utils.c \
				$(DIR_PARSING)parsing_map_utils2.c \

OBJS		=	$(SRCS:%.c=%.o)

all:			$(NAME)

$(NAME):		$(OBJS) $(LIBFT_A) $(GNL_A) $(MLX_A)
				@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT) -lft -L$(GNL) -lgnl -L$(MLX) -lmlx -lm -o $(NAME) -framework OpenGL -framework AppKit
				@echo "Linked into executable \033[0;32m$(NAME)\033[0m."

$(LIBFT_A):
				@$(MAKE) -s -C $(LIBFT)
				@echo "Compiled $(LIBFT_A)."

$(GNL_A):
				@$(MAKE) -s -C $(GNL)
				@echo "Compiled $(GNL_A)."

$(MLX_A):
				@$(MAKE) -s -C $(MLX)
				@echo "Compiled $(MLX_A)."

bonus:			all

.c.o:
				@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)
				@echo "Compiling $<."

localclean:
				@$(RM) $(OBJS)
				@echo "Removed object files."

clean:			localclean
				@$(MAKE) clean -s -C $(LIBFT)
				@echo "Clean libft."
				@$(MAKE) clean -s -C $(GNL)
				@echo "Clean gnl."
				@$(MAKE) clean -s -C $(MLX)
				@echo "Clean mlx."

fclean:			localclean
				@$(MAKE) fclean -s -C $(LIBFT)
				@echo "Full clean libft."
				@$(MAKE) fclean -s -C $(GNL)
				@echo "Full clean gnl."
				@$(MAKE) clean -s -C $(MLX)
				@echo "Clean mlx."
				@$(RM) $(NAME)
				@echo "Removed executable."

re:				fclean all

.PHONY:			all clean fclean re localclean bonus