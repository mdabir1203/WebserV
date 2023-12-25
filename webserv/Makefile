NAME := webserv

OBJDIR := obj

SRCDIR := src

INCLUDES_DIR := inc

CONFIG_PARSER := $(addprefix config_parser/, CheckChar.cpp ConfigParser.cpp validateValues.cpp)
CONFIG_OBJECTS := $(addprefix config_objects/, CGIConfig.cpp LocationConfig.cpp ServerConfig.cpp WebServerConfig.cpp)

SERVER := $(addprefix server/,  ${CONFIG_PARSER} \
								${CONFIG_OBJECTS} \
								Server.cpp Response.cpp RequestParser.cpp RequestParserUtils.cpp Methods.cpp \
								LookupConfig.cpp)

SRCS := main.cpp \
		${SERVER}

OBJS := $(patsubst %.cpp, ${OBJDIR}/%.o, ${SRCS})

HEADER := $(addprefix inc/, Colors.hpp ConfigParser.hpp \
							Server.hpp Response.hpp RequestParser.hpp Methods.hpp \
							LookupConfig.hpp)

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
	${RM} test_config_parser.out
	${RM} test_request_parser.out

re: fclean all

test_request_parser:
	c++ ${CXXFLAGS} src/server/test_request_parser.cpp src/server/RequestParser.cpp src/server/RequestParserUtils.cpp -I inc -g -O0 -o test_request_parser.out

#test_config_parser - START
OBJS_TEST_CONFIG_PARSER := $(patsubst %.cpp, ${OBJDIR}/%.o, $(addprefix server/, ${CONFIG_PARSER} ${CONFIG_OBJECTS} config_parser/test_config_parser.cpp LookupConfig.cpp))
CXXFLAGS = -Wall -Wextra -Werror -g -O0 ${INCLUDES}
test_config_parser: ${OBJS_TEST_CONFIG_PARSER}
	${CXX} ${CXXFLAGS} ${OBJS_TEST_CONFIG_PARSER} -o test_config_parser.out

${OBJDIR}/%.o: ${SRCDIR}/%.cpp ${HEADER} | ${OBJDIR}
	mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} -c $< -o $@ 
#test_config_parser - END

.PHONY: all clean fclean re test_request_parser test_config_parser

.NOTPARALLEL: