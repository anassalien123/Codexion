NAME = codexion

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

CC = cc -Wall -Wextra -Werror -pthread
FLAGS = -I $(INC_DIR)

SRC = codexion.c \
	  parsing.c \
	  utils.c \
	  init_program.c \
	  create_threads.c \
	  destroy_program.c \
	  init_coders.c \
	  join_threads.c \
	  coder_routine.c \
	  release_dongles.c \
	  take_dongles.c \
	  monitor.c \
	  heap.c \
	  heap_utils.c \
	  request_dongle.c \
	  order.c \
	  heap_ops.c \
	  allocate_memory.c

OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

ra: all clean

.PHONY: all clean fclean re ra
