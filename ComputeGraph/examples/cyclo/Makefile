# Makefile for MSVC compiler on Windows
SHELL = cmd
CC = cl.exe
RM = del /Q /F

INCLUDES = /Igenerated /I../../cg/src /I.
WINFLAGS = /DWIN32 /D_WINDOWS /EHsc /Zi /Ob0 /Od /RTC1 -MDd 
CFLAGS =  $(INCLUDES) $(WINFLAGS)

all:
	$(CC) /Fecyclo.exe $(CFLAGS) generated/scheduler.cpp main.cpp

clean:
	$(RM) main.obj
	$(RM) scheduler.obj 
	$(RM) cyclo.ilk 
	$(RM) cyclo.exe 
	$(RM) *.pdb 