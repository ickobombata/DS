# DS
Data structures, course at FMI

Labyrinth solution finder
The labyrinth consists of:
  - Rooms
  - if there is a pass from one room to another, there is a door between them
  - For every door there is a key somewhere in some room
  - Start and end point in the labyrinth
  
 The solution is to find all the keys needed to get from the start to the end point,
 and to record the movements(directions) needed.
 The second part is to compress the directions
  ex. RRRRR => 5R , RDRDU => 2(RD)U
