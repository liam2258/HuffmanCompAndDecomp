import subprocess
import os
import pytest

# Function to compile the program
def test_compilation():
    # Compile the C++ files
    compile_command = "g++ ../../main.cpp -o main"
    compile_process = subprocess.Popen(compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    
    # Wait for the compilation to finish
    compile_process.wait()

    # Check for compilation errors
    _, compile_error = compile_process.communicate()

    assert os.path.exists("main.exe"), f'Expected main.exe to complile but it does not exist {compile_error}'

def test_clean_up():
    os.remove("main.exe")
    assert not os.path.exists("main.exe"), f"File main.exe should not exist, but it does"