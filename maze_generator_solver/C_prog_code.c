#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "raylib.h"

// Directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// Structure to represent each cell
typedef struct {
    int visited;        //  to check if cell has been visited
    int partOfSolution; //  to check if cell is part of the final solution
    int walls[4];       // Array to check if walls exist in each direction
    int h;              // Heuristic value (Manhattan distance to goal)
    int g;              // Cost from start
    int parentX;        // X-coordinate of parent cell
    int parentY;        // Y-coordinate of parent cell
    int shortestPath;  // 1 if part of the shortest path
} Cell;

// Function declarations
void initializeMaze(Cell** maze, int width, int height);
void generateMaze(Cell** maze, int x, int y, int width, int height);
void saveMazeToFile(Cell** maze, int width, int height, const char* filename);
void solveMaze(Cell** maze, int width, int height);
void displayMazeWithRaylib(Cell** maze, int width, int height, int mode);
int isInBounds(int x, int y, int width, int height);
void removeWall(Cell** maze, int x1, int y1, int x2, int y2);

int main() {
    int width, height, choice;

    // random numbers for different mazes
    srand((unsigned int)time(NULL));

    // Get maze dimensions
    printf("Enter the width of the maze: ");
    while (scanf("%d", &width) != 1 || width <= 0) {
        printf("Invalid input. Please enter a positive integer for width: ");
        while (getchar() != '\n');  // Clear invalid input
    }

    printf("Enter the height of the maze: ");
    while (scanf("%d", &height) != 1 || height <= 0) {
        printf("Invalid input. Please enter a positive integer for height: ");
        while (getchar() != '\n');  // Clear invalid input
    }

    // Allocate memory for the maze
    Cell** maze = (Cell**)malloc(height * sizeof(Cell*));
    for (int i = 0; i < height; i++) {
        maze[i] = (Cell*)malloc(width * sizeof(Cell));
    }

    // Initialize and generate the maze
    initializeMaze(maze, width, height);
    generateMaze(maze, 0, 0, width, height);
    
    // Save the maze to the file
    saveMazeToFile(maze, width, height, "maze.txt");

    // Display the generated maze with raylib
    displayMazeWithRaylib(maze, width, height, 0);

    // Solve the maze
    solveMaze(maze, width, height);

    // Loop to display explored paths or shortest path
    do {
        printf("\nPress 1 to display all explored paths (Red) or 2 to display the shortest path (Green): ");
        while (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
            printf("Invalid input. Please press 1 or 2: ");
            while (getchar() != '\n');  // Clear invalid input
        }

        if (choice == 1) {
            displayMazeWithRaylib(maze, width, height, 1); // Display explored paths
        } else if (choice == 2) {
            displayMazeWithRaylib(maze, width, height, 2); // Display shortest path
        }

        printf("\nDo you want to display another view? (1 for Yes, 0 for No): ");
        while (scanf("%d", &choice) != 1 || (choice != 0 && choice != 1)) {
            printf("Invalid input. Please press 1 for Yes or 0 for No: ");
            while (getchar() != '\n');  // Clear invalid input
        }
    } while (choice == 1);

    // Free allocated memory
    for (int i = 0; i < height; i++) {
        free(maze[i]);
    }
    free(maze);

    return 0;   
}

// Function to initialize the maze
void initializeMaze(Cell** maze, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            maze[y][x].visited = 0;
            maze[y][x].partOfSolution = 0;
            maze[y][x].walls[UP] = 1;
            maze[y][x].walls[DOWN] = 1;
            maze[y][x].walls[LEFT] = 1;
            maze[y][x].walls[RIGHT] = 1;
            maze[y][x].h = (height - 1 - y) + (width - 1 - x); // Manhattan distance
            maze[y][x].g = INT_MAX; // Initially set to a high value
            maze[y][x].parentX = -1; // Undefined initially
            maze[y][x].parentY = -1;
        }
    }
}

// Utility function to check if a cell is within bounds
int isInBounds(int x, int y, int width, int height) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// Utility function to remove walls between two cells
void removeWall(Cell** maze, int x1, int y1, int x2, int y2) {
    if (x1 == x2) {
        if (y1 > y2) {
            maze[y1][x1].walls[UP] = 0;
            maze[y2][x2].walls[DOWN] = 0;
        } else {
            maze[y1][x1].walls[DOWN] = 0;
            maze[y2][x2].walls[UP] = 0;
        }
    } else if (y1 == y2) {
        if (x1 > x2) {
            maze[y1][x1].walls[LEFT] = 0;
            maze[y2][x2].walls[RIGHT] = 0;
        } else {
            maze[y1][x1].walls[RIGHT] = 0;
            maze[y2][x2].walls[LEFT] = 0;
        }
    }
}

// Recursive function to generate maze using DFS
void generateMaze(Cell** maze, int x, int y, int width, int height) {
    maze[y][x].visited = 1;

    int directions[] = { UP, DOWN, LEFT, RIGHT };
    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        int temp = directions[i];
        directions[i] = directions[j];
        directions[j] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x, ny = y;

        switch (directions[i]) {
        case UP:    ny = y - 1; break;
        case DOWN:  ny = y + 1; break;
        case LEFT:  nx = x - 1; break;
        case RIGHT: nx = x + 1; break;
        }

        if (isInBounds(nx, ny, width, height) && !maze[ny][nx].visited) {
            removeWall(maze, x, y, nx, ny);
            generateMaze(maze, nx, ny, width, height);
        }
    }
}
void saveMazeToFile(Cell** maze, int width, int height, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    fprintf(file, "Generated Maze:\n");

    // Loop through rows to write the maze structure
    for (int y = 0; y < height; y++) {
        // Write the top wall for each cell in the row
        for (int x = 0; x < width; x++) {
            fprintf(file, "+"); // Cell corner
            if (maze[y][x].walls[UP]) {
                fprintf(file, "---"); // Wall exists
            } else {
                fprintf(file, "   "); // No wall
            }
        }
        fprintf(file, "+\n"); // End of the row's top wall

        // Write the vertical walls and cell spaces
        for (int x = 0; x < width; x++) {
            if (maze[y][x].walls[LEFT]) {
                fprintf(file, "|"); // Wall exists
            } else {
                fprintf(file, " "); // No wall
            }
            fprintf(file, "   "); // Cell space
        }
        // Rightmost wall of the row
        if (maze[y][width - 1].walls[RIGHT]) {
            fprintf(file, "|");
        }
        fprintf(file, "\n");
    }

    // Write the bottom wall of the maze
    for (int x = 0; x < width; x++) {
        fprintf(file, "+---");
    }
    fprintf(file, "+\n");

    fclose(file);
    printf("Maze saved");
}

// Function to solve the maze using A* algorithm
// Function to solve the maze using A* algorithm
// Function to solve the maze using A* algorithm
void solveMaze(Cell** maze, int width, int height) {
    // Open list to track nodes to explore
    int (*openList)[2] = malloc(width * height * sizeof(int[2]));
    int* closedList = calloc(width * height, sizeof(int));

    int openListSize = 0;
    maze[0][0].g = 0;  // Starting cell cost
    openList[openListSize][0] = 0;  // Start position (x)
    openList[openListSize][1] = 0;  // Start position (y)
    openListSize++;

    // Initialize all cells
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            maze[y][x].g = INT_MAX;
            maze[y][x].shortestPath = 0;
            maze[y][x].partOfSolution = 0;  // Initialize to 0
        }
    }
    maze[0][0].g = 0;  // Starting point has zero cost

    // A* Algorithm loop
    while (openListSize > 0) {
        // Find cell with the lowest f = g + h (f = total estimated cost)
        int minIndex = 0;
        for (int i = 1; i < openListSize; i++) {
            int cx = openList[i][0], cy = openList[i][1];
            int fx = maze[cy][cx].g + maze[cy][cx].h;
            int fmin = maze[openList[minIndex][1]][openList[minIndex][0]].g +
                       maze[openList[minIndex][1]][openList[minIndex][0]].h;

            if (fx < fmin) {
                minIndex = i;
            }
        }

        // Pop current node
        int x = openList[minIndex][0], y = openList[minIndex][1];
        for (int i = minIndex; i < openListSize - 1; i++) {
            openList[i][0] = openList[i + 1][0];
            openList[i][1] = openList[i + 1][1];
        }
        openListSize--;

        // Mark as visited
        closedList[y * width + x] = 1;

        // Mark this cell as part of the solution (it is being explored)
        maze[y][x].partOfSolution = 1;

        // Goal reached: Backtrack and mark shortest path
        if (x == width - 1 && y == height - 1) {
            while (!(x == 0 && y == 0)) {
                maze[y][x].shortestPath = 1;  // Mark shortest path cell
                int px = maze[y][x].parentX, py = maze[y][x].parentY;
                x = px;
                y = py;
            }
            maze[0][0].shortestPath = 1;  // Start cell
            break;
        }

        // Explore neighbors
        for (int dir = 0; dir < 4; dir++) {
            int nx = x, ny = y;
            switch (dir) {
                case UP:    ny = y - 1; break;
                case DOWN:  ny = y + 1; break;
                case LEFT:  nx = x - 1; break;
                case RIGHT: nx = x + 1; break;
            }

            // Skip out-of-bounds cells or walls
            if (!isInBounds(nx, ny, width, height) || maze[y][x].walls[dir]) {
                continue;
            }

            // Skip cells already in the closed list
            if (closedList[ny * width + nx]) {
                continue;
            }
            
            // Calculate the new cost to reach the neighboring cell
            int newG = maze[y][x].g + 1;  // g = g + 1 (each move has cost 1)

            // If the new cost is better than the old cost, update and add it to the open list
            if (newG < maze[ny][nx].g) {
                maze[ny][nx].g = newG;
                maze[ny][nx].parentX = x;
                maze[ny][nx].parentY = y;

                // Add to open list if not already present
                int inOpenList = 0;
                for (int i = 0; i < openListSize; i++) {
                    if (openList[i][0] == nx && openList[i][1] == ny) {
                        inOpenList = 1;
                        break;
                    }
                }
                if (!inOpenList) {
                    openList[openListSize][0] = nx;
                    openList[openListSize][1] = ny;
                    openListSize++;
                }
            }
        }
    }

    free(openList);
    free(closedList);
}

// Function to display the maze with raylib
void displayMazeWithRaylib(Cell** maze, int width, int height, int mode) {
    const int cellSize = 20;
    InitWindow(width * cellSize, height * cellSize, "Maze Solver");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Draw the walls (checking each direction)
                if (maze[y][x].walls[UP]) {
                    DrawLine(x * cellSize, y * cellSize, (x + 1) * cellSize, y * cellSize, BLACK);  // Draw top wall
                }
                if (maze[y][x].walls[DOWN]) {
                    DrawLine(x * cellSize, (y + 1) * cellSize, (x + 1) * cellSize, (y + 1) * cellSize, BLACK);  // Draw bottom wall
                }
                if (maze[y][x].walls[LEFT]) {
                    DrawLine(x * cellSize, y * cellSize, x * cellSize, (y + 1) * cellSize, BLACK);  // Draw left wall
                }
                if (maze[y][x].walls[RIGHT]) {
                    DrawLine((x + 1) * cellSize, y * cellSize, (x + 1) * cellSize, (y + 1) * cellSize, BLACK);  // Draw right wall
                }

                // Draw the entire solution (All visited cells) in Light Gray
                if (mode == 1 && maze[y][x].partOfSolution) {
                    DrawRectangle(x * cellSize + 1, y * cellSize + 1, cellSize - 2, cellSize - 2, LIGHTGRAY);
                }

                // Draw the shortest path (Green)
                if (mode == 2 && maze[y][x].shortestPath) {
                    DrawRectangle(x * cellSize + 1, y * cellSize + 1, cellSize - 2, cellSize - 2, GREEN);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
}


