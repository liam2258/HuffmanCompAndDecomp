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

    # Create the compile command and run it
    compile_command = f"g++ {main_cpp_path} {bitutils_cpp_path} -o {output_executable_path}"
    compile_process = subprocess.Popen(compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Check results of compilation
    _, compile_error = compile_process.communicate()


    # Modify how file is checked based on Linux or Windows
    if platform.system() == "Linux":
        assert os.path.exists("main"), f'Expected main.exe to complile but it does not exist {compile_error}'
    elif platform.system() == "Windows":
        assert os.path.exists(output_executable_path + ".exe"), f'Expected main.exe to complile but it does not exist {compile_error}'

def test_file_compression():

    # Get absolute path
    path = os.path.dirname(os.path.abspath(__file__))

    # Create commands and run commands, collect results

    command = f"{path}/main alice_in_wonderland.txt"
    compression_process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    _, compression_error1 = compression_process.communicate()

    command = f"{path}/main kjv.txt"
    compression_process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    _, compression_error2 = compression_process.communicate()

    # Confirm files are created based on OS
    if platform.system() == "Linux":
        assert os.path.exists("alice_in_wonderland.hcmp"), f'Expected file to be created but it was not {compression_error1}'
        assert os.path.exists("kjv.hcmp"), f'Expected file to be created but it was not {compression_error2}'
    elif platform.system() == "Windows":
        assert os.path.exists(path + "/alice_in_wonderland.hcmp"), f'Expected file to be created but it was not {compression_error1}'
        assert os.path.exists(path + "/kjv.hcmp"), f'Expected file to be created but it was not {compression_error2}'

    # Check that compressed files are smaller than originals
    assert os.path.getsize(path + "/alice_in_wonderland.hcmp") < os.path.getsize(path + "/alice_in_wonderland.txt")
    assert os.path.getsize(path + "/kjv.hcmp") < os.path.getsize(path + "/kjv.txt")