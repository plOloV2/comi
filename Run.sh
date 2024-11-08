#!/bin/bash

errors() {
    local error_code=$?
    if [ $error_code -eq 0 ]; then
       echo -e "\nExecuted succesfuly"
    else
         if [ $error_code -eq 1 ]; then
            echo "Error: argc not equal 2. Program returned with code 1."
        elif [ $error_code -eq 2 ]; then
            echo "Error: PrepareData() error. Program returned with code 2."
        elif [ $error_code -eq 3 ]; then
            echo "Error: CalculateStartDist() failed. Program returned with code 3."
        elif [ $error_code -eq 4 ]; then
            echo "Error: PrepareThreads() error. Program returned with code 4."
        elif [ $error_code -eq 5 ]: then
            echo "Error: CalculateBestRoad() failed. Program returned with code 5."
        fi
    fi
}


# Set the output binary name
OUTPUT_BINARY="comi"

# Set scripts location
SCRIPTS_LOCATION="scripts"

# Set bin location
BIN_FILES_LOCATION="bin"

# Compile the source file
./"$SCRIPTS_LOCATION"/compile.sh

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    if [ -f "$BIN_FILES_LOCATION/$OUTPUT_BINARY" ]; then
        echo -e "\nRunning the program with args: $@\n"
        ./"$BIN_FILES_LOCATION/$OUTPUT_BINARY" "$@"
        errors
    else
        echo "Error: Compiled binary '$BIN_FILES_LOCATION/$OUTPUT_BINARY' not found!"
        exit 1
    fi
else
    echo "Compilation failed. The program will not be run."
    exit 1
fi
