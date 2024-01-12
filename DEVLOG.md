# Dev Log:

## DAVID JOBIN

### 2024-01-04 - Conceived plan and populated proposal.md
N/A

### 2024-01-05
Set up files

### (Weekend of) 2024-01-06 - Initializations
Fragmented the parts into separate files,
linked with the makefile, and created basic
initialization of structures.

### 2024-01-08 - Reorganized player initialization
Moved functions out of main, now debugged and
in player, used to initialize the starting nation.
Added difficulty selection.

### 2024-01-09 - Introduced rival nations and structs
After the player is initialized, a variable list of
struct country pointers is created. Many currently
unused values, like GDP and military rating, have been
set up and are ready for implementation. The structure
of future code is roadmapped in main.c with many comments.

### 2024-01-10 - Rethought project and outlined server-client mechanics.
Scrapped forking ideas in favor of server-client functionality, so that
many players can join. Thought out player-player, robot-robot, and player-robot
interactions. "robot" would just be the server file.

### 2024-01-11 - Established server-client infrastructure.
Also updated the proposal match scope changes.