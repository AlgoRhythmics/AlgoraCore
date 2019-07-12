[![pipeline status](https://gitlab.com/AlgoRhythmics/AlgoraCore/badges/develop/pipeline.svg)](https://gitlab.com/AlgoRhythmics/AlgoraCore/commits/develop)

# Algora|Core

**Algora|Core** is the core library of the **Algora** project.

It provides a directed graph data structure realized as an adjacency list,
support for sub- and supergraphs,
property maps to attribute a graph's vertices and arcs,
as well as some basic graph algorithms and
routines to read and write graphs.

The design principle was to have data structures and algorithms that are
1. intuitive to work with,
2. extensible, and
3. as efficient as possible.

## Building

**Algora|Core** is written in C++17.
Implementations are based on the STL and additionally use `boost/dynamic_bitset` as well as
`boost/logic/tribool`.
The building process employs `qmake` version 5.

On Debian/Ubuntu, all dependencies can be installed by running: `# apt install
qt5-qmake libboost-dev`.
On Fedora, run `# dnf install qt5-qtbase-devel boost-devel`.

To facilitate the building process, **Algora|Core** comes with an
`easyCompile` script that creates the necessary build directories and
compiles the project.
All you need to do is running `$ ./easyCompile` from the project's topmost
directory.
The compiled library can then be found in the `build/Debug` and `build/Release`
subdirectories.

## License

**Algora** and **Algora|Core** is free software and licensed under the
GNU General Public License version 3.
See also the file `COPYING`.

## Author

**Algora|Core** was written by Kathrin Hanauer.
