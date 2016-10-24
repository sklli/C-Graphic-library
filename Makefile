# Variable definitions.

PLATFORM	= $(shell uname)
CC		= clang
OBJDIR		= ./objs
SRC = ./src
TESTS		= ./tests
INCLUDES	= ./include
INCFLAGS	:= -I${INCLUDES}
#OPTFLAGS	:= -Os -DNDEBUG
OPTFLAGS	:= -g
CCFLAGS		:= -c ${OPTFLAGS} -Wall -std=c99

# The list of objects to include in the library

LIBEIOBJS	:=  ${OBJDIR}/ei_application.o ${OBJDIR}/ei_geometrymanager.o ${OBJDIR}/ei_widget.o ${OBJDIR}/ei_class_frame.o  ${OBJDIR}/ei_widgetclass.o ${OBJDIR}/ei_new_types.o ${OBJDIR}/ei_draw_widget.o  ${OBJDIR}/relief.o ${OBJDIR}/ei_class_button.o ${OBJDIR}/ei_event.o ${OBJDIR}/ei_callback.o ${OBJDIR}/ei_class_toplevel.o




# Platform specific definitions (OS X, Linux, Windows)

ifeq (${PLATFORM},Darwin)

	# Building for Mac OS X

	PLATDIR		= _osx
	INCFLAGS	:= ${INCFLAGS} -I/usr/local/include -I/usr/local/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase.a
	LIBS		= ${LIBEIBASE} -L/usr/local/lib -lfreeimage -lSDL -lSDL_ttf -lSDL_gfx -framework AppKit -lm

else
	ifeq (${PLATFORM},Linux)

	# Building for Linux

	PLATDIR		= _x11
	INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
#	ARCH	        = 32
	ARCH	        = 64
	LIBEIBASE	= ${PLATDIR}/libeibase${ARCH}.a
	LIBS		= ${LIBEIBASE} -L${PLATDIR} -lSDL -lSDL_ttf -lSDL_gfx${ARCH} -lfreeimage${ARCH} -lm -lstdc++

	else

	# Building for Windows

	PLATDIR		= _win
	INCFLAGS	:= ${INCFLAGS} -I/usr/include -I/usr/include/SDL -I/usr/local/include/SDL
	LINK		= ${CC} -mwindows -mno-cygwin
	LIBEI		= ${OBJDIR}/libei.lib
	LIBEIBASE	= ${PLATDIR}/libeibase.lib
	LIBS		= -lmingw32 ${LIBEIBASE} /usr/lib/SDL_ttf.lib /usr/local/lib/libSDL_gfx.a -L/usr/lib -lSDL -lFreeImage

	endif
endif



# Main target of the makefile. To build specific targets, call "make <target_name>"

TARGETS		=	${LIBEI} \
			minimal frame button hello_world \
			puzzle minesweeper two048 parsing
all : ${TARGETS}



########## Test-programs

# minimal

minimal : ${OBJDIR}/minimal.o ${LIBEIBASE}
	${LINK} -o minimal ${OBJDIR}/minimal.o ${LIBS}

${OBJDIR}/minimal.o : ${TESTS}/minimal.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minimal.c -o ${OBJDIR}/minimal.o

# frame

frame : ${OBJDIR}/frame.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame ${OBJDIR}/frame.o ${LIBEI} ${LIBS}

${OBJDIR}/frame.o : ${TESTS}/frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame.c -o ${OBJDIR}/frame.o

# button

button : ${OBJDIR}/button.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button ${OBJDIR}/button.o ${LIBEI} ${LIBS}

${OBJDIR}/button.o : ${TESTS}/button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button.c -o ${OBJDIR}/button.o

# hello_world

hello_world : ${OBJDIR}/hello_world.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world ${OBJDIR}/hello_world.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world.o : ${TESTS}/hello_world.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world.c -o ${OBJDIR}/hello_world.o

# parsing

parsing : ${OBJDIR}/parsing.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o parsing ${LDFLAGS} ${OBJDIR}/parsing.o ${LIBEI} ${LIBS}

${OBJDIR}/parsing.o : ${TESTS}/parsing.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/parsing.c -o ${OBJDIR}/parsing.o

# puzzle

puzzle : ${OBJDIR}/puzzle.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle ${OBJDIR}/puzzle.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle.o : ${TESTS}/puzzle.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle.c -o ${OBJDIR}/puzzle.o

# two048

two048 : ${OBJDIR}/two048.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o two048 ${OBJDIR}/two048.o ${LIBEI} ${LIBS}

${OBJDIR}/two048.o : ${TESTS}/two048.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/two048.c -o ${OBJDIR}/two048.o

# minesweeper

minesweeper : ${OBJDIR}/minesweeper.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o minesweeper ${OBJDIR}/minesweeper.o ${LIBEI} ${LIBS}

${OBJDIR}/minesweeper.o : ${TESTS}/minesweeper.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minesweeper.c -o ${OBJDIR}/minesweeper.o

# Building of the library libei

${LIBEI} : ${LIBEIOBJS}
	ar -rcs ${LIBEI} ${LIBEIOBJS}



# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg


$(OBJDIR)/%.o: $(SRC)/%.c
	$(CC) $(CCFLAGS) $(INCFLAGS) $(LIBS) -c $^ -o $@
# Removing all built files.

clean:
	rm -f ${TARGETS}
	rm -f *.exe
	rm -f ${OBJDIR}/*
