# The compiler.
CC              = gcc

# Compiler flags.
CCFLAGS = -Wall -o -I/usr/local/include

# The target executable name.
TARGET  = clearbrew

# The source to compile.
SRC             = $(CURDIR)/*.c $(CURDIR)/*/*.c

# The included libraries.
LIBS    = -lintelfpgaup -lm -L/usr/local/lib -lcurl

# Compile the source file:
$(TARGET): $(SRC)
		@$(CC) $(CCFLAGS) $(TARGET) $(SRC) $(LIBS)


# Remove the target (the executable).
.PHONY: clean
clean:
		@rm -f $(TARGET)