#!/usr/bin/bash

# Creating bin directory if not exist
# Change current directory to bin one
create_bin_dir ()
{
	if [[ ! -e bin ]]; then
		echo "========================================"
		echo "Creating bin directory"
		mkdir bin
		cd bin
	else 
		cd bin
		rm *
 	fi
}

create_bin_dir
echo "========================================"
echo "Creating binary files..."
echo "========================================"

echo "Creating hello_mpi..."
mpic++ -o hello_mpi ../src/hello_mpi.cpp
echo "hello_mpi is created"

echo "Creating numb_mpi..."
mpic++ -o numb_mpi ../src/numb_mpi.cpp
echo "numb_mpi is created"

echo "Creating pi_mpi..."
mpic++ -o pi_mpi ../src/pi_mpi.cpp
echo "pi_mpi is created"

echo "========================================"
echo "Building process completed!"
echo "========================================"

