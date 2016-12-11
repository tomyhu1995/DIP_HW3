CC = gcc
CPP = g++

CFLAGS = 
CFLAGS += -g
#CFLAGS += -Wall
CFLAGS += -std=c99

CPPFLAGS = 
CPPFLAGS += -g
#CPPFLAGS += -Wall

CFLAGS_CPP =
CFLAGS_CPP += -O3 -pipe -fomit-frame-pointer -funroll-all-loops -s

INC = 
INC += -I .
INC += -I ./

SRC =
SRC += ./BMP_file_head
SRC += ./BMP_info_head
SRC += ./raw_data
SRC += ./IMG_CREATE
SRC += ./img_cover
SRC += ./img_analysis
SRC += ./filter
SRC += ./LINE_detect

SRC_CPP = 
SRC_CPP += ./EasyBMP

LDFLAG =
LDFLAG += -lm

SRC_FILES = ${addsuffix .c , ${SRC}}
OBJ_FILES = ${addsuffix .o , ${SRC}}

SRC_FILES_CPP = ${addsuffix .cpp , ${SRC_CPP}}
OBJ_FILES_CPP = ${addsuffix .o , ${SRC_CPP}}

MAIN = BMP_open.cpp

EXE = BMP_EXE

all: ${EXE}
${EXE} : ${OBJ_FILES} ${OBJ_FILES_CPP}
	${CPP} ${CPPFLAGS} ${INC} ${OBJ_FILES} ${OBJ_FILES_CPP} ${MAIN} -o $@ ${LDFLAG}
${OBJ_FILES} : %.o : %.c
	${CC} ${CFLAGS} ${INC} -c $< -o $@ ${LDFLAG}
${OBJ_FILES_CPP} : %.o : %.cpp
	${CPP} ${CPPFLAGS} ${CFLAGS_CPP} ${INC} -c $< -o $@
clean :
	rm ${OBJ_FILES} ${OBJ_FILES_CPP} ${EXE} *.bmp *.txt
