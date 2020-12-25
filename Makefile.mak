CPP = tcc
SOURCE_DIR = Source
BUILD_DIR = Build

SOURCE_FILES = Source\setsb.cpp
EXECUTABLE = setsb.exe

CPPFLAGS = -G -n$(BUILD_DIR)

all: $(BUILD_DIR)\$(EXECUTABLE)

$(BUILD_DIR)\$(EXECUTABLE): $(SOURCE_FILES)
	$(CPP) $(CPPFLAGS) -e$(EXECUTABLE) $?

clean:
	del $(BUILD_DIR)\$(EXECUTABLE)
	del $(BUILD_DIR)\*.obj
