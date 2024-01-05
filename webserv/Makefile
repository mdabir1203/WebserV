# Variables
NAME			:= webserv
CXX				:= c++
RM				:= rm -f

# Directories
OBJDIR			:= obj
SRCDIR			:= src
INCLUDES_DIR	:= inc

# Include paths
INCLUDES		:= -I ${INCLUDES_DIR}

# Source files for the config parser
CONFIG_PARSER := $(addprefix config_parser/, \
    CheckChar.cpp \
    ConfigParser.cpp \
    validateValues.cpp \
)

# Source files for the config objects
CONFIG_OBJECTS := $(addprefix config_objects/, \
    CGIConfig.cpp \
    LocationConfig.cpp \
    ServerConfig.cpp \
    WebServerConfig.cpp \
)

# Source files for the server
SERVER := $(addprefix server/, \
    ${CONFIG_PARSER} \
    ${CONFIG_OBJECTS} \
    Server.cpp \
    Response.cpp \
    RequestParser.cpp \
    RequestParserUtils.cpp \
    RequestParserUri.cpp \
    Methods.cpp \
    LookupConfig.cpp \
    directory_listing.cpp \
    file_locking.cpp \
    ClientState.cpp \
)

# All source files
SRCS := \
    main.cpp \
    ${SERVER}

# Object files corresponding to each source file
OBJS := $(patsubst %.cpp, ${OBJDIR}/%.o, ${SRCS})

# ---Compiler Flags---
# Production flags
CXXFLAGS := -Wall -Wextra -Werror
CXXFLAGS += -std=c++98 -pedantic
# CXXFLAGS += -O2

# Development flags
CXXFLAGS += -g -O0
# CXXFLAGS += -fsanitize=thread
# CXXFLAGS += -fsanitize=address

# Add includes
CXXFLAGS += ${INCLUDES}

# Generating dependencies for user headers automaticly
CXXFLAGS += -MMD 


# Rules
all: ${NAME}

${NAME}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

${OBJDIR}/%.o: ${SRCDIR}/%.cpp | ${OBJDIR}
	mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} -c $< -o $@

${OBJDIR}:
	mkdir -p ${OBJDIR}

clean:
	${RM} -r ${OBJDIR}

fclean: clean
	${RM} ${NAME}

re: fclean all

# adding the dependcies as rules and not complainging (-) if file not existent
-include ${OBJS:.o=.d}

.PHONY: all clean fclean re
.NOTPARALLEL: