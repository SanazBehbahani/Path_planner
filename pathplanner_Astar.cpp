#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

/* TODO: Define a Map class
   Inside the map class, define the mapWidth, mapHeight and grid as a 2D vector
*/
class Map:
{
    // Access specifier
    public:
    // Data members
    const static int mapWidth = 6;
    const static int mapHeight = 5;
    vector <vector<int>> grid = 
    {
    {0, 1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0}
    };

    vector <vector<int>> manhattan_heuristic = 
    {
    {9, 8, 7, 6, 5, 4},
    {8, 7, 6, 5, 4, 3},
    {7, 6, 5, 4, 3, 2},
    {6, 5, 4, 3, 2, 1},
    {5, 4, 3, 2, 1, 0}
    };
};

/* TODO: Define a Planner class
   Inside the Planner class, define the start, goal, cost, movements, and movements_arrows
   Note: The goal should be defined it terms of the mapWidth and mapHeight
*/
class Planner: Map
{
    // Access specifier
    public:
    // Data members
    int start [2] = {0, 0};
    int goal [2] = {4, 5};
    int cost = 1;
    vector <vector <int>> movements = 
    {
    {-1, 0},
    {0, -1},
    {1, 0},
    {0, 1}
    };
    string movement_arrows [4] = {"^", "<", "v", ">"};
};

/* TODO: Define a print2DVector function which will print 2D vectors of any data type
   Example
   
   Input: 
   vector<vector<int> > a{{ 1, 0 },{ 0, 1 }};
   print2DVector(a);
   vector<vector<string> > b{{ "a", "b" },{ "c", "d" }};
   print2DVector(b);
   
   Output:
   1 0
   0 1
   a b
   c d
   Hint: You need to use templates
*/
template <typename T>
void print2DVector (T Vec)
{
    for (int i = 0; i < Vec.size(); ++i){
        for (int j = 0; j < Vec[0].size(); ++j){
	    cout << Vec [i][j] << ' ';
        };
	cout << endl;
    };
};

// Search function which generates the expansions
void search (Map map, Planner planner)
{
    // Create a closed 2D array filled with 0s and 1 as its first element
    vector <vector <int>> closed (map.mapHeight, vector <int> (map.mapWidth));
    closed [planner.start[0]][planner.start[1]] = 1;

    // Create expand array filled with -1
    vector <vector <int>> expand (map.mapHeight, vector <int> (map.mapWidth, -1));

    // Create action array filled with -1
    vector <vector <int>> action (map.mapHeight, vector <int> (map.mapWidth, -1));    
    
    // Define the triplet
    int x = planner.start[0];
    int y = planner.start[1];
    int g = 0;
    int f = g + map.manhattan_heuristic [x][y];

    // Store the expansion
    vector <vector <int>> open;
    open.pushback ({g, x, y});
    
    // Flags
    bool found = false;
    bool resign = false;

    // Counter
    int count = 0;

    int x2;
    int y2;
    
    // While I am still searching for the goal and problem is solvable
    while (!found && !resign){
	// Resign if no value in the open list and you can't expand anymore
	if (open.size == 0){
	    resign = true;
	    cout << "Failed to reach goal :(" << endl;
	};
	// Keep expanding
	else {
	    // Remove triplets from open list
	    sort (open.begin(), open.end());
	    reverse (open.begin(), open.end());
	    vector <int> next; 
	    // Store poped value in next
	    next = open.back();
	    open.pop_back();

	    g = next [0];
	    x = next [1];
	    y = next [2];

	    // Fill the expand vectors with count
	    expand [x][y] = count;
	    count += 1;

	    // Check if we reached the goal
    	    if (x == planner.goal[0] && y == planner.goal[1]){
		found = true;
		cout << "[" << g << ", " << x << ", " << y << "]" << endl;
	    };

   	    // else expand new element
            else{
		for (int i = 0; i < planner.movements.size(); i++){
		    x2 = x + planner.movements [i][0];
		    y2 = y + planner.movements [i][1];
		    if (x2 >= 0 && x2 < map.grid.size() && y2 >= 0 && y2 < map.grid[0].size()){
			if (closed [x2][y2] == 0 && map.grid [x2][y2] == 0){
			    int g2 = g + planner.cost;
			    open.push_back ({g2, x2, y2});
			    closed [x2][y2] = 1; 
			    action [x2][y2] = i;
			};
		    };
		};

	    };
    	    
	};

    };
    // Print the expansion list 
    print2DVector (expand);

    // Find the path with robot orientation
    vector <vector <string>> policy (map.mapHeight, vector<string>(map.mapWidth, "-"));

    // Goint backward
    x = planner.goal[0];
    y = planner.goal[1];
    policy [x][y] = "*";

    while (x != planner.start[0] or y!= planner.start[1]){
	x2 = x - planner.movements [action[x][y]][0];
	y2 = y - planner.movements [action[x][y]][1];
	policy [x2][y2] = planner.movements_arrow[action[x][y]];
	x = x2;
	y = y2;
    }; 

    // Print the path with arrows 
    print2DVector (policy);
    
};
    

/*############ Don't modify the main function############*/
int main()
{
    // Instantiate map and planner objects
    Map map;
    Planner planner;

    // Print classes variables
    cout << "Map:" << endl;
    print2DVector(map.grid);
    cout << "Start: " << planner.start[0] << " , " << planner.start[1] << endl;
    cout << "Goal: " << planner.goal[0] << " , " << planner.goal[1] << endl;
    cout << "Cost: " << planner.cost << endl;
    cout << "Robot Movements: " << planner.movements_arrows[0] << " , " << planner.movements_arrows[1] << " , " << planner.movements_arrows[2] << " , " << planner.movements_arrows[3] << endl;
    cout << "Delta:" << endl;
    print2DVector(planner.movements);

    // Search for the expansions
    search (map, planner);

    return 0;
}

