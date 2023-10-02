import subprocess
import os

# Function to compile the program
def compile_cpp_files():
    # Compile the C++ files and place the executable in the specified location
    compile_command = "g++ ../../main.cpp -o main"
    compile_process = subprocess.Popen(compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    executable = "main"
    
    # Wait for the compilation to finish
    compile_process.wait()

    # Check for compilation errors
    _, compile_error = compile_process.communicate()
    if compile_process.returncode != 0:
        print(f'Compilation failed with the following error:\n{compile_error.decode("utf-8")}')
    else:
        print(f'Compilation successful. Executable placed in: {executable}')

if __name__ == "__main__":
    compile_cpp_files()

    # Clean up created files
    os.remove("main.exe")