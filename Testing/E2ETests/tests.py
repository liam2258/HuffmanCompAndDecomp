import subprocess
import os
import pytest
import platform

# Function to test file compilation
def test_compilation():
    
    # Get the directory of the current script
    script_directory = os.path.dirname(os.path.abspath(__file__))

    # Construct the absolute paths to the source files
    main_cpp_path = os.path.join(script_directory, "../../src/main.cpp")
    bitutils_cpp_path = os.path.join(script_directory, "../../src/BitUtils.cpp")
    treeutils_cpp_path = os.path.join(script_directory, "../../src/TreeUtils.cpp")
    maputils_cpp_path = os.path.join(script_directory, "../../src/MapUtils.cpp")
    computils_cpp_path = os.path.join(script_directory, "../../src/CompUtils.cpp")
    node_cpp_path = os.path.join(script_directory, "../../src/Node.cpp")

    # Specify the output path for the compiled executable in the script directory
    output_executable_path = os.path.join(script_directory, "main")

    # Create the compile command and run it
    compile_command = f"g++ {node_cpp_path} {main_cpp_path} {bitutils_cpp_path} {treeutils_cpp_path} {maputils_cpp_path} {computils_cpp_path} -o {output_executable_path}"
    compile_process = subprocess.Popen(compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Check results of compilation
    _, compile_error = compile_process.communicate()


    # Modify how file is checked based on Linux or Windows
    if platform.system() == "Linux":
        assert os.path.exists("main"), f'Expected main.exe to complile but it does not exist {compile_error}'
    elif platform.system() == "Windows":
        assert os.path.exists(output_executable_path + ".exe"), f'Expected main.exe to complile but it does not exist {compile_error}'

# Function to test file compression
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

# Function to test file decompression
def test_file_decompression():
    # Get absolute path
    path = os.path.dirname(os.path.abspath(__file__))

    # Create commands and run commands, collect results
    command = f"{path}/main alice_in_wonderland.hcmp"
    decompression_process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    _, decompression_error1 = decompression_process.communicate()

    command = f"{path}/main kjv.hcmp"
    decompression_process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    _, decompression_error2 = decompression_process.communicate()

    # Confirm files are created
    assert os.path.exists(path + "/alice_in_wonderland(unzp).txt"), f'Expected file to be created but it was not {decompression_error1}'
    assert os.path.exists(path + "/alice_in_wonderland(unzp).txt"), f'Expected file to be created but it was not {decompression_error2}'

    # Confirm unzipped file sizes match
    assert os.path.getsize(path + "/alice_in_wonderland(unzp).txt") == os.path.getsize(path + "/alice_in_wonderland.txt"), f'Expected file contents to match but it does not'
    assert os.path.getsize(path + "/kjv(unzp).txt") == os.path.getsize(path + "/kjv.txt"), f'Expected file contents to match but it does not'

# Function to remove files produced by testing
def test_clean_up():
    # Get absolute path
    path = os.path.dirname(os.path.abspath(__file__))

    # Modify how executable is removed based on system
    if platform.system() == "Linux":
        os.remove(path + "/main")
        assert not os.path.isfile(path + "/main"), f'Expected file not to exist but it does'
    elif platform.system() == "Windows":
        os.remove(path + "/main.exe")
        assert not os.path.isfile(path + "/main.exe"), f'Expected file not to exist but it does'

    # Remove created files
    os.remove(path + "/alice_in_wonderland.hcmp")
    os.remove(path + "/kjv.hcmp")
    os.remove(path + "/alice_in_wonderland(unzp).txt")
    os.remove(path + "/kjv(unzp).txt")

    # Verify files were removed
    assert not os.path.isfile(path + "/alice_in_wonderland.hcmp"), f'Expected file not to exist but it does'
    assert not os.path.isfile(path + "/kjv.hcmp"), f'Expected file not to exist but it does'
    assert not os.path.isfile(path + "/alice_in_wonderland(unzp).txt"), f'Expected file not to exist but it does'
    assert not os.path.isfile(path + "/kjv(unzp).txt"), f'Expected file not to exist but it does'