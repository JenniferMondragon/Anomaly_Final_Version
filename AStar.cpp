// Aid Code from: https://www.geeksforgeeks.org/a-search-algorithm/ 
// My Code from: https://drive.google.com/file/d/1lAIwC6SxVn_buTL05D7auc7LPAYa17ii/view?usp=sharing

#include <iostream>
#include "stdc++.h"
using namespace std;

#define NumRow 9
#define NumCol 8

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int>> NewPair;

bool checkSpace(int r, int c)
{
	return (r >= 0) && (r < NumRow) && (c >= 0) && (c < NumCol);
}

bool checkMove(int grid[][NumCol], int r, int c)
{
	if (grid[r][c] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool checkDest(int r, int c, Pair dest)
{
	if (r = dest.first && c == dest.second)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double calcHeuristic(int r, int c, Pair dest)
{
	return ((double)sqrt((r - dest.first) * (r - dest.first) + (c - dest.second) * (c - dest.second))); 
}

struct cell
{
	int parent1, parent2;
	double x, y, z;
};

void followPath(cell cellInfo[][NumCol], Pair dest)
{
	int row = dest.first;
	int col = dest.second;

	stack<Pair> Path;

	while (!(cellInfo[row][col].parent1 == row
		&& cellInfo[row][col].parent2 == col)) {
		Path.push(make_pair(row, col));
		int temp_row = cellInfo[row][col].parent1;
		int temp_col = cellInfo[row][col].parent2;
		row = temp_row;
		col = temp_col;
	}

	Path.push(make_pair(row, col));
	while (!Path.empty()) {
		pair<int, int> p = Path.top();
		Path.pop();
		printf("-> (%d,%d) ", p.first, p.second);
	}

	return;
}

void aStarSearch(int grid[][NumCol], Pair src, Pair dest)
{
	// these are checks for each of the possible moves or destination blockage
	if ((checkSpace(src.first, src.second) == false))
	{
		cout << "Invalid Move." << endl;
		return;
	}

	if ((checkSpace(dest.first, dest.second) == false))
	{
		cout << "Invalid Destination." << endl;
		return;
	}

	if ((checkMove(grid, src.first, src.second) == false))
	{
		cout << "Invalid Move." << endl;
		return;
	}

	if ((checkSpace(src.first, src.second) == false) || (checkSpace(dest.first, dest.second)) == false)
	{
		cout << "Invalid Move." << endl;
		return;
	}

	bool closedList[NumRow][NumCol];
	memset(closedList, false, sizeof(closedList));

	cell cellInfo[NumRow][NumCol];

	int x, y;

	for (x = 0; x < NumRow; x++)
	{
		for (y = 0; y < NumCol; y++)
		{
			cellInfo[x][y].x = FLT_MAX;
			cellInfo[x][y].y = FLT_MAX;
			cellInfo[x][y].z = FLT_MAX;
			cellInfo[x][y].parent1 = -1;
			cellInfo[x][y].parent2 = -1;
		}
	}

	x = src.first, y = src.second;
	cellInfo[x][y].x = 0.0;
	cellInfo[x][y].y = 0.0;
	cellInfo[x][y].z = 0.0;
	cellInfo[x][y].parent1 = x;
	cellInfo[x][y].parent2 = y;

	set<NewPair> openList;

	openList.insert(make_pair(0.0, make_pair(x, y)));

	bool found = false;

	while (!openList.empty())
	{
		NewPair p = *openList.begin();

		openList.erase(openList.begin());

		x = p.second.first;
		y = p.second.second;
		closedList[x][y] = true;

		double xNew, yNew, zNew;

		//Checks North
		if (checkSpace(x - 1, y) == true)
		{
			if (checkDest(x, y + 1, dest) == true)
			{
				cellInfo[x - 1][y].parent1 = x;
				cellInfo[x - 1][y].parent2 = y;
				followPath(cellInfo, dest);
				found = true;
				return;
			}
			else if (closedList[x - 1][y] == false && checkMove(grid, x - 1, y) == true)
			{
				xNew = cellInfo[x][y].x + 1.0;
				yNew = calcHeuristic(x - 1, y, dest);
				zNew = xNew + yNew;

				if (cellInfo[x - 1][y].z == FLT_MAX || cellInfo[x - 1][y].z > zNew)
				{
					openList.insert(make_pair(
						zNew, make_pair(x - 1, y)));

					// Update the details of this cell
					cellInfo[x - 1][y].z = zNew;
					cellInfo[x - 1][y].y = yNew;
					cellInfo[x - 1][y].x = xNew;
					cellInfo[x - 1][y].parent1 = x;
					cellInfo[x - 1][y].parent2 = y;
				}
			}
		}

		//checks South
		if (checkSpace(x + 1, y) == true)
			{
				if (checkDest(x, y + 1, dest) == true)
				{
					cellInfo[x + 1][y].parent1 = x;
					cellInfo[x + 1][y].parent2 = y;
					followPath(cellInfo, dest);
					found = true;
					return;
				}

				else if (closedList[x + 1][y] == false && checkMove(grid, x + 1, y) == true)
				{
					xNew = cellInfo[x][y].x + 1.0;
					yNew = calcHeuristic(x + 1, y, dest);
					zNew = xNew + yNew;

					if (cellInfo[x + 1][y].z == FLT_MAX || cellInfo[x + 1][y].z > zNew)
					{
						openList.insert(make_pair(
							zNew, make_pair(x + 1, y)));

						// Update the details of this cell
						cellInfo[x + 1][y].z = zNew;
						cellInfo[x + 1][y].y = yNew;
						cellInfo[x + 1][y].x = xNew;
						cellInfo[x + 1][y].parent1 = x;
						cellInfo[x + 1][y].parent2 = y;
					}
				}
			}
		
		//check East
		if (checkSpace(x, y + 1) == true) {
					if (checkDest(x, y + 1, dest) == true)
					{
						cellInfo[x][y + 1].parent1 = x;
						cellInfo[x][y + 1].parent2 = y;
						followPath(cellInfo, dest);
						found = true;
						return;
					}

					else if (closedList[x][y + 1] == false && checkMove(grid, x, y + 1) == true)
					{
						xNew = cellInfo[x][y].x + 1.0;
						yNew = calcHeuristic(x, y + 1, dest);
						zNew = xNew + yNew;


						if (cellInfo[x][y + 1].z == FLT_MAX || cellInfo[x][y + 1].z > zNew)
						{
							openList.insert(make_pair(
								zNew, make_pair(x, y + 1)));

							// Update the details of this cell
							cellInfo[x][y + 1].z = zNew;
							cellInfo[x][y + 1].y = xNew;
							cellInfo[x][y + 1].x = yNew;
							cellInfo[x][y + 1].parent1 = x;
							cellInfo[x][y + 1].parent2 = y;
						}
					}
				}

		//check West
		if (checkSpace(x, y - 1) == true)
				{
					if (checkDest(x, y - 1, dest) == true) {
						// Set the Parent of the destination cell
						cellInfo[x][y - 1].parent1 = x;
						cellInfo[x][y - 1].parent2 = y;
						followPath(cellInfo, dest);
						found = true;
						return;
					}

					else if (closedList[x][y - 1] == false && checkMove(grid, x, y - 1) == true)
					{
						xNew = cellInfo[x][y].x + 1.0;
						yNew = calcHeuristic(x, y - 1, dest);
						zNew = xNew + yNew;

						if (cellInfo[x][y - 1].z == FLT_MAX
							|| cellInfo[x][y - 1].z > zNew) {
							openList.insert(make_pair(
								zNew, make_pair(x, y - 1)));

							// Update the details of this cell
							cellInfo[x][y - 1].z = zNew;
							cellInfo[x][y - 1].y = xNew;
							cellInfo[x][y - 1].x = yNew;
							cellInfo[x][y - 1].parent1 = x;
							cellInfo[x][y - 1].parent2 = y;
						}
					}
				}

		//check NorthEast
		if (checkSpace(x - 1, y + 1) == true) {
			if (checkDest(x - 1, y + 1, dest) == true) {
				cellInfo[x - 1][y + 1].parent1 = x;
				cellInfo[x - 1][y + 1].parent2 = y;
				followPath(cellInfo, dest);
				found = true;
				return;
			}

		
			else if (closedList[x - 1][y + 1] == false
				&& checkMove(grid, x - 1, y + 1)
				== true) {
				xNew = cellInfo[x][y].x + 1.414;
				yNew = calcHeuristic(x - 1, y + 1, dest);
				zNew = xNew + yNew;

				if (cellInfo[x - 1][y + 1].z == FLT_MAX
					|| cellInfo[x - 1][y + 1].z > zNew) {
					openList.insert(make_pair(
						zNew, make_pair(x - 1, y + 1)));

					// Update the details of this cell
					cellInfo[x - 1][y + 1].z = zNew;
					cellInfo[x - 1][y + 1].y = xNew;
					cellInfo[x - 1][y + 1].x = yNew;
					cellInfo[x - 1][y + 1].parent1 = x;
					cellInfo[x - 1][y + 1].parent2 = y;
				}
			}
		}

		//check NorthWest
		if (checkSpace(x - 1, y - 1) == true) {
			if (checkDest(x - 1, y - 1, dest) == true) {
				cellInfo[x - 1][y - 1].parent1 = x;
				cellInfo[x - 1][y - 1].parent2 = y;
				followPath(cellInfo, dest);
				found = true;
				return;
			}

			else if (closedList[x - 1][y - 1] == false
				&& checkMove(grid, x - 1, y - 1)
				== true) {
				xNew = cellInfo[x][y].x + 1.414;
				yNew = calcHeuristic(x - 1, y - 1, dest);
				zNew = xNew + yNew;

				if (cellInfo[x - 1][y - 1].z == FLT_MAX
					|| cellInfo[x - 1][y - 1].z > zNew) {
					openList.insert(make_pair(
						zNew, make_pair(x - 1, y - 1)));
					cellInfo[x - 1][y - 1].z = zNew;
					cellInfo[x - 1][y - 1].y = xNew;
					cellInfo[x - 1][y - 1].x = yNew;
					cellInfo[x - 1][y - 1].parent1 = x;
					cellInfo[x - 1][y - 1].parent2 = y;
				}
			}
		}

		//check SouthEast
		if (checkSpace(x + 1, y + 1) == true) {
			if (checkDest(x + 1, y + 1, dest) == true) {
				cellInfo[x + 1][y + 1].parent1 = x;
				cellInfo[x + 1][y + 1].parent2 = y;
				followPath(cellInfo, dest);
				found = true;
				return;
			}

			else if (closedList[x + 1][y + 1] == false
				&& checkMove(grid, x + 1, y + 1)
				== true) {
				xNew = cellInfo[x][y].x + 1.414;
				yNew = calcHeuristic(x + 1, y + 1, dest);
				zNew = xNew + yNew;

				if (cellInfo[x + 1][y + 1].z == FLT_MAX
					|| cellInfo[x + 1][y + 1].z > zNew) {
					openList.insert(make_pair(
						zNew, make_pair(x + 1, y + 1)));

					// Update the details of this cell
					cellInfo[x + 1][y + 1].z = zNew;
					cellInfo[x + 1][y + 1].y = xNew;
					cellInfo[x + 1][y + 1].x = yNew;
					cellInfo[x + 1][y + 1].parent1 = x;
					cellInfo[x + 1][y + 1].parent2 = y;
				}
			}
		}

		//check SouthWest
		if (checkSpace(x + 1, y - 1) == true) {
			if (checkDest(x + 1, y - 1, dest) == true) {
				cellInfo[x + 1][y - 1].parent1 = x;
				cellInfo[x + 1][y - 1].parent2 = y;
				followPath(cellInfo, dest);
				found = true;
				return;
			}

			else if (closedList[x + 1][y - 1] == false
				&& checkMove(grid, x + 1, y - 1)
				== true) {
				xNew = cellInfo[x][y].x + 1.414;
				yNew = calcHeuristic(x + 1, y - 1, dest);
				zNew = xNew + yNew;

				if (cellInfo[x + 1][y - 1].z == FLT_MAX
					|| cellInfo[x + 1][y - 1].z > zNew) {
					openList.insert(make_pair(
						zNew, make_pair(x + 1, y - 1)));

					// Update the details of this cell
					cellInfo[x + 1][y - 1].z = zNew;
					cellInfo[x + 1][y - 1].y = xNew;
					cellInfo[x + 1][y - 1].x = yNew;
					cellInfo[x + 1][y - 1].parent1 = x;
					cellInfo[x + 1][y - 1].parent2 = y;
				}
			}
		}
	}
}

int main()
{
	// used to showcase the code
	// Not used in game

	int grid[NumRow][NumCol] = { 
			{ 1, 1, 1, 1, 0, 1, 1, 1},
			{ 1, 1, 0, 1, 1, 1, 0, 1},
			{ 1, 1, 0, 1, 1, 0, 1, 0},
			{ 0, 1, 0, 1, 0, 0, 0, 0},
			{ 1, 1, 0, 1, 1, 1, 0, 1},
			{ 1, 1, 1, 1, 1, 0, 1, 0},
			{ 1, 0, 0, 0, 1, 0, 0, 0},
			{ 1, 1, 1, 1, 1, 0, 1, 1},
			{ 1, 1, 0, 0, 0, 1, 0, 0}};


	Pair src = make_pair(8, 0);

	Pair dest = make_pair(0, 0);

	aStarSearch(grid, src, dest); 

	return(0);
}

