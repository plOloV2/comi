#!/bin/bash

comp_time() {
    local start=${EPOCHREALTIME/./}
    "$@"
    local exit_code=$?
    echo >&2 "Took ~$(( (${EPOCHREALTIME/./} - start)/1000 )) ms."
    return ${exit_code}
}

# Set file locations
SOURCE_FILES_LOCATION="src"
BIN_FILES_LOCATION="bin"

# Set compiling flags for gcc
FLAGS="-fopenmp"

# Set the source file and output binary names
SOURCE_FILE="comi.c"
DEPEN_1_FILE="dis.s"
DEPEN_2_FILE="base.s"
OUTPUT_BINARY="comi"

# Check if all files are present
if [ ! -f "$SOURCE_FILES_LOCATION/$SOURCE_FILE" ]; then
    echo "Error: Source file '$SOURCE_FILE' not found!"
    exit 1
fi

if [ ! -f "$SOURCE_FILES_LOCATION/$DEPEN_1_FILE" ]; then
    echo "Error: Source file '$DEPEN_1_FILE' not found!"
    exit 1
fi

if [ ! -f "$SOURCE_FILES_LOCATION/$DEPEN_2_FILE" ]; then
    echo "Error: Source file '$DEPEN_2_FILE' not found!"
    exit 1
fi

mkdir -p "$BIN_FILES_LOCATION"

# Compile the source file
echo "Compiling program with flags $FLAGS"
comp_time gcc "$SOURCE_FILES_LOCATION/$SOURCE_FILE" "$SOURCE_FILES_LOCATION/$DEPEN_1_FILE" "$SOURCE_FILES_LOCATION/$DEPEN_2_FILE" -o "$BIN_FILES_LOCATION/$OUTPUT_BINARY" "$FLAGS" -g

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Output binary is $OUTPUT_BINARY"
else
    echo "Compilation failed."
    exit 1
fi
