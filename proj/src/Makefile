# name of the program (Minix service)
PROG=proj

# source code files to be compiled
.PATH: ${.CURDIR}/drivers/graph/
.PATH: ${.CURDIR}/drivers/kbc/
.PATH: ${.CURDIR}/drivers/rtc/
.PATH: ${.CURDIR}/drivers/timer/
.PATH: ${.CURDIR}/drivers/keyboard/
.PATH: ${.CURDIR}/drivers/mouse/
.PATH: ${.CURDIR}/drivers/utils/
.PATH: ${.CURDIR}/game/
.PATH: ${.CURDIR}/game/sprite/
.PATH: ${.CURDIR}/game/rgb/
.PATH: ${.CURDIR}/game/state_machine/

#SRCS = $(wildcard *.c)
SRCS = video_graphic.c kbc.c keyboard.c mouse.c utils.c timer.c game.c proj.c sprite.c rgb.c state_machine.c

IPATHS=-I./libs/graph/ -I./libs/kbc/ -I./libs/rtc/-I./libs/timer/ -I./libs/keyboard/ -I./libs/mouse/  -I./libs/mouse/  
# SRC = src
# OBJ = obj
# 
# SOURCES := $(wildcard $(SRC)/*.c)
# OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
# 
# all: $(OBJECTS) $(CC) $^ -o $@
# 
# $(OBJ)/%.o: $(SRC)/%.c $(CC) -I$(SRC) -c $< -o $@


#OBJDIR=obj
#
#all:   $(EXECUTABLE)
#
#$(EXECUTABLE):  $(OBJECTS) $(CC) $(CFLAGS) -o $@ $(RUNTIME_PATH) $(OBJECTS) $(LIB_PATH) $(LIBS)
#
#$(OBJECTS): src/%.o : src/%.c $(CC) $(CFLAGS) -c $< $(LIB_PATH) $(LIBS) -o $@



# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic 

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf


# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
