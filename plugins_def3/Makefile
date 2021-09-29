TARGET_DIR 	= test
TARGET  	= $(TARGET_DIR)/Hello_py.1.2.3.so

CC			= g++ -std=c++11
LIB			=
LDFLAGS 	=
DEFINES 	=
INCLUDE 	= -I./include
CFLAGS  	=  $(DEFINES) $(INCLUDE)
CXXFLAGS	= $(CFLAGS)
SHARE   	= -fPIC -shared -o

OBJ_DIR 	= ./obj
OBJ			= $(patsubst %.cc, $(OBJ_DIR)/%.o, $(wildcard *.cc))

RM 			= rm -rf

all : $(TARGET)

objs : $(OBJ)

clean :
	@echo Cleaning....
	@if [ -d $(OBJ_DIR) ]; then \
		$(RM) $(OBJ_DIR) ;\
	fi
	@if [ -d $(TARGET_DIR) ]; then \
		rm -rf  $(TARGET) ;\
	fi
	@echo Cleaning.....finished!
	@echo


$(OBJ_DIR)/%.o : %.cc
	@mkdir -p $(OBJ_DIR)
	@echo Compiling: $< 
	$(CC) $(CFLAGS) -c $< $(SHARE) $@ 
	@echo

$(TARGET) : $(OBJ)
	@mkdir -p $(TARGET_DIR)
	@echo Compiling: $@
	$(CC) $(CXXFLAGS) $(SHARE) $@ $(OBJ) $(LDFLAGS) $(LIB)