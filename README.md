## libycres [![Code Checker](https://github.com/yellowcake-org/libycres/actions/workflows/codechecker.yml/badge.svg)](https://github.com/yellowcake-org/libycres/actions/workflows/codechecker.yml)

This repository is a part of [Yellowcake](https://github.com/yellowcake-org) project.

### What it does

The library is intended to provide instruments for working with original Fallout™ resources, such as `.map`, `.pro`, `.dat` files, and many others, which were used in classic Fallout™ games. This repository includes example tools, provided for each file format separately, via `examples`' targets. As for now, they provide basic functionality, printing out parsed information. For complex formats, like `.map`, not all the fields are well understood, and thus skipped during parsing. Example tools might not print all the parsed info at the time. Nonetheless, almost all fields are being parsed.

### Why this is useful

It is useful in obvious way for reproducing original game: providing modern, safe and stable codebase to work with original games' resources. It may serve as a documentation for those who want to learn about original file formats. And, finally, its' example tools could be a good starting point for modders of original games. In the beginning it can't do much, though, but in case of significant demand from the community, it might become something bigger, like full-featured map editor;  stay tuned.

### How to get started

The project uses CMake as build system. I do my best to keep `CMakeLists.txt` as simple as possible. The root one has library target and examples targets, which provide separate `CMakeLists.txt` files. Dependencies are manage through git submodules and including their's `CMakeLists.txt` as subdirectories. 

### Where to get help

[GitHub Issues](https://github.com/yellowcake-org/libycres/issues) is the right place. GitHub Discussion are disabled at the moment, but will be enabled in case of significant activity from community; Discord server might be run in addition, when number of people interested reaches reasonable milestone. As for now, I am handling all issues internally, using additional software.

### Who maintains and contributes

Currently, only [__@0xceed__](https://github.com/0xceed) is working on the library. While help from the community will be _very_ needed in the future, this early in development I need to focus on creating good example of my vision, guidelines and many, many more.