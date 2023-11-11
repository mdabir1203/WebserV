NAME := webserv

OBJDIR := obj

SRCDIR := src

INCLUDES_DIR := inc

# SERVER := $(addprefix server/, test.cpp)

SRCS := main.cpp ConfigurationParcer.cpp Exceptions.cpp \
		${SERVER}

OBJS := $(patsubst %.cpp, ${OBJDIR}/%.o, ${SRCS})

# HEADER := $(addprefix includes/, Server.hpp)

INCLUDES := -I ${INCLUDES_DIR}

#Production flags
CXXFLAGS := -Wall -Wextra -Werror
CXXFLAGS += -std=c++98

#Development flags
# CXXFLAGS := -Wall -Wextra -std=c++98 -pedantic -g -O0
# CXXFLAGS += -g -O0
# CXXFLAGS += -fsanitize=thread
# CXXFLAGS += -fsanitize=address

#Add includes
CXXFLAGS += ${INCLUDES}

CXX := c++

RM := rm -f

all: ${NAME}
	./webserv

${NAME}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

${OBJDIR}/%.o: ${SRCDIR}/%.cpp ${HEADER} | ${OBJDIR}
	mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} -c $< -o $@

${OBJDIR}:
	mkdir -p ${OBJDIR}

clean:
	${RM} -r ${OBJDIR}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re

.NOTPARALLEL: