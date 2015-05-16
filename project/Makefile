CPP = g++
CPPFLAGS = -Wall -g
LDFLAGS = 

TARGET = yurtcraft demo


### These lines are for access to the VRG3D and G3D installation
### directories. All other necessary software is assumed to be
### installed in the system-wide directories.

ifeq ($(shell if [ -d /research/graphics ] ; then echo 1; fi),1)
  # CS Dept
  EXTRA_INCLUDES = -I$(G)/install_linux/include -I$(G)/install_linux/include/vrg3d
  EXTRA_LIBS = -L$(G)/install_linux/lib -L$(G)/install_linux/lib/vrg3d
else
  # CCV - Granoff
  EXTRA_INCLUDES = -I/share/cave/include
  EXTRA_LIBS = -L/share/cave/lib
endif


OBJDIR = obj

SRCS = $(shell ls *.cpp)

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

INCLUDE_DIRS = -I. $(EXTRA_INCLUDES)
INCLUDE_DIRS += -I./lib/include

LIB_DIRS = $(EXTRA_LIBS)
LIB_DIRS += -L./lib

LIBS += -Xlinker --start-group -Xlinker -ldl  -Xlinker -lX11 -Xlinker \
	-lXext  -Xlinker -lpthread  -Xlinker -lz  -Xlinker -ljpeg  -Xlinker \
	-lpng  -Xlinker -lzip  -Xlinker -lSDL -Xlinker -lvrg3d  -Xlinker \
	-lavutil  -Xlinker -lavformat  -Xlinker -lavcodec  -Xlinker -lGLG3Dd \
	-Xlinker -lG3Dd -Xlinker -lGLU -Xlinker -lGL -Xlinker --end-group \
	-lvrpn -lglut -lXmu

all: yurtcraft

yurtcraft:  obj/vrfrontend.o obj/main.o obj/vrcamera.o obj/sampleapp.o obj/aab.o obj/chunk.o obj/chunkfactory.o obj/collisions.o obj/cylinder.o obj/ellipsoid.o obj/entity.o obj/graphics.o obj/mcchunk.o obj/mcchunkfactory.o obj/mcworld.o obj/noise.o obj/obj.o obj/player.o obj/skybox.o obj/triangle.o obj/voxelsystem.o obj/world.o obj/biomemanager.o obj/biometile.o obj/terrainfeature.o obj/biome.o obj/mountainbiome.o
	$(CPP) $(LDFLAGS) -o yurtcraft $^ $(LIB_DIRS) $(LIBS)


#$(TARGET) : $(OBJS) 
#	$(CPP) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIB_DIRS) $(LIBS)

$(OBJDIR)/%.o: src/%.cpp
	mkdir -p $(OBJDIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@ $(INCLUDE_DIRS)

clean:
	rm -f $(TARGET) $(OBJDIR)/*.o demo-log.txt
	rm -rf $(OBJDIR)
