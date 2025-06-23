[![✗](https://img.shields.io/badge/Release-v1.1.0-ffb600.svg?style=for-the-badge)](https://github.com/fardenghi/TP-TLA)

[![✗](https://github.com/fardenghi/TP-TLA/actions/workflows/pipeline.yaml/badge.svg?branch=production)](https://github.com/fardenghi/TP-TLA/actions/workflows/pipeline.yaml)

# Flex-Bison-Compiler

A base compiler example, developed with Flex and Bison.

- [Environment](#environment)
- [CI/CD](#cicd)
- [Recommended Extensions](#recommended-extensions)
- Installation
  - [Docker](doc/readme/Docker.md)
  - [Microsoft Windows](doc/readme/Windows.md)
  - [Ubuntu](doc/readme/Ubuntu.md)

## Environment

Set the following environment variables to control and configure the behaviour of the application:

| Name                  |    Default    | Description                                                                                                                                                           |
| --------------------- | :-----------: | --------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `LOG_IGNORED_LEXEMES` |    `true`     | When `true`, logs all of the ignored lexemes found with Flex at DEBUGGING level. To remove those logs from the console output set it to `false`.                      |
| `LOGGING_LEVEL`       | `INFORMATION` | The minimum level to log in the console output. From lower to higher, the available levels are: `ALL`, `DEBUGGING`, `INFORMATION`, `WARNING`, `ERROR` and `CRITICAL`. |

To run the generated Python programs, you will need Python 3 and several essential libraries installed on your system.

1. Install Python 3
You must have Python version 3.8 or newer.

On Windows:
Download the latest installer from the official Python website: python.org/downloads/
Run the installer. Important: On the first screen, check the box at the bottom that says "Add Python to PATH" before clicking "Install Now". This allows you to run Python from any command prompt.
On macOS:
The recommended method is to use Homebrew. If you have Homebrew installed, simply open the Terminal and run:
Bash

brew install python3
Alternatively, you can download the official macOS installer from the Python website.
On Linux:
For Debian, Ubuntu, or other derivatives, you can install Python 3 using the apt package manager in your terminal:

Bash

sudo apt update
sudo apt install python3
2. Install Required Libraries
With Python installed, you can now install the necessary libraries (pygame and numpy) using Python's package installer, pip.

Open your terminal or command prompt and run the following command:

Bash

pip install pygame numpy
Note: On some macOS and Linux systems, you may need to use pip3 instead to ensure the libraries are installed for the correct Python version:

Bash

pip3 install pygame numpy
3. Run the Program
Once the prerequisites are installed, you can run the simulation.

Open a terminal or command prompt.

Navigate to the directory containing the generated Python file (your_program_name.py).

Execute the script with the following command:

Bash

python3 your_program_name.py
(On Windows, you will likely just use python instead of python3).


## Recommended Extensions

- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
- [Yash](https://marketplace.visualstudio.com/items?itemName=daohong-emilio.yash)

## Running and Compiling a Program

To compile and run a program and redirect the Python output to a file, you can use the `start.sh` script located in the `/scripts/ubuntu` directory, as follows:

```bash
./start.sh "path/to/source/file" "path/to/output"
```
- `"path/to/source/file"`: path to the input source file.
- `"path/to/output.py"`: path where the Python output should be written.



