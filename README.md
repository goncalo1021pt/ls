# ft_ls

This project is a reimplementation of the Unix `ls` command, which lists directory contents with memory-efficient architecture and comprehensive flag support.

## Overview

The goal of this project is to understand and recreate the core functionality of the `ls` utility, commonly found on Unix-like operating systems. The program displays information about files and directories, supporting various options to control the output format and details.

This implementation features a **memory-efficient architecture** that processes directories one-by-one and frees memory immediately after processing each subdirectory, making it capable of handling massive directory trees without memory issues.

## Architecture

- **Argument-by-argument processing**: Each command line argument is processed completely before moving to the next
- **Memory-efficient recursion**: For `-R` flag, subdirectories are processed and freed immediately
- **Clean separation of concerns**: Separate functions for loading, sorting, printing, and recursion
- **Error resilience**: Continues processing other arguments/directories even if some fail

## Features

### Supported Flags

| Flag | Description |
|------|-------------|
| `-l` | Long listing format (permissions, links, owner, group, size, date) |
| `-a` | Show all files, including hidden files (starting with `.`) |
| `-r` | Reverse the order of sorting |
| `-t` | Sort by modification time (newest first) |
| `-R` | Recursively list subdirectories |
| `-d` | List directories themselves, not their contents |
| `-g` | Like `-l` but don't show owner |
| `-f` | Don't sort; show files in directory order |

### File Type Colors

The program displays different file types in different colors:
- **Blue**: Directories
- **Cyan**: Symbolic links
- **Green**: Executable files
- **Magenta**: Socket files
- **Yellow**: FIFO/pipe files
- **Yellow on blue**: Block devices
- **Yellow on blue**: Character devices
- **Red on black**: Orphaned symbolic links

### Additional Features

- **Multiple arguments**: Can list multiple files and directories
- **Error handling**: Proper error messages with `strerror(errno)`
- **Memory safety**: No memory leaks, efficient cleanup
- **Standards compliance**: Output matches standard `ls` behavior
- **Scalability**: Can handle massive directory trees (tested on directories with millions of files)

## Usage

```sh
./ft_ls [options] [file...]
```

### Examples

#### Basic Usage
```sh
./ft_ls                    # List current directory
./ft_ls /home /etc         # List multiple directories
./ft_ls file.txt           # Show info about specific file
```

#### Flag Combinations
```sh
./ft_ls -l                 # Detailed listing
./ft_ls -la                # Detailed listing including hidden files
./ft_ls -ltr               # Detailed, time-sorted, reverse order
./ft_ls -R                 # Recursive listing of all subdirectories
./ft_ls -d */              # List only directories (not their contents)
```

#### Advanced Usage
```sh
./ft_ls -laRt /usr         # Show all files, detailed, recursive, time-sorted
./ft_ls -d . .. /tmp       # Show directory info without listing contents
./ft_ls -f                 # Raw directory order (no sorting)
```

## Technical Implementation

### Memory Management
- **Constant memory usage**: Only current directory + recursion stack depth held in memory
- **Immediate cleanup**: Each subdirectory freed right after processing
- **No memory leaks**: All allocated memory properly freed

### Error Handling
- **Graceful degradation**: Continues processing if individual files/directories fail
- **Proper error codes**: Returns appropriate exit codes (0=success, 1=minor errors, 2=serious errors)
- **Informative messages**: Uses `strerror(errno)` for accurate error descriptions

### Sorting Algorithms
- **Adaptive sorting**: Uses insertion sort for small arrays, merge sort for larger ones
- **Multiple criteria**: Supports alphabetical, time-based, and reverse sorting
- **Case handling**: Properly handles hidden files in sorting order

## Building

```sh
make                       # Build the project
make clean                 # Remove object files
make fclean               # Remove object files and executable
make re                   # Clean rebuild
```

## File Structure

```
.
├── Makefile
├── README.md
├── includes/
│   ├── headers/
│   │   └── ft_ls.h       # Main header file
│   └── libft/            # Custom library functions
└── srcs/
    ├── exec.c            # Main execution logic
    ├── main.c            # Entry point and argument parsing
    ├── parsing.c         # Option parsing
    ├── print.c           # Output formatting and display
    ├── sort.c            # Sorting algorithms
    └── tree.c            # File tree data structure
```

## Learning Objectives

- **System Programming**: Working with directory and file system APIs (`opendir`, `readdir`, `lstat`)
- **Memory Management**: Efficient allocation, deallocation, and leak prevention
- **Algorithm Implementation**: Sorting algorithms and data structure management
- **Command-line Processing**: Argument parsing and option handling
- **Error Handling**: Robust error checking and user-friendly error messages
- **Performance Optimization**: Memory-efficient algorithms for large-scale directory traversal

## Testing

The implementation has been tested with:
- Large directory trees (millions of files)
- Various file types (regular files, directories, symlinks, devices)
- Permission edge cases
- Invalid arguments and error conditions
- Memory leak detection tools

## Standards Compliance

This implementation closely follows the behavior of the standard Unix `ls` command, including:
- Output formatting
- Error message format
- Exit code conventions
- Flag behavior and interactions

## Notes

This project is part of the 42 school curriculum, focusing on system programming, memory management, and algorithm implementation. The emphasis is on creating a robust, efficient, and maintainable codebase that can handle real-world usage scenarios.

## Author

Created as part of 42 School specialization curriculum.