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
.PATH: ${.CURDIR}/drivers/serial_port/
.PATH: ${.CURDIR}/game/
.PATH: ${.CURDIR}/game/sprite/
.PATH: ${.CURDIR}/game/state_machine/
.PATH: ${.CURDIR}/game/int_handlers/dependent/
.PATH: ${.CURDIR}/game/int_handlers/independent/
.PATH: ${.CURDIR}/game/int_handlers/
.PATH: ${.CURDIR}/assets/
.PATH: ${.CURDIR}/game/objects/
.PATH: ${.CURDIR}/game/views/
.PATH: ${.CURDIR}/game/protocol

#SRCS = $(wildcard *.c)
SRCS = video_graphic.c kbc.c keyboard.c mouse.c utils.c timer.c game.c proj.c sprite.c mouse_st.c independent_ih.c dependent_ih.c menu_st.c pieces.c views.c font.c rtc.c uart.c communication_protocol.c animation.c

IPATHS=-I./libs/graph/ -I./libs/kbc/ -I./libs/rtc/-I./libs/timer/ -I./libs/keyboard/ -I./libs/mouse/  -I./libs/mouse/ 


# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D __LCOM_OPTIMIZED__ 

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf


# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
