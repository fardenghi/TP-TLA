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



