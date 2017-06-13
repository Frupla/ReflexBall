#
# ZDS II Make File - ArkanoidEngine, Debug
#

CC = @C:\PROGRA~2\ZiLOG\ZDSII_~1.3\bin\eZ8cc
ASM = @C:\PROGRA~2\ZiLOG\ZDSII_~1.3\bin\eZ8asm
LINK = @C:\PROGRA~2\ZiLOG\ZDSII_~1.3\bin\eZ8link
LIB = @C:\PROGRA~2\ZiLOG\ZDSII_~1.3\bin\eZ8lib

# compiler options
#   -alias -asm -bitfieldsize:32 -charsize:8 -const:RAM -debug
#   -define:_Z8F4802 -define:_Z8ENCORE_640_FAMILY
#   -define:_Z8ENCORE_F640X -define:_MODEL_LARGE -doublesize:32
#   -NOexpmac -floatsize:32 -NOfplib -genprintf -NOglobalcopy
#   -NOglobalcse -NOglobalfold -intrinsic -intsize:16 -intsrc -NOjmpopt
#   -NOkeepasm -NOkeeplst -NOlist -NOlistinc -localcopy -localcse
#   -localfold -longsize:32 -NOloopopt -maxerrs:50 -model:L -NOoptlink
#   -optsize -peephole -NOpromote -quiet -regvar -revaa -NOsdiopt
#   -shortsize:16
#   -stdinc:"C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std;C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog;C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\Z8Encore_F640X"
#   -strict -NOwatch -cpu:Z8F4802
#   -asmsw:" -cpu:Z8F4802 -define:_Z8F4802=1 -define:_Z8ENCORE_640_FAMILY=1 -define:_Z8ENCORE_F640X=1 -define:_MODEL_LARGE=1 -include:C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std;C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog;C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\Z8Encore_F640X -revaa"
CFLAGS = -sw:C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ArkanoidEngine_Debug.ccsw
# assembler options
#   -debug -define:_Z8F4802=1 -define:_Z8ENCORE_640_FAMILY=1
#   -define:_Z8ENCORE_F640X=1 -define:_MODEL_LARGE=1 -genobj -NOigcase
#   -include:"C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std;C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog;C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\Z8Encore_F640X"
#   -list -NOlistmac -name -pagelen:56 -pagewidth:80 -quiet -sdiopt
#   -warn -NOzmasm -revaa -cpu:Z8F4802
AFLAGS = -sw:C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ArkanoidEngine_Debug.asmsw

OUTDIR = C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\

Debug: ArkanoidEngine

deltarget: 
	@if exist C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ArkanoidEngine.lod  \
            del C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ArkanoidEngine.lod

clean: 
	@if exist C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ArkanoidEngine.lod  \
            del C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ArkanoidEngine.lod
	@if exist C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\zsldevinit.obj  \
            del C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\zsldevinit.obj
	@if exist C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ansi.obj  \
            del C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ansi.obj
	@if exist C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\engine.obj  \
            del C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\engine.obj
	@if exist C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\sinLUT.obj  \
            del C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\sinLUT.obj
	@if exist C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\Z8encore.obj  \
            del C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\Z8encore.obj
	@if exist C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\test.c.obj  \
            del C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\test.c.obj
	@if exist C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\math.obj  \
            del C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\math.obj
	@if exist C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\standalone_timer.obj  \
            del C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\standalone_timer.obj

rebuildall: clean Debug

relink: deltarget Debug

LIBS = 

OBJS =  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\zsldevinit.obj  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ansi.obj  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\engine.obj  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\sinLUT.obj  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\Z8encore.obj  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\test.c.obj  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\math.obj  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\standalone_timer.obj

ArkanoidEngine: $(OBJS)
	 $(LINK)  @C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ArkanoidEngine_Debug.linkcmd

C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\zsldevinit.obj :  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\zsldevinit.asm  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\ez8dev.inc  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\gpio.inc
	 $(ASM)  $(AFLAGS) C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\zsldevinit.asm

C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ansi.obj :  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ansi.c  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h
	 $(CC)  $(CFLAGS) C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ansi.c

C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\engine.obj :  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\engine.c  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\Z8encore.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ansi.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\math.h
	 $(CC)  $(CFLAGS) C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\engine.c

C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\sinLUT.obj :  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\sinLUT.c  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\sinLUT.h
	 $(CC)  $(CFLAGS) C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\sinLUT.c

C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\Z8encore.obj :  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\Z8encore.c  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ansi.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\charset.h
	 $(CC)  $(CFLAGS) C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\Z8encore.c

C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\test.c.obj :  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\test.c.c  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\Z8encore.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\ansi.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\engine.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\math.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\sinLUT.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\STANDA~1.H
	 $(CC)  $(CFLAGS) C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\test.c.c

C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\math.obj :  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\math.c  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\sinLUT.h
	 $(CC)  $(CFLAGS) C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\math.c

C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\standalone_timer.obj :  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\standalone_timer.c  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~2\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\STANDA~1.H
	 $(CC)  $(CFLAGS) C:\Users\tilfr\Dropbox\PROGRA~1\Project\REFLEX~1\standalone_timer.c

