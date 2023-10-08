import subprocess
import os
import pytest
import platform

# Function to compile the program
def test_compilation():
    
    # Get the directory of the current script
    script_directory = os.path.dirname(os.path.abspath(__file__))

    # Construct the absolute paths to the source files
    main_cpp_path = os.path.join(script_directory, "../../main.cpp")
    bitutils_cpp_path = os.path.join(script_directory, "../../BitUtils.cpp")

    # Specify the output path for the compiled executable in the script directory
    output_executable_path = os.path.join(script_directory, "main")

    # Compile command with absolute paths for source files and output
    compile_command = f"g++ {main_cpp_path} {bitutils_cpp_path} -o {output_executable_path}"
    compile_process = subprocess.Popen(compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Check for compilation errors
    _, compile_error = compile_process.communicate()

    if platform.system() == "Linux":
        assert os.path.exists("main"), f'Expected main.exe to complile but it does not exist {compile_error}'
    elif platform.system() == "Windows":
        assert os.path.exists(output_executable_path + ".exe"), f'Expected main.exe to complile but it does not exist {compile_error}'

def test_file_creation():
    path = os.path.dirname(os.path.abspath(__file__))
    command = f"{path}/main test.txt"
    compile_process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    compile_process.wait()
    _, compile_error1 = compile_process.communicate()

    command = f"{path}/main alice_in_wonderland.txt"
    compile_process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    compile_process.wait()
    _, compile_error2 = compile_process.communicate()

    command = f"{path}/main kjv.txt"
    compile_process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    compile_process.wait()
    _, compile_error3 = compile_process.communicate()

    # Wait for the compilation to finish
    compile_process.wait()
    if platform.system() == "Linux":
        assert os.path.exists("test.hcmp"), f'Expected file to be created but it was not {compile_error1}'
        assert os.path.exists("alice_in_wonderland.hcmp"), f'Expected file to be created but it was not {compile_error2}'
        assert os.path.exists("kjv.hcmp"), f'Expected file to be created but it was not {compile_error3}'
    elif platform.system() == "Windows":
        assert os.path.exists(path + "/test.hcmp"), f'Expected file to be created but it was not {compile_error1}'
        assert os.path.exists(path + "/alice_in_wonderland.hcmp"), f'Expected file to be created but it was not {compile_error2}'
        assert os.path.exists(path + "/kjv.hcmp"), f'Expected file to be created but it was not {compile_error3}'