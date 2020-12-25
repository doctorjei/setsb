CPP = i586-pc-msdosdjgpp-g++
SOURCE_DIR = Source
BUILD_DIR = Build

SOURCE_FILES = Source/setsb.cpp
EXECUTABLE = setsb.exe

CPPFLAGS = -O1

all: $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): $(SOURCE_FILES)
	$(CPP) $(CPPFLAGS) -o $(BUILD_DIR)/$(EXECUTABLE) $?

clean:
	rm $(BUILD_DIR)/$(EXECUTABLE)
