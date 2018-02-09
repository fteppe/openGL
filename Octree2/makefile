SRC = Camera.cpp Cube.cpp CubeMap.cpp EventHandler.cpp GameObject.cpp Light.cpp main.cpp Material.cpp Octree.cpp Polygon.cpp Scene.cpp SceneLoader.cpp ShaderBasic.cpp Shader.cpp ShaderPBR.cpp ShaderSpecular.cpp Solid.cpp stdafx.cpp Texture.cpp Vec3.cpp VertexBufferObject.cpp waveFrontLoader.cpp WindowBuilder.cpp
BUILDDIR = ./linuxBuild/
#OBJ = $(SRC:.cpp=.o)
OBJ = $(SRC:%.cpp=$(BUILDDIR)%.o)
DEPENDFILES = $(OBJ:%.o=%.dep)
CXX = g++
FLAGS = -Wall
INCLUDES = -I ./Dependencies/
LIB = -lGL -lGLU -lGLEW -lsfml-graphics -lsfml-window -lsfml-system
EXE = app



all: $(EXE)
	echo "building app"
#echo $(OBJ)
#echo $(DEPENDFILES)

$(EXE) : $(OBJ) $(DEPENDFILE)
	$(CXX)  -g   $(OBJ) $(LIB) -o $(EXE)

$(BUILDDIR)%.o:
	$(CXX) -g -c $< $(FLAGS) $(INCLUDES) -o $@

$(BUILDDIR)%.dep : %.cpp %.h
	g++ -MM -MT $(<:%.cpp=$(BUILDDIR)%.o) $< > $@

$PHONY test:
	echo $(OBJ)

$PHONY clean:
	rm $(BUILDDIR)*.o

-include $(DEPENDFILES)