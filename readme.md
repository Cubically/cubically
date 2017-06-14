# Rubik's Cube

Cubelang, also called the Pain Cube or just plain Rubik's Cube, is an extremely complex esoteric programming language.

This repository contains:

 - `rubiks.c` - cube operations
 - `lang.c` - the language
 - `visualizer.c` - a 2d cube visualizer/emulator

To build everything type `make`. This will generate two executables, `rubiks-lang` and `visualizer`. `rubiks-lang` either takes a file as a command-line argument or reads from standard input.
# Memory

Cubelang has an odd memory structure. The memory is a Rubik's cube. Various commands perform operations on this cube, such as turning the right face clockwise by 90 degrees. Other commands perform operations on certain faces of the cube, like adding all values on the top face together and storing them on the notepad (see below).

The cube is initialized with the numbers 0 through 5 on its faces, like this:

       000
       000
       000
    111222333444
    111222333444
    111222333444
       555
       555
       555

After performing a clockwise 90° turn on the right face, the memory cube would look like this:

       002
       002
       002
    111225333044
    111225333044
    111225333044
       554
       554
       554

There is one other piece of memory in Cubelang, the "notepad" (also called the scratchpad). This can store one value at a time. Performing various commands on the nonexistent sixth face will perform the command on memory.

# Syntax

A non-integer in the source code will set the command to that character. Each integer will execute the previous command, using that integer as an argument. There is no way to pass a value outside the range `0..9` to a command.

For example, `Y0` executes the command `Y` with argument `0`. `Y015` executes `Y` with `0`, then with `1`, then with `5`.

# Commands

Memory commands:

|command|description|arguments|
|-|-|-|
|R|turn right cube face clockwise 90°|face (value 0-5)|
|L|turn left cube face clockwise 90°|face|
|U|turn top cube face clockwise 90°|face|
|D|turn bottom cube face clockwise 90°|face|
|F|turn front cube face clockwise 90°|face|
|B|turn back cube face clockwise 90°|

General commands:

|command|description|arguments|
|-|-|-|
|E|exit|anything|
|@|add all values of specified face together, print as ASCII|face value (0-6), 6 = notepad|
|%|add all values of specified face together, print as decimal|^|
|+|add all values of specified face together, add to notepad, write result on notepad|^|
|-|add all values of specified face together, subtract from notepad, write result on notepad|^|
|*|add all values of specified face together, multiply by notepad, write result on notepad|^|
|/|add all values of specified face together, divide notepad by, write result on notepad|^|
|=|compare notepad and specified face for equality, write result on notepad|face value (0-5)|
|:|set notepad to specified face|^|

# Faces

|number|face|color|
|-|-|-|
|0|top|red|
|1|left|blue|
|2|front|purple|
|3|right|green|
|4|back|blue|
|5|down|yellow|
