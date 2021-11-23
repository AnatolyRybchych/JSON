
CC = g++
out = ./static/JSON.o
additionalLibs = 
linkArgs = -shared 
compileArgs = -c -g
OBJDIR = ./objects/
SRC_TYPE= '\.cpp'

#src dirs
SRC_DIR  						=	./src/

#ALL SOURCE FILES SHOULD HAVE UNIQUE NAME
#src files
src/.cpp   						=	$(shell ls -r $(SRC_DIR) | grep $(SRC_TYPE))


#src files grouped
srcFiles =   $(src/.cpp)

#object files from src files
objects = $(patsubst %,$(OBJDIR)%,$(srcFiles:.cpp=.o))

#link all objects together
build: $(objects) 
	@mkdir -p $(dir $(out))
	$(CC) $(linkArgs) -o $(out) $+ $(additionalLibs) -g


#compile all objects
$(OBJDIR)%.o:$(SRC_DIR)$(notdir %.cpp)							#./src/
	$(CC) $(compileArgs) -o $@ $+


