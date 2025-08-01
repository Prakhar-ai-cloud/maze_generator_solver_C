Maze Runners: Maze Generator and Solver 
Overview: 

This program generates a maze, solves it using the A algorithm*, and visualizes it using the 
Raylib library. It also saves the maze structure to a text file in a clear, human-readable format 
that shows the walls and paths. Users can toggle between viewing all explored paths or the 
shortest path to the goal, providing an engaging and interactive maze-solving experience. 
The program uses recursive backtracking to generate a random maze and calculates the 
solution based on Manhattan distance heuristics. The solution is displayed in a graphical 
interface, highlighting the shortest path in green. The saved maze file provides a detailed 
representation of the maze layout. 

Disclaimer: 
• Maze Size: Ensure the maze dimensions are reasonable to avoid performance issues 
or a cluttered graphical display. 
• Dependencies: The program requires the Raylib library. Ensure it is installed and 
configured correctly before running the program. 
• Compatibility: This code is designed for educational purposes and has been tested in 
a local environment using the Raylib editor. Adjustments might be needed for other 
platforms or configurations. 
•  
Features: 
1. Random Maze Generation: 
o Uses recursive backtracking to generate a maze with random pathways and a 
guaranteed solution. 
2. Interactive Visualization: 
o The maze and its solution are displayed using Raylib. Users can toggle 
between viewing: 
▪ Explored paths (all cells visited during solving). 
▪ Shortest path (optimal route to the goal). 
3. Maze Solving with A Algorithm*: 
o Implements the A* algorithm for efficient pathfinding. 
o Highlights the shortest path visually. 
4. File Saving: 
o Saves the generated maze to a text file (maze.txt), clearly showing walls and 
open pathways in a grid-like structure. 
5. Customizable Dimensions: 
o Users can specify the maze's width and height, allowing for small or large 
mazes. 
6. Error Handling: 
o Input validation ensures proper dimensions are entered. 
o File operations include error handling to handle potential issues. 
7. Educational and Fun: 
o Designed to showcase fundamental algorithms like recursive backtracking and 
A* in an interactive and visually appealing manner. 