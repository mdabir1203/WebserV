NAME := webserv

OBJDIR := obj

SRCDIR := src

INCLUDES_DIR := inc

CONFIG_PARSER := $(addprefix config_parser/, NewConfigParser.cpp ConfigParserUtils.cpp Exceptions.cpp)

SERVER := $(addprefix server/,  ${CONFIG_PARSER} \
								Server.cpp Response.cpp RequestParser.cpp RequestParserUtils.cpp Methods.cpp)

SRCS := main.cpp \
		${SERVER}

OBJS := $(patsubst %.cpp, ${OBJDIR}/%.o, ${SRCS})

HEADER := $(addprefix inc/, Colors.hpp ConfigParser.hpp Exceptions.hpp \
							Server.hpp Response.hpp RequestParser.hpp Methods.hpp)

INCLUDES := -I ${INCLUDES_DIR}

#Production flags
CXXFLAGS := -Wall -Wextra -Werror
CXXFLAGS += -std=c++98 -pedantic -g -O2

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

test_request_parser:
	c++ src/server/testParse.cpp src/server/RequestParser.cpp src/server/RequestParserUtils.cpp -I inc -g -O0 -o test_request_parser.out
	./test_request_parser.out

test_config_parser:
	c++ src/server/config_parser/WebServerConfig.cpp \
		 src/server/config_parser/test_config_parser.cpp \
		 src/server/config_parser/NewConfigParser.cpp \
		 src/server/config_parser/LocationConfig.cpp \
		 src/server/config_parser/ServerConfig.cpp -o test_config_parser.out -I inc src/server/config_parser/validateValues.cpp -g -O0
# ./test_config_parser.out

.PHONY: all clean fclean re test_request_parser test_config_parser

.NOTPARALLEL: