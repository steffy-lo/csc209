# CSC209
 Systems Programming & Software Tools
 
## Assignment 1: Shell Script
#### Part 1: The animal guessing game
Example Output:
$ sh animal db
Think of an animal.
Is your animal green?
y
Is your animal a frog?
n
Aw, I lose.  What animal were you thinking of?
tree
Tell me a question which would distinguish a frog from a tree.
Does it have leaves?
What would be the answer for a tree?
y
I'll remember that.
$ sh animal db
Think of an animal.
Is your animal green?
y
Does it have leaves?
n
Is your animal a frog?
y
I got it!
$ 

This part of the assignment writes the above program as an sh shell script.

The unix filesystem is used to store all of the data for the program. Each question is represented by a directory. Each directory contains either a file named name stating the animal at that leaf node (e.g. "frog") or a file named question containing a yes/no question to ask (including the question mark if applicable), and subdirectories yes and no which are further nodes to be consulted in the case of the respective answers.
As in the example above, the shell scripts will all take the starting directory name as $1 (output a usage message if $# is not 1). As animals are added, it modifies this directory.

#### Part 2: Euclid's greatest common divisor algorithm
The second part of this assignment involves writing an sh shell script that uses Euclid's algorithm to calculate the greatest common divisor of any number of non-negative integers, where inputs will be supplied either from files or from standard input. The algorithm to calculate the GCD of two non-negative integers x and y is:
        while (y != 0) {
            t = x
            x = y
            y = t % y
        }
        print x
Arguments to the shell script are file names; if there are no arguments, it will process the standard input. All of the contents of all of the concatenated files other than white space are expected to be non-negative integers and it will be able to accept numbers separated by spaces or by newlines, and blank lines, and so on.
The script outputs a single number which is the greatest common divisor of all of the input numbers.

## Assignment 2
#### 1. Derep
A standard unix filter, which processes its standard input if there are no arguments, or processes a sequence of one or more file names on the command-line. If one of these "file names" is actually the single-character string "−", stdin will be read at that point, and there are no command-line options.
The transformation on the file is to eliminate large repetitions of a single byte. If a single byte occurs more than ten times in a row, after the first ten occurrences, instead of the remaining occurrences output "[%d more %d bytes]", where the first %d is the number of remaining occurrences and the second %d is the byte in question.

#### 2. Parsing the PATH variable
A function in C that takes three parameters, as follows:
	int parsepath(char *path, char **result, int size)
The first parameter is the string to be parsed, in the format of the sh PATH variable (colon-separated directory names).
The second parameter is a pointer to the beginning of an array of pointers-to-char to hold the separately parsed-out directory path names. To avoid exceeding array bounds, the third parameter identifies the size of this array in the usual way.

parsepath() assigns the individual directory path names to successive members of the result array, in order. To do this in some cases it calls malloc() to create a new string array.
parsepath() returns the number of items put in the result array, or −1 for error (which is either a failed malloc() or exceeding the size of the result array).

#### 3. Funny times on a file
This tool finds files for which the mtime and ctime are not equal. It will search a particular directory and below, recursively, similar to find. The successful exit status is 0 whether such files are found or not.

The command-line arguments are one or more directories to search, preceded by two possible options: −s (for strictly-less-than) makes it report only files for which the mtime is less than the ctime, not files for which it is greater; and −n limit makes it exit with exit status 2 after reporting limit files if there are more files yet to report.

Command-line options are parsed with getopt() so as to accommodate all standard allowable command-line variations.
This tool doesn't use ftw() or fts().

## Assignment 3
#### Part 1: A trivial shell
This part of the assignment examines the basic operations needed to execute commands in a shell with execve().
This assignment does NOT use the 'p' family of exec() functions: execvp(), execlp(), or execvep() since these functions implement the PATH searching which is part of this assignment.

#### Part 2: A binary search tree without malloc(), via fork()
This second part of the assignment is essentially a binary search tree, where each connection between nodes will be represented by pipes, and each node is a separate process (the processes are arranged in a binary search tree, connected with pipes). Each connection needs bidirectional communication, in which a search request can be sent from parent to child and the result can be sent from child to parent; thus each link (graph edge) will correspond to two pipes (since a pipe is unidirectional).
The search keys are integers, and the values are arbitrary character strings of size up to 9 bytes (i.e. stored as a string in an array of size 10). The initial process is the root node. The key and value are specified on the command-line (the key is argv[1] and the value is argv[2]).

The initial process reads lines from stdin in a loop. The line can be simply an integer; this means to search for that key. Otherwise, the line can be an integer, a space, and the value; this means to set that key to that value. In either case the (possibly new) key and value are output.

Each process loops until EOF, getting a key or a key+value either from stdin (in the case of the root node process) or from the pipe from its parent (in the case of all other processes). Then, if the key matches its own key, it can return the key+value information up the pipe to its parent (or, if it's the root node, output it to stdout). Otherwise, it passes the request down to either its left child or right child as appropriate. When a process is first created, it doesn't have a left child or right child; when it goes to pass a request down to a child, it calls fork() if necessary (setting up the new child, which then does its own loop).

Upon EOF from the pipe from the parent, the pipes will close to any child node processes, and wait() for all child node processes, then exit. Child nodes, if any, will thus get EOF from their pipes to their parents, and do the same; and so on all the way down the tree. Similarly, when the root node process gets EOF from stdin, it will also do this (close pipes to children, wait, exit). This will cause an orderly shutdown of the entire tree when you signal EOF on stdin (e.g. by pressing control-D).

## Assignment 4
A program which relays messages, inspired by the children's "telephone game" in which children whisper a message to one another and we see how it gets distorted in the relaying. This program, however, will not distort the messages but rather, will keep track of the random path the messages take until they get back to their originator.

This program is called "yak.c" and can communicate with other instances of itself, which are referred to below as "yakkers".

Yak repeatedly does the following: It either reads a line from the standard input or receives a message from a connected "peer" (using select()). If this message originated with me, or has been circulating on too long a path (there is a "−c" option to set the maximum length of this path), it is displayed to the user. Otherwise, it is relayed to a random one of the connected peers.

Each message is prefaced by a list of IP address/port numbers which show the route the message has taken so far. This list is prepended to each time the message is relayed.

As messages are received (and possibly relayed), we may learn of other yakkers we didn't know about. Since the message contains their IP address and listening port number, we can connect to them to expand our network.

Furthermore, a blank line on stdin means to display a list of all current connections.

New peers may connect at any time (so your listening port number will also be included in the select()).

#### Some other details
Yak by default listens on port 3000, and optionally connects to another yakker upon startup. The '−p' option makes it listen on a different port number, and the optional hostname plus further optional port number on the command line specify a yakker to connect to upon startup in a syntax similar to that of nc.
