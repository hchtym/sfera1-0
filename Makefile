CXX	= ../compilor/curver/usr/bin/arm-linux-g++
LD	= ../compilor/curver/usr/bin/arm-linux-g++
        
LDFLAGS = -lm -lpthread -static-libgcc

#uncomment this when using sqlite
#LDFLAGS += -ldl

CFLAGS = $(PRE_CFLAGS)
CFLAGS += -g -Wall  -I../sdk/ -I../sfc/ -I.

CPPFLAGS = $(PRE_CFLAGS)
CPPFLAGS += -fPIC -DLINUX 

#modify the SRC and EXE accrod with your application
SOURCES = $(wildcard *.cpp)

OBJ = $(patsubst %.cpp,%.o,$(SOURCES)) ../sfc/sfc.a ../sdk/libscl.a ../sdk/libCBFL.a
#OBJ += ../sqlite/.libs/libsqlite3.a

EXE = sfera

all:$(EXE)

$(EXE):$(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^
	cp $(EXE) ../output

%.o:%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(INCLUDES) $(LDADD) $(LIBS)

clean:
	rm -f *.o $(EXE)
	
