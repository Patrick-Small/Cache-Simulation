# Cache-Simulation
Written for CS_441 Computer Systems Architecture during the Spring semester of 2024 at SUNY Polytechnic Institute.

This C++ Program simulates a Set Associative Cache using LRU Replacement. It takes in 4 inputs, the first being a trace file that contains memor addresses, and then 3 numbers that are specified below. This program can simulate a Direct Mapped cache if you set the number of blocks per set to 0, and can also simulate a Fully Associative cache if you set the number of direct-mapped sets to 0. 
 
Inputs go in this order:
  1. Memory trace input file
  2. X, where 2^X == # of direct-mapped sets
  3. Y, where 2^Y == # of blocks/set
  4. Z, where 2^Z == # of cached words/block
