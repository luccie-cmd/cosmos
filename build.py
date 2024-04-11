#!/bin/python3

import os, glob

def check_file_extension(file_path, valid_extensions):
    try:
        file_name = file_path.split('/')[-1]  # Extract the file name from the file path
        if '.' in file_name:
            file_extension = file_name.split('.')[-1]  # Extract the file extension
            if file_extension in valid_extensions:
                return True
            else:
                return False
        else:
            print(f"No file extension found in '{file_name}'.")
    except IndexError:
        print(f"Invalid file path: '{file_path}'")

def main(directory_path: str):
    print("Compiling all the files")
    os.system("mkdir -p build")
    os.system("rm -f ./build/main")
    os.system("mkdir -p build/objects")
    files = glob.glob(directory_path + '/**', recursive=True)
    # Loop through each file
    for file_path in files:
        if not check_file_extension(file_path, ['cpp']):
            continue
        # Get the file name with the full path
        print("Compiling:", file_path)
        os.system("mkdir -p ./build/objects/"+os.path.dirname(file_path))
        # Use the c++ compiler set the include as the include directory use compile mode and set the maximun optimization level and also set some warnings (also enable Asan) enable std c++ 23 also define COMPILE
        command: str = f"g++ -c -std=c++20 -ggdb -DCOMPILE -Iinclude -o ./build/objects/{file_path}.o {file_path}"
        if os.system(command) != 0:
            print(f"FAILED TO COMPILE DIRECTORY {directory_path}")
            return
    files = glob.glob("./build/objects/**", recursive=True)
    command: str = "g++ -std=c++20 -o ./build/main -ggdb "
    for file_path in files:
        if file_path[-2] != '.':
            continue
        print(f"Adding {file_path} to the commmand")
        command += file_path + " "
    # Add the remaining arguments
    command += "-Llib -lfmt -lm"
    print("Command: " + command)
    os.system(command)
    print("Removing objects")
    # os.system("rm -rf ./build/objects/")
    

if __name__ == '__main__':
    main("./src")