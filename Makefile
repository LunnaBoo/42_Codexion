NAME = codexion.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
TARGET = codexion
RM = rm -rf
AR = ar -rc

OBJDIR = build
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=$(OBJDIR)/%.o)

all: $(TARGET)

$(TARGET): $(NAME)
	$(CC) $(CFLAGS) -o $(TARGET) $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean: 
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(TARGET)

re: fclean all

test: all
	bash tests/run.sh

.PHONY: all clean fclean re test
