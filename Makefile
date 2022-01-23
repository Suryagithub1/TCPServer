

OBJ_NAME = $(PKG_NAME)

LIBS = 
CPP = g++
CPPFLAGS =      -g -Wall -fstack-protector-all -Wstack-protector -fno-defer-pop -Wformat -Wformat-security -fexceptions -finline-functions -fPIC -O2 -Wwrite-strings -Wparentheses -fpermissive -std=c++0x                    
SRC = src
BIN = bin
LOCAL_INCLUDE_PATH = ./include/

INCLUDE =           -I $(LOCAL_INCLUDE_PATH)
OBJ = $(BIN)/$(OBJ_NAME)

INSTALL_DIR = /opt/$(OBJ_NAME)

OBJFILES := $(patsubst $(SRC)/%.cpp,$(BIN)/%.o,$(wildcard $(SRC)/*.cpp))

all: bin OBJ

OBJ: $(OBJFILES)
	$(CPP) $(CFLAGS) $(OBJFILES) $(LIBS) -o $(OBJ)  

$(BIN)/%.o:$(SRC)/%.cpp $(LOCAL_INCLUDE_PATH)/*.h 
	$(CPP) $(INCLUDE) -c $< -o $@  $(CPPFLAGS)

bin:
	mkdir -p $(BIN)

install:
	test -d $(INSTALL_DIR) ||  mkdir $(INSTALL_DIR)
	cp config $(INSTALL_DIR) -r
	cp bin $(INSTALL_DIR) -r
	cp README $(INSTALL_DIR)

clean:
	rm $(BIN)/* -rf

run:
	./$(OBJ) -c config/config.yaml

