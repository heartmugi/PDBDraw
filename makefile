.SUFFIXES: .c .o .a 

LIBS=-lGL -lGLU -lglut -lm
#LIBS=-framework GLUT -framework OpenGL -lm
#LIBS=-lglut32 -lglu32 -lopengl32 -DWIN32 -lm

SOURCE= \
	myInit.c \
	myLight.c \
	myIdle.c \
	myCamera.c \
	myKeyboard.c \
	myMouse.c \
	myReshape.c \
	myDisplay.c \
	myOperation.c \
	myFileLists.c \
	myBack.c \
	myHelp.c \
	lpdbMinTemp.c \
	lpdbMaxTemp.c \
	lpdbMinOccu.c \
	lpdbMaxOccu.c \

SOURCE_PDB = \
	pdbRead.c \
	lpdbCenterCalc.c \
	lpdbSizeCalc.c \
	lpdbMinCalc.c \
	lpdbMaxCalc.c \
	lpdbResCount.c \
	lpdbEleCount.c \
	lpdbInfoPrint.c \

.c.o:
	gcc -g -Wall -c $*.c -o $*.o
	#gcc -g -c -Wall $*.c -o $*.o -Wno-deprecated
	#gcc -g -Wall -DWIN32 -c $*.c -o $*.o

all: myTest

win: myTest.exe

myTest:myTest.o libmyGLUT.a lpdbInfoPrint.o lpdbCenterCalc.o lpdbSizeCalc.o lpdbMinCalc.o lpdbMaxCalc.o lpdbResCount.o lpdbEleCount.o lpdbFree.o pdbRead.o
	gcc -g -Wall -o $@ myTest.o lpdbInfoPrint.o lpdbCenterCalc.o lpdbSizeCalc.o lpdbMinCalc.o lpdbMaxCalc.o lpdbResCount.o lpdbEleCount.o lpdbFree.o pdbRead.o lpdbMinTemp.o lpdbMaxTemp.o -L./ -lmyGLUT -lm $(LIBS)

PDBDraw:myTest.o libmyGLUT.a lpdbInfoPrint.o lpdbCenterCalc.o lpdbSizeCalc.o lpdbMinCalc.o lpdbMaxCalc.o lpdbResCount.o lpdbEleCount.o lpdbFree.o pdbRead.o
	gcc -g -Wall -o $@ myTest.o lpdbInfoPrint.o lpdbCenterCalc.o lpdbSizeCalc.o lpdbMinCalc.o lpdbMaxCalc.o lpdbResCount.o lpdbEleCount.o lpdbFree.o pdbRead.o lpdbMinTemp.o lpdbMaxTemp.o -L./ -lmyGLUT -lm $(LIBS)

myTest.exe: myTest.o libmyGLUT.a
	gcc -g -Wall -o $@ myTest.o -L./ -lmyGLUT $(LIBS) -lm

libmyGLUT.a: $(SOURCE:.c=.o)
	ar r $@ $(SOURCE:.c=.o)

libmyPDB.a: $(SOURCE_PDB:.c=.o)
	ar r $@ $(SOURCE_PDB:.c=.o)

test:
	./myTest 121p-GC.pdb

testP:
	./PDBDraw 121p-GC.pdb

clean:
	rm -rf *.o *.exe *.a myTest 

depend: $(SOURCE)
	gcc -M *.c > dependency

