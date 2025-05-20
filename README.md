# ls

This project is a reimplementation of the Unix `ls` command, which lists directory contents.

## Overview

The goal of this project is to understand and recreate the core functionality of the `ls` utility, commonly found on Unix-like operating systems. The program displays information about files and directories, supporting various options to control the output format and details.

## Features

- List files and directories in the current or specified path
- Support for common flags such as:
  - `-l`: long listing format (detailed info)
  - `-a`: show hidden files
  - `-r`: reverse order
  - `-t`: sort by modification time
- Handles multiple directories and files as arguments

## Usage

```sh
./ls [options] [file...]
```

Examples:
- `./ls` — list files in the current directory
- `./ls -l` — detailed listing
- `./ls -a` — include hidden files
- `./ls -ltr` — combine options

## Learning Objectives

- Practice working with directory and file system APIs
- Handle command-line arguments and options
- Format and display output similar to the standard `ls` command

## Notes

This project is for educational purposes as part of the 42 school curriculum.

