# sokoban-solver

A Sokoban puzzle solver using a BFS strategy.

## Building

To compile just use your regular compiler for C++

```
g++ -Wall -Wextra -pedantic-errors -o sokoban-bfs sokoban-bfs.cpp
```

## Executing

To execute just feed the input level via stdin with the following characteristics:

- `#` means a wall in the level
- `@` means the starting position of the player
- `$` means a crate for the player to push
- `.` means a possible end position for one of the crates

For example:

```
cat ./testCases/6.in | ./sokoban-bfs
```

Input:

```
#######
#. @  #
#$    #
#     #
####  #
#    $#
#.$  .#
#######
```

Output:

```
LDDLURRRRDDDLLDL
```
