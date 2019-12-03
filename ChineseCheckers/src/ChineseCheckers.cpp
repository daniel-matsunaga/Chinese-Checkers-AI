//============================================================================
// Name        : homework2.cpp
// Author      : Daniel Matsunaga
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <ratio>
using namespace std;

class pieces
{
   char value;
   int j;
   int i;
   int goalDistance;
   int equatorDistance;

public:
   pieces(int _value, int _j, int _i, int _goalDistance, int _equatorDistance)
   {
      value = _value;
      j = _j;
      i = _i;
      goalDistance = _goalDistance;
      equatorDistance = _equatorDistance;
   }

   char getValue() const { return value; }
   int getJ() const { return j; }
   int getI() const { return i; }
   int getGoal() const {return goalDistance; }
   int getEquator() const {return equatorDistance; }
   };

class goal
{
   char value;
   int j;
   int i;


public:
   goal(int _value, int _j, int _i)
   {
      value = _value;
      j = _j;
      i = _i;
   }

   char getValue() const { return value; }
   int getJ() const { return j; }
   int getI() const { return i; }
};

class moves
{
	   int parent;
	   int y;
	   int x;
	   int advance;
	   char type;
	   int node;
	   int heuristic;

	public:
	   moves(int _parent, int _y, int _x, int _advance, char _type, int _node, int  _heuristic)
	   {
	      parent = _parent;
	      y = _y;
	      x = _x;
	      advance = _advance;
	      type = _type;
	      node = _node;
	      heuristic = _heuristic;
	   }

	   int getParent() const { return parent; }
	   int getJ() const { return y; }
	   int getI() const { return x; }
	   int getAdvance() { return advance; }
	   char getType() const { return type; }
	   int getNode() const { return node; }
	   int getHeuristic() const { return heuristic; }
	   void setHeuristic(int w) { heuristic = w; }
	   void setType(char t) { type = t; }

};


//GLOBAL VARIABLES
//============================================================================
char currentBoard[16][16];
vector<pieces> myPieces;
vector<pieces> oppPieces;
vector<goal> goals;
vector<goal> startingArea;
vector<vector<moves>> pMoves; //All possible moves, separated by layers
vector<moves> solution;
string gameType, color;
float playTime;
char myColorPiece, oppColorPiece, inputChar;
int goalIt, layer, goalLayer, turn;
int myCount, oppCount;
//============================================================================

moves compareMin(moves m1, moves m2)
{
    if(m1.getHeuristic() > m2.getHeuristic())
    {
    	return m2;
    }
    else if(m2.getHeuristic() > m1.getHeuristic())
    {
    	return m1;
    }
    else
    {
		if(myColorPiece == 'W')
		{
			if(m1.getJ() > m2.getJ())
			{
				return m1;
			}
			else
			{
				return m2;
			}
		}
		else if(myColorPiece == 'B')
		{
			if(m1.getJ() < m2.getJ())
			{
				return m2;
			}
			else
			{
				return m1;
			}
		}
    	return m1;
    }
}

moves compareMax(moves m1, moves m2)
{
	if(m1.getHeuristic() > m2.getHeuristic())
	{
		return m1;
	}
	else if(m2.getHeuristic() > m1.getHeuristic())
	{
		return m2;
	}
	else
	{
		if(myColorPiece == 'W')
		{
			if(m1.getJ() < m2.getJ())
			{
				return m1;
			}
			else
			{
				return m2;
			}
		}
		else if(myColorPiece == 'B')
		{
			if(m1.getJ() < m2.getJ())
			{
				return m2;
			}
			else
			{
				return m1;
			}
		}
		return m1;
	}
}

int Max(int alpha, moves v)
{
	if(alpha > v.getHeuristic())
	{
		return alpha;
	}
	else if(v.getHeuristic() > alpha)
	{
		return v.getHeuristic();
	}
	else
	{
		return alpha;
	}
}

int Min(int beta, moves v)
{
	if(beta > v.getHeuristic())
	{
		return v.getHeuristic();
	}
	else if(v.getHeuristic() > beta)
	{
		return beta;
	}
	else
	{
		return beta;
	}
}

void startingMoves()
{
	if(myColorPiece == 'W')
	{
		ofstream output("output.txt");
		switch(turn)
		{
			case 0:
				output << "E 11,15 10,14";
				output.close();
				break;
			case 1:
				output << "E 15,11 14,10";
				output.close();
				break;
			case 2:
				output << "J 12,15 10,13";
				output.close();
				break;
			case 3:
				output << "J 15,12 13,10";
				output.close();
				break;
			case 4:
				output << "E 12,13 11,12";
				output.close();
				break;
			case 5:
				output << "E 13,12 12,11";
				output.close();
				break;
			case 6:
				output << "J 13,15 11,13\nJ 11,13 9,13";
				output.close();
				break;
			case 7:
				output << "J 15,13 13,11\nJ 13,11 13,9";
				output.close();
				break;
			case 8:
				output << "J 14,15 12,13\nJ 12,13 10,11";
				output.close();
				break;
			case 9:
				output << "J 15,14 13,12\nJ 13,12 11,10";
				output.close();
				break;
			case 10:
				output << "E 13,13 12,12";
				output.close();
				break;
			case 11:
				output << "J 15,15 13,13\nJ 13,13 11,11";
				output.close();
				break;
			case 12:
				output << "J 11,14 9,12";
				output.close();
				break;
			case 13:
				output << "J 14,11 12,9";
				output.close();
				break;
			case 14:
				output << "E 12,14 11,13";
				output.close();
				break;
			case 15:
				output << "E 14,12 13,11";
				output.close();
				break;
			case 16:
				output << "J 14,14 14,12\nJ 14,12 12,10\nJ 12,10 10,10";
				output.close();
				break;
			case 17:
				output << "E 13,14 12,14";
				output.close();
				break;
			case 18:
				output << "J 12,14 10,12\nJ 10,12 8,12";
				output.close();
				break;
			case 19:
				output << "E 14,13 14,12";
				output.close();
				break;
			case 20:
				output << "J 14,12 12,10\nJ 12,10 12,8";
				output.close();
				break;
			default:
				output.close();
				break;
		}
	}
	else if(myColorPiece == 'B')
	{
		ofstream output("output.txt");
		switch(turn)
		{
			case 0:
				output << "E 4,0 5,1";
				output.close();
				break;
			case 1:
				output << "E 0,4 1,5";
				output.close();
				break;
			case 2:
				output << "J 3,0 5,2";
				output.close();
				break;
			case 3:
				output << "J 0,3 2,5";
				output.close();
				break;
			case 4:
				output << "E 3,2 4,3";
				output.close();
				break;
			case 5:
				output << "E 2,3 3,4";
				output.close();
				break;
			case 6:
				output << "J 2,0 4,2\nJ 4,2 6,2";
				output.close();
				break;
			case 7:
				output << "J 0,2 2,4\nJ 2,4 2,6";
				output.close();
				break;
			case 8:
				output << "J 1,0 3,2\nJ 3,2 5,4";
				output.close();
				break;
			case 9:
				output << "J 0,1 2,3\nJ 2,3 4,5";
				output.close();
				break;
			case 10:
				output << "E 2,2 3,3";
				output.close();
				break;
			case 11:
				output << "J 0,0 2,2\nJ 2,2 4,4";
				output.close();
				break;
			case 12:
				output << "J 4,1 6,3";
				output.close();
				break;
			case 13:
				output << "J 1,4 3,6";
				output.close();
				break;
			case 14:
				output << "E 3,1 4,2";
				output.close();
				break;
			case 15:
				output << "E 1,3 2,4";
				output.close();
				break;
			case 16:
				output << "J 1,1 1,3\nJ 1,3 3,5\nJ 3,5 5,5";
				output.close();
				break;
			case 17:
				output << "E 2,1 3,1";
				output.close();
				break;
			case 18:
				output << "J 3,1 5,3\nJ 5,3 7,3";
				output.close();
				break;
			case 19:
				output << "E 1,2 1,3";
				output.close();
				break;
			case 20:
				output << "J 1,3 3,5\nJ 3,5 3,7";
				output.close();
				break;
			default:
				output.close();
				break;
		}
	}

	return;
}

void createOutput(moves currentMove)
{
	moves m = currentMove;
	moves n = currentMove;
	int lay = goalLayer;
	int node = 0;

	while(lay > 1)
	{
		node = m.getNode();
		lay = lay - 1;
		m = pMoves[lay][node];
	}

	ofstream output("output.txt");

	solution.push_back(m);
	while(m.getParent() != - 1)
	{
		n = m;
		m = pMoves[lay][m.getParent()];
		if(m.getParent() == -1)
		{
			m.setType(n.getType());
		}
		solution.push_back(m);
	}

	while(solution.size() != 1)
	{
		m = solution.back();
		output << m.getType() << " " << m.getI() << "," << m.getJ() << " ";
		solution.pop_back();
		m = solution.back();
		output << m.getI() << "," << m.getJ();
		if(solution.size() != 1)
		{
			output << "\n";
		}
	}
	output.close();

	return;
}

int calculateGoalDistance(int j, int i, char colorPiece)
{
	int distance = 0;
	int m = 0;
	int n = 0;
	if(color == "WHITE")
	{
		if(colorPiece == 'W')
		{
			if(i > j) //Above Equator
			{
				distance = abs((goals[goalIt].getJ() - j) + (goals[goalIt].getI() - (i + (goals[goalIt].getJ() - j))));
			}
			else if(i < j) //Below Equator
			{
				distance = abs((goals[goalIt].getI() - i) + (goals[goalIt].getJ() - (j + (goals[goalIt].getI() - i))));
			}
			else if(i == j)
			{
				m = abs(goals[goalIt].getJ() - j);
				n = abs(goals[goalIt].getI() - i);
				if(m > n)
				{
					distance = m;
				}
				else
				{
					distance = n;
				}
			}
		}
		else if(colorPiece == 'B')
		{
			if(i > j) //Above Equator
			{
				distance = (15 - i) + (15 - (j + (15 - i)));
			}
			else if(i < j) //Below Equator
			{
				distance = (15 - j) + (15 - (i + (15 - j)));
			}
			else if(i == j)
			{
				distance = 15 - j;
			}
		}
	}
	else if(color == "BLACK")
	{
		if(colorPiece == 'W')
		{
			if(i > j) //Above Equator
			{
				distance = abs((0 - j) + (0 - (i + (0 - j))));
			}
			else if(i < j) //Below Equator
			{
				distance = abs((0 - i) + (0 - (j + (0 - i))));
			}
			else if(i == j)
			{
				distance = abs(0 - j);
			}
		}
		else if(colorPiece == 'B')
		{
			if(i > j) //Above Equator
			{
				distance = abs((goals[goalIt].getI() - i) + (goals[goalIt].getJ() - (j + (goals[goalIt].getI() - i))));
			}
			else if(i < j) //Below Equator
			{
				distance = abs((goals[goalIt].getJ() - j) + (goals[goalIt].getI() - (i + (goals[goalIt].getJ() - j))));
			}
			else if(i == j)
			{
				m = abs(goals[goalIt].getJ() - j);
				n = abs(goals[goalIt].getI() - i);
				if(m > n)
				{
					distance = m;
				}
				else
				{
					distance = n;
				}
			}

		}
	}
	return distance;
}

int calculateEquator(int y, int x)
{
	int equator = 0;
	if(x > y) //Above Equator
	{
		while(x > y)
		{
			x--;
			y++;
			equator++;
		}
	}
	else if(x < y) //Below Equator
	{
		while(x < y)
		{
			x++;
			y--;
			equator++;
		}
	}
	else if(x == y)
	{
		equator = 0;
	}

	return equator;
}

int findGoal()
{
	for(int i = 0; i < 19; i++)
	{
		if(goals[i].getValue() != myColorPiece)
		{
			return i;
		}
	}

	return 0;
}

void readInput()
{

	ifstream inputFile ("input.txt");
	if (inputFile.is_open())
	{
		//Assign values from the input.txt to variables
		getline(inputFile, gameType);
		while(gameType[gameType.length() - 1] == '\n' || gameType[gameType.length() - 1] == '\r')
		{
			gameType.erase(gameType.length() - 1);
		}

		getline(inputFile, color);
		while(color[color.length() - 1] == '\n' || color[color.length() - 1] == '\r')
		{
			color.erase(color.length() - 1);
		}

		string line;
		getline(inputFile, line);
		while(line[line.length() - 1] == '\n' || line[line.length() - 1] == '\r')
		{
			line.erase(line.length() - 1);
		}
		playTime = stof(line);

		for(int j = 0; j < 16; j++)
		{
			getline(inputFile, line);
			while(line[line.length() - 1] == '\n' || line[line.length() - 1] == '\r')
			{
				line.erase(line.length() - 1);
			}
			for(unsigned int i = 0; i < line.length(); i++)
			{
					currentBoard[j][i] = line[i];
			}
		}
		inputFile.close();
	}

	ifstream playInput("playdata.txt");
	if(playInput.is_open())
	{
		playInput >> turn;
		playInput.close();
	}
	else
	{
		turn = 0;
	}

	float depth1 = 0;
	float depth2 = 0;
	float depth3 = 0;
	ifstream calFile("calibration.txt");
	if(calFile.is_open())
	{
		calFile >> depth1 >> depth2 >> depth3;
	}
	calFile.close();

	if((playTime*1000.0 / depth3) >= 300)
	{
		goalLayer = 3;
	}
	else
	{
		goalLayer = 1;
	}

	//If can't read calibration file
	if(depth1 == 0 && depth2 == 0 && depth3 == 0)
	{
		if(turn >= 21)
		{
			goalLayer = 3;
		}
		else if(turn >= 40)
		{
			goalLayer = 2;
		}
		else if(turn >= 50)
		{
			goalLayer = 1;
		}
	}

	cout << goalLayer;
	layer = 0;
	pMoves.resize(5);

	if(gameType == "SINGLE")
	{
		//SINGLE GAME
		goalIt = 0;
		goalLayer = 1;
		//FULL GAME
		//Initialize myPieces and Goals
		if(color == "WHITE")
		{
			myColorPiece = 'W';
			oppColorPiece = 'B';
			//Y,X
			goals = { goal(currentBoard[0][0], 0, 0), goal(currentBoard[0][1], 0, 1), goal(currentBoard[1][0], 1, 0), goal(currentBoard[2][0], 2, 0), goal(currentBoard[1][1], 1, 1),
					  goal(currentBoard[0][2], 0, 2), goal(currentBoard[0][3], 0, 3), goal(currentBoard[1][2], 1, 2), goal(currentBoard[2][1], 2, 1), goal(currentBoard[3][0], 3, 0),
					  goal(currentBoard[4][0], 4, 0), goal(currentBoard[3][1], 3, 1), goal(currentBoard[2][2], 2, 2), goal(currentBoard[1][3], 1, 3), goal(currentBoard[0][4], 0, 4),
					  goal(currentBoard[1][4], 1, 4), goal(currentBoard[2][3], 2, 3), goal(currentBoard[3][2], 3, 2), goal(currentBoard[4][1], 4, 1) };

			startingArea = { goal(currentBoard[15][15], 15, 15), goal(currentBoard[15][14], 15, 14), goal(currentBoard[14][15], 14, 15), goal(currentBoard[13][15], 13, 15), goal(currentBoard[14][14], 14, 14),
							 goal(currentBoard[15][13], 15, 13), goal(currentBoard[15][12], 15, 12), goal(currentBoard[14][13], 14, 13), goal(currentBoard[13][14], 13, 14), goal(currentBoard[12][15], 12, 15),
							 goal(currentBoard[11][15], 11, 15), goal(currentBoard[12][14], 12, 14), goal(currentBoard[13][13], 13, 13), goal(currentBoard[14][12], 14, 12), goal(currentBoard[15][11], 15, 11),
							 goal(currentBoard[14][11], 14, 11), goal(currentBoard[13][12], 13, 12), goal(currentBoard[12][13], 12, 13), goal(currentBoard[11][14], 11, 14) };
		}
		else if(color == "BLACK")
		{
			myColorPiece = 'B';
			oppColorPiece = 'W';
			goals = { goal(currentBoard[15][15], 15, 15), goal(currentBoard[15][14], 15, 14), goal(currentBoard[14][15], 14, 15), goal(currentBoard[13][15], 13, 15), goal(currentBoard[14][14], 14, 14),
					  goal(currentBoard[15][13], 15, 13), goal(currentBoard[15][12], 15, 12), goal(currentBoard[14][13], 14, 13), goal(currentBoard[13][14], 13, 14), goal(currentBoard[12][15], 12, 15),
					  goal(currentBoard[11][15], 11, 15), goal(currentBoard[12][14], 12, 14), goal(currentBoard[13][13], 13, 13), goal(currentBoard[14][12], 14, 12), goal(currentBoard[15][11], 15, 11),
					  goal(currentBoard[14][11], 14, 11), goal(currentBoard[13][12], 13, 12), goal(currentBoard[12][13], 12, 13), goal(currentBoard[11][14], 11, 14) };

			startingArea = { goal(currentBoard[0][0], 0, 0), goal(currentBoard[0][1], 0, 1), goal(currentBoard[1][0], 1, 0), goal(currentBoard[2][0], 2, 0), goal(currentBoard[1][1], 1, 1),
							 goal(currentBoard[0][2], 0, 2), goal(currentBoard[0][3], 0, 3), goal(currentBoard[1][2], 1, 2), goal(currentBoard[2][1], 2, 1), goal(currentBoard[3][0], 3, 0),
							 goal(currentBoard[4][0], 4, 0), goal(currentBoard[3][1], 3, 1), goal(currentBoard[2][2], 2, 2), goal(currentBoard[1][3], 1, 3), goal(currentBoard[0][4], 0, 4),
							 goal(currentBoard[1][4], 1, 4), goal(currentBoard[2][3], 2, 3), goal(currentBoard[3][2], 3, 2), goal(currentBoard[4][1], 4, 1)};
		}

	}
	else if(gameType == "GAME")
	{
		//Read Playdata.txt
		goalIt = 0;
		//FULL GAME
		//Initialize myPieces and Goals
		if(color == "WHITE")
		{
			myColorPiece = 'W';
			oppColorPiece = 'B';
			goals = { goal(currentBoard[0][0], 0, 0), goal(currentBoard[0][1], 0, 1), goal(currentBoard[1][0], 1, 0), goal(currentBoard[2][0], 2, 0), goal(currentBoard[1][1], 1, 1),
					  goal(currentBoard[0][2], 0, 2), goal(currentBoard[0][3], 0, 3), goal(currentBoard[1][2], 1, 2), goal(currentBoard[2][1], 2, 1), goal(currentBoard[3][0], 3, 0),
					  goal(currentBoard[4][0], 4, 0), goal(currentBoard[3][1], 3, 1), goal(currentBoard[2][2], 2, 2), goal(currentBoard[1][3], 1, 3), goal(currentBoard[0][4], 0, 4),
					  goal(currentBoard[1][4], 1, 4), goal(currentBoard[2][3], 2, 3), goal(currentBoard[3][2], 3, 2), goal(currentBoard[4][1], 4, 1) };
			goalIt = findGoal();
		}
		else if(color == "BLACK")
		{
			myColorPiece = 'B';
			oppColorPiece = 'W';
			goals = { goal(currentBoard[15][15], 15, 15), goal(currentBoard[15][14], 15, 14), goal(currentBoard[14][15], 14, 15), goal(currentBoard[13][15], 13, 15), goal(currentBoard[14][14], 14, 14),
					  goal(currentBoard[15][13], 15, 13), goal(currentBoard[15][12], 15, 12), goal(currentBoard[14][13], 14, 13), goal(currentBoard[13][14], 13, 14), goal(currentBoard[12][15], 12, 15),
					  goal(currentBoard[11][15], 11, 15), goal(currentBoard[12][14], 12, 14), goal(currentBoard[13][13], 13, 13), goal(currentBoard[14][12], 14, 12), goal(currentBoard[15][11], 15, 11),
					  goal(currentBoard[14][11], 14, 11), goal(currentBoard[13][12], 13, 12), goal(currentBoard[12][13], 12, 13), goal(currentBoard[11][14], 11, 14) };
			goalIt = findGoal();
		}
	}

	return;
}

void getAdjacentMoves(char colorPiece, int node, int x)
{
	if(color == "WHITE")
	{
		if(colorPiece == 'W')
			{
				for(int i = x; i < x + myCount; i++)
				{
					//Check Adjacent moves
					//Check North
					 if((pMoves[layer][i].getJ() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI()] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), pMoves[layer][i].getI(), 1, 'E', node, 0));
					 }

					 //Check North-East
					 if((pMoves[layer][i].getJ() - 1) >= 0 && (pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() + 1] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), (pMoves[layer][i].getI() + 1), 0, 'E', node, 0));
					 }

					 //Check East
					 if((pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 1] == '.')
					 {
						 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() + 1), -1, 'E', node, 0));
					 }

					 //Check South-East
					 if((pMoves[layer][i].getJ() + 1) <= 15 && (pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() + 1 ] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), (pMoves[layer][i].getI() + 1), -2, 'E', node, 0));
					 }

					 //Check South
					 if((pMoves[layer][i].getJ() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI()] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), pMoves[layer][i].getI(), -1, 'E', node, 0));
					 }

					 //Check South-West
					 if((pMoves[layer][i].getJ() + 1) <= 15 && (pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() - 1 ] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), (pMoves[layer][i].getI() - 1), 0, 'E', node, 0));
					 }

					 //Check West
					 if((pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 1] == '.')
					 {
						 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() - 1), 1, 'E', node, 0));
					 }

					 //Check North-West
					 if((pMoves[layer][i].getJ() - 1) >= 0 && (pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() - 1 ] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), (pMoves[layer][i].getI() - 1), 2, 'E', node, 0));
					 }
				}
			}
			else if(colorPiece == 'B')
			{
				for(int i = x; i < x + oppCount; i++)
				{
					//Check Adjacent moves
					//Check North
					 if((pMoves[layer][i].getJ() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI()] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), pMoves[layer][i].getI(), -1, 'E', node, 0));
					 }

					 //Check North-East
					 if((pMoves[layer][i].getJ() - 1) >= 0 && (pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() + 1 ] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), (pMoves[layer][i].getI() + 1), 0, 'E', node, 0));
					 }

					 //Check East
					 if((pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 1] == '.')
					 {
						 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() + 1), 1, 'E', node, 0));
					 }

					 //Check South-East
					 if((pMoves[layer][i].getJ() + 1) <= 15 && (pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() + 1 ] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), (pMoves[layer][i].getI() + 1), 2, 'E', node, 0));
					 }

					 //Check South
					 if((pMoves[layer][i].getJ() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI()] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), pMoves[layer][i].getI(), 1, 'E', node, 0));
					 }

					 //Check South-West
					 if((pMoves[layer][i].getJ() + 1) <= 15 && (pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() - 1 ] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), (pMoves[layer][i].getI() - 1), 0, 'E', node, 0));
					 }

					 //Check West
					 if((pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 1] == '.')
					 {
						 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() - 1), -1, 'E', node, 0));
					 }

					 //Check North-West
					 if((pMoves[layer][i].getJ() - 1) >= 0 && (pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() - 1 ] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), (pMoves[layer][i].getI() - 1), -2, 'E', node, 0));
					 }
				}
			}
	}
	else if(color == "BLACK")
	{
		if(colorPiece == 'W')
		{
			for(int i = x; i < x + oppCount; i++)
			{
				//Check Adjacent moves
				//Check North
				 if((pMoves[layer][i].getJ() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI()] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), pMoves[layer][i].getI(), 1, 'E', node, 0));
				 }

				 //Check North-East
				 if((pMoves[layer][i].getJ() - 1) >= 0 && (pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() + 1 ] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), (pMoves[layer][i].getI() + 1), 0, 'E', node, 0));
				 }

				 //Check East
				 if((pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 1] == '.')
				 {
					 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() + 1), -1, 'E', node, 0));
				 }

				 //Check South-East
				 if((pMoves[layer][i].getJ() + 1) <= 15 && (pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() + 1 ] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), (pMoves[layer][i].getI() + 1), -2, 'E', node, 0));
				 }

				 //Check South
				 if((pMoves[layer][i].getJ() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI()] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), pMoves[layer][i].getI(), -1, 'E', node, 0));
				 }

				 //Check South-West
				 if((pMoves[layer][i].getJ() + 1) <= 15 && (pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() - 1 ] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), (pMoves[layer][i].getI() - 1), 0, 'E', node, 0));
				 }

				 //Check West
				 if((pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 1] == '.')
				 {
					 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() - 1), 1, 'E', node, 0));
				 }

				 //Check North-West
				 if((pMoves[layer][i].getJ() - 1) >= 0 && (pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() - 1 ] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), (pMoves[layer][i].getI() - 1), 2, 'E', node, 0));
				 }
			}
		}
		else if(colorPiece == 'B')
		{
			for(int i = x; i < x + myCount; i++)
			{
				//Check Adjacent moves
				//Check North
				 if((pMoves[layer][i].getJ() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI()] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), pMoves[layer][i].getI(), -1, 'E', node, 0));
				 }

				 //Check North-East
				 if((pMoves[layer][i].getJ() - 1) >= 0 && (pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() + 1 ] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), (pMoves[layer][i].getI() + 1), 0, 'E', node, 0));
				 }

				 //Check East
				 if((pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 1] == '.')
				 {
					 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() + 1), 1, 'E', node, 0));
				 }

				 //Check South-East
				 if((pMoves[layer][i].getJ() + 1) <= 15 && (pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() + 1 ] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), (pMoves[layer][i].getI() + 1), 2, 'E', node, 0));
				 }

				 //Check South
				 if((pMoves[layer][i].getJ() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI()] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), pMoves[layer][i].getI(), 1, 'E', node, 0));
				 }

				 //Check South-West
				 if((pMoves[layer][i].getJ() + 1) <= 15 && (pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() - 1 ] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), (pMoves[layer][i].getI() - 1), 0, 'E', node, 0));
				 }

				 //Check West
				 if((pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 1] == '.')
				 {
					 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() - 1), -1, 'E', node, 0));
				 }

				 //Check North-West
				 if((pMoves[layer][i].getJ() - 1) >= 0 && (pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() - 1 ] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), (pMoves[layer][i].getI() - 1), -2, 'E', node, 0));
				 }
			}
		}
	}

	return;
}

void getJumpMoves(int i, char colorPiece, int node) //TEST IF NEW JUMPS VISIT PREVIOUS JUMPS ex: oscillating
{
	int n;

	if(colorPiece == 'W')
	{
		//Check North
		if((pMoves[layer][i].getJ() - 2) >= 0 && currentBoard[pMoves[layer][i].getJ() - 2][pMoves[layer][i].getI()] == '.' && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI()] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)//loop until p = 0 to get to layer 0
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() - 2) == pMoves[layer][n].getJ() && pMoves[layer][i].getI() == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() - 2) != pMoves[layer][n].getJ())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 2), pMoves[layer][i].getI(), pMoves[layer][i].getAdvance() + 3, 'J', node, 0));
				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check North-East
		if((pMoves[layer][i].getJ() - 2) >= 0 && (pMoves[layer][i].getI() + 2) <= 15 && currentBoard[pMoves[layer][i].getJ() - 2][pMoves[layer][i].getI() + 2] == '.' && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() + 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() - 2) == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() - 2) != pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 2), (pMoves[layer][i].getI() + 2), pMoves[layer][i].getAdvance(), 'J', node, 0));
				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check East
		if((pMoves[layer][i].getI() + 2) <= 15 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 2] == '.' && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if(pMoves[layer][i].getJ() == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getI() + 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() + 2), pMoves[layer][i].getAdvance() - 1, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check South-East
		if((pMoves[layer][i].getJ() + 2) <= 15 && (pMoves[layer][i].getI() + 2) <= 15 && currentBoard[pMoves[layer][i].getJ() + 2][pMoves[layer][i].getI() + 2] == '.' && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() + 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() + 2) == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() + 2) != pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 2), (pMoves[layer][i].getI() + 2), pMoves[layer][i].getAdvance() - 1, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check South
		if((pMoves[layer][i].getJ() + 2) <= 15 && currentBoard[pMoves[layer][i].getJ() + 2][pMoves[layer][i].getI()] == '.' && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI()] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() + 2) == pMoves[layer][n].getJ() && pMoves[layer][i].getI() == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() + 2) != pMoves[layer][n].getJ())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 2), pMoves[layer][i].getI(), pMoves[layer][i].getAdvance() - 1, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check South-West
		if((pMoves[layer][i].getJ() + 2) <= 15 && (pMoves[layer][i].getI() - 2) >= 0 && currentBoard[pMoves[layer][i].getJ() + 2][pMoves[layer][i].getI() - 2] == '.' && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() - 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() + 2) == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() + 2) != pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 2), (pMoves[layer][i].getI() - 2), pMoves[layer][i].getAdvance(), 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check West
		if((pMoves[layer][i].getI() - 2) >= 0 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 2] == '.'&& currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if(pMoves[layer][i].getJ() == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getI() - 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() - 2), pMoves[layer][i].getAdvance() + 3, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check North-West
		if((pMoves[layer][i].getJ() - 2) >= 0 && (pMoves[layer][i].getI() - 2) >= 0 && currentBoard[pMoves[layer][i].getJ() - 2][pMoves[layer][i].getI() - 2] == '.' && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() - 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() - 2) == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() - 2) != pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 2), (pMoves[layer][i].getI() - 2), pMoves[layer][i].getAdvance() + 7, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}
	}
	else if(colorPiece == 'B')
	{
		//Check North
		if((pMoves[layer][i].getJ() - 2) >= 0 && currentBoard[pMoves[layer][i].getJ() - 2][pMoves[layer][i].getI()] == '.' && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI()] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() - 2) == pMoves[layer][n].getJ() && pMoves[layer][i].getI() == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() - 2) != pMoves[layer][n].getJ())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 2), pMoves[layer][i].getI(), pMoves[layer][i].getAdvance() - 1, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check North-East
		if((pMoves[layer][i].getJ() - 2) >= 0 && (pMoves[layer][i].getI() + 2) <= 15 && currentBoard[pMoves[layer][i].getJ() - 2][pMoves[layer][i].getI() + 2] == '.' && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() + 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() - 2) == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() - 2) != pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 2), (pMoves[layer][i].getI() + 2), pMoves[layer][i].getAdvance(), 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check East
		if((pMoves[layer][i].getI() + 2) <= 15 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 2] == '.' && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if(pMoves[layer][i].getJ() == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getI() + 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() + 2), pMoves[layer][i].getAdvance() + 3, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check South-East
		if((pMoves[layer][i].getJ() + 2) <= 15 && (pMoves[layer][i].getI() + 2) <= 15 && currentBoard[pMoves[layer][i].getJ() + 2][pMoves[layer][i].getI() + 2] == '.' && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() + 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() + 2) == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() + 2) != pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 2), (pMoves[layer][i].getI() + 2), pMoves[layer][i].getAdvance() + 7, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check South
		if((pMoves[layer][i].getJ() + 2) <= 15 && currentBoard[pMoves[layer][i].getJ() + 2][pMoves[layer][i].getI()] == '.' && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI()] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() + 2) == pMoves[layer][n].getJ() && pMoves[layer][i].getI() == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() + 2) != pMoves[layer][n].getJ())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 2), pMoves[layer][i].getI(), pMoves[layer][i].getAdvance() + 3, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check South-West
		if((pMoves[layer][i].getJ() + 2) <= 15 && (pMoves[layer][i].getI() - 2) >= 0 && currentBoard[pMoves[layer][i].getJ() + 2][pMoves[layer][i].getI() - 2] == '.' && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() - 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() + 2) == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() + 2) != pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 2), (pMoves[layer][i].getI() - 2), pMoves[layer][i].getAdvance(), 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check West
		if((pMoves[layer][i].getI() - 2) >= 0 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 2] == '.'&& currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if(pMoves[layer][i].getJ() == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getI() - 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() - 2), pMoves[layer][i].getAdvance() - 1, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check North-West
		if((pMoves[layer][i].getJ() - 2) >= 0 && (pMoves[layer][i].getI() - 2) >= 0 && currentBoard[pMoves[layer][i].getJ() - 2][pMoves[layer][i].getI() - 2] == '.' && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() - 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() - 2) == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() - 2) != pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 2), (pMoves[layer][i].getI() - 2), pMoves[layer][i].getAdvance() - 1, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}
	}

	return;
}

void getStartJumpMoves(int i, char colorPiece, int node)
{
	int n;

	if(colorPiece == 'W')
	{
		//Check North
		if((pMoves[layer][i].getJ() - 2) >= 0 && currentBoard[pMoves[layer][i].getJ() - 2][pMoves[layer][i].getI()] == '.' && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI()] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)//loop until p = 0 to get to layer 0
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() - 2) == pMoves[layer][n].getJ() && pMoves[layer][i].getI() == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() - 2) != pMoves[layer][n].getJ())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 2), pMoves[layer][i].getI(), pMoves[layer][i].getAdvance() + 3, 'J', node, 0));
				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check West
		if((pMoves[layer][i].getI() - 2) >= 0 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 2] == '.'&& currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if(pMoves[layer][i].getJ() == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getI() - 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() - 2), pMoves[layer][i].getAdvance() + 3, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check North-West
		if((pMoves[layer][i].getJ() - 2) >= 0 && (pMoves[layer][i].getI() - 2) >= 0 && currentBoard[pMoves[layer][i].getJ() - 2][pMoves[layer][i].getI() - 2] == '.' && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() - 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() - 2) == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() - 2) != pMoves[layer][n].getJ() && (pMoves[layer][i].getI() - 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 2), (pMoves[layer][i].getI() - 2), pMoves[layer][i].getAdvance() + 7, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}
	}
	else if(colorPiece == 'B')
	{
		//Check East
		if((pMoves[layer][i].getI() + 2) <= 15 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 2] == '.' && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if(pMoves[layer][i].getJ() == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getI() + 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() + 2), pMoves[layer][i].getAdvance() + 3, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check South-East
		if((pMoves[layer][i].getJ() + 2) <= 15 && (pMoves[layer][i].getI() + 2) <= 15 && currentBoard[pMoves[layer][i].getJ() + 2][pMoves[layer][i].getI() + 2] == '.' && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() + 1] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() + 2) == pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() + 2) != pMoves[layer][n].getJ() && (pMoves[layer][i].getI() + 2) != pMoves[layer][n].getI())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 2), (pMoves[layer][i].getI() + 2), pMoves[layer][i].getAdvance() + 7, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}

		//Check South
		if((pMoves[layer][i].getJ() + 2) <= 15 && currentBoard[pMoves[layer][i].getJ() + 2][pMoves[layer][i].getI()] == '.' && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI()] != '.')
		{
			n = i;
			while(pMoves[layer][n].getParent() != -1)
			{
				n = pMoves[layer][n].getParent();
				if((pMoves[layer][i].getJ() + 2) == pMoves[layer][n].getJ() && pMoves[layer][i].getI() == pMoves[layer][n].getI())
				{
					n = -2;
					break;
				}
			}
			if(n >= 0 && (pMoves[layer][i].getJ() + 2) != pMoves[layer][n].getJ())
			{
				pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 2), pMoves[layer][i].getI(), pMoves[layer][i].getAdvance() + 3, 'J', node, 0));

				getJumpMoves(pMoves[layer].size() - 1, colorPiece, node);
			}
		}
	}
	return;
}

void getStartAdjacentMoves(char colorPiece, int node, int x)
{
	if(color == "WHITE")
	{
		if(colorPiece == 'W')
			{
				for(int i = x; i < x + myCount; i++)
				{
					//Check Adjacent moves
					//Check North
					 if((pMoves[layer][i].getJ() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI()] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), pMoves[layer][i].getI(), 1, 'E', node, 0));
					 }

					 //Check West
					 if((pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 1] == '.')
					 {
						 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() - 1), 1, 'E', node, 0));
					 }

					 //Check North-West
					 if((pMoves[layer][i].getJ() - 1) >= 0 && (pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() - 1 ] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), (pMoves[layer][i].getI() - 1), 2, 'E', node, 0));
					 }
				}
			}
			else if(colorPiece == 'B')
			{
				for(int i = x; i < x + oppCount; i++)
				{
					 //Check East
					 if((pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 1] == '.')
					 {
						 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() + 1), 1, 'E', node, 0));
					 }

					 //Check South-East
					 if((pMoves[layer][i].getJ() + 1) <= 15 && (pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() + 1 ] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), (pMoves[layer][i].getI() + 1), 2, 'E', node, 0));
					 }

					 //Check South
					 if((pMoves[layer][i].getJ() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI()] == '.')
					 {
						 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), pMoves[layer][i].getI(), 1, 'E', node, 0));
					 }
				}
			}
	}
	else if(color == "BLACK")
	{
		if(colorPiece == 'W')
		{
			for(int i = x; i < x + oppCount; i++)
			{
				//Check Adjacent moves
				//Check North
				 if((pMoves[layer][i].getJ() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI()] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), pMoves[layer][i].getI(), 1, 'E', node, 0));
				 }

				 //Check West
				 if((pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() - 1] == '.')
				 {
					 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() - 1), 1, 'E', node, 0));
				 }

				 //Check North-West
				 if((pMoves[layer][i].getJ() - 1) >= 0 && (pMoves[layer][i].getI() - 1) >= 0 && currentBoard[pMoves[layer][i].getJ() - 1][pMoves[layer][i].getI() - 1 ] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() - 1), (pMoves[layer][i].getI() - 1), 2, 'E', node, 0));
				 }
			}
		}
		else if(colorPiece == 'B')
		{
			for(int i = x; i < x + myCount; i++)
			{
				 //Check East
				 if((pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ()][pMoves[layer][i].getI() + 1] == '.')
				 {
					 pMoves[layer].push_back(moves(i, pMoves[layer][i].getJ(), (pMoves[layer][i].getI() + 1), 1, 'E', node, 0));
				 }

				 //Check South-East
				 if((pMoves[layer][i].getJ() + 1) <= 15 && (pMoves[layer][i].getI() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI() + 1 ] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), (pMoves[layer][i].getI() + 1), 2, 'E', node, 0));
				 }

				 //Check South
				 if((pMoves[layer][i].getJ() + 1) <= 15 && currentBoard[pMoves[layer][i].getJ() + 1][pMoves[layer][i].getI()] == '.')
				 {
					 pMoves[layer].push_back(moves(i, (pMoves[layer][i].getJ() + 1), pMoves[layer][i].getI(), 1, 'E', node, 0));
				 }
			}
		}
	}


	return;
}

void getLegalMoves(char colorPiece, int node)
{
	unsigned int x = pMoves[layer].size();
	myCount = 0;
	oppCount = 0;
	int m = 0;
	int w = 0;
	int inStartingArea = 0;

	if(colorPiece == myColorPiece)
	{
		if(gameType == "GAME")
		{
			while(w < 19) //iterate through myPieces
			{
				if(goalIt < 10)
				{
					m = 0;
					while(m < goalIt) //iterate through goal spots already filled to find which
					{
						if(goals[m].getJ() == myPieces[w].getJ() && goals[m].getI() == myPieces[w].getI())
						{
							w = w + 1;
							break;
						}
						m = m + 1;
					}
					if(m == goalIt)
					{
						pMoves[layer].push_back(moves(-1, myPieces[w].getJ(), myPieces[w].getI(), 0, 'N', node, 0));
						myCount = myCount + 1;
						w = w + 1;
					}
				}
				else
				{
					m = 0;
					while(m < 19) //iterate through goal spots already filled to find which
					{
						if(goals[m].getJ() == myPieces[w].getJ() && goals[m].getI() == myPieces[w].getI())
						{
							w = w + 1;
							break;
						}
						m = m + 1;
					}
					if(m == 19)
					{
						pMoves[layer].push_back(moves(-1, myPieces[w].getJ(), myPieces[w].getI(), 0, 'N', node, 0));
						myCount = myCount + 1;
						w = w + 1;
					}
				}

			}
			for(unsigned int i = x; i < (x + myCount); i++)
			{
				getJumpMoves(i, colorPiece, node);
			}
			getAdjacentMoves(colorPiece, node, x);
		}
		else if(gameType == "SINGLE")
		{

			for(int w = 0; w < 19; w++)
			{
				for(int m = 0; m < 19; m++)
				{
					if(startingArea[m].getJ() == myPieces[w].getJ() && startingArea[m].getI() == myPieces[w].getI())
					{
						pMoves[layer].push_back(moves(-1, myPieces[w].getJ(), myPieces[w].getI(), 0, 'N', node, 0));
						myCount = myCount + 1;
						inStartingArea = 1;
						break;
					}
				}
			}

			if(inStartingArea == 0)
			{
				for(int w = 0; w < 19; w++)
				{
					for(int m = 0; m < 19; m++)
					{
						if(goals[m].getJ() == myPieces[w].getJ() && goals[m].getI() == myPieces[w].getI())
						{
							break;
						}
						if(m == 18)
						{
							pMoves[layer].push_back(moves(-1, myPieces[w].getJ(), myPieces[w].getI(), 0, 'N', node, 0));
							myCount = myCount + 1;
						}
					}
				}
				for(unsigned int i = x; i < (x + myCount); i++)
				{
					getJumpMoves(i, colorPiece, node);
				}
				getAdjacentMoves(colorPiece, node, x);
			}
			else if(inStartingArea == 1)
			{
				for(unsigned int i = x; i < (x + myCount); i++)
				{
					getStartJumpMoves(i, colorPiece, node);
				}
				getStartAdjacentMoves(colorPiece, node, x);


				if(pMoves[layer].size() == (x + myCount))
				{
					while(myCount != 0)
					{
						pMoves[layer].pop_back();
						myCount = myCount - 1;
					}
					for(int w = 0; w < 19; w++)
					{
						for(int m = 0; m < 19; m++)
						{
							if(goals[m].getJ() == myPieces[w].getJ() && goals[m].getI() == myPieces[w].getI())
							{
								break;
							}
							if(m == 18)
							{
								pMoves[layer].push_back(moves(-1, myPieces[w].getJ(), myPieces[w].getI(), 0, 'N', node, 0));
								myCount = myCount + 1;
							}
						}
					}
					for(unsigned int i = x; i < (x + myCount); i++)
					{
						getJumpMoves(i, colorPiece, node);
					}
					getAdjacentMoves(colorPiece, node, x);
				}
			}
		}

	}
	else if(colorPiece == oppColorPiece)
	{
		for(int w = 0;  w < 19; w++)
		{
			pMoves[layer].push_back(moves(-1, oppPieces[w].getJ(), oppPieces[w].getI(), 0, 'N', node, 0));
			oppCount = oppCount + 1;
		}

		for(unsigned int i = x; i < (x + oppCount); i++)
		{
			getJumpMoves(i, colorPiece, node);
		}
		getAdjacentMoves(colorPiece, node, x);
	}
}

void swapBoard(moves currentMove)
{
	//make swap on currentBoard according to currentMove
	//update my pieces
	//add my pieces to my moves
	char swap;
	int n = currentMove.getParent();
	while(pMoves[layer][n].getParent() != -1)//loop until p = 0 to get to layer 0
	{
		n = pMoves[layer][n].getParent();
	}

	//Update board based on move
	swap = currentBoard[pMoves[layer][n].getJ()][pMoves[layer][n].getI()];
	currentBoard[pMoves[layer][n].getJ()][pMoves[layer][n].getI()] = currentBoard[currentMove.getJ()][currentMove.getI()];
	currentBoard[currentMove.getJ()][currentMove.getI()] = swap;

	return;
}

void updatePieces()
{
	//Update Board --> increment/decrement layer --> Update Pieces --> updateParentMoves
	myPieces.clear();
	oppPieces.clear();

	for(int j = 0; j < 16; j++)
	{
		for(int i = 0; i < 16; i++)
		{
			if(currentBoard[j][i] == myColorPiece) // My Pieces
			{
				myPieces.push_back(pieces(currentBoard[j][i], j, i, 0, 0));

			}
			else if(currentBoard[j][i] == oppColorPiece) // Opponent's Pieces
			{
				oppPieces.push_back(pieces(currentBoard[j][i], j, i, 0, 0));
			}
		}
	}

	return;
}

moves getUtility(moves currentMove)
{
	int arr[6] = {0, 0, 0, 0, 0, 0};
	//calculate
	//A = sum of squared distances to goal, B= sum of squared distances to equator, C = sum of squared advances for pieces
	for(int i = 0; i < 19; i++)
	{

		//Sum of A's
		arr[0] = arr[0] + calculateGoalDistance(myPieces[i].getJ(), myPieces[i].getI(), myColorPiece);
		arr[1] = arr[1] + calculateGoalDistance(oppPieces[i].getJ(), oppPieces[i].getI(), oppColorPiece);

		//Sup of B's
		arr[2] = arr[2] + calculateEquator(myPieces[i].getJ(), myPieces[i].getI());
		arr[3] = arr[3] + calculateEquator(oppPieces[i].getJ(), oppPieces[i].getI());
	}

	arr[0] = pow(arr[0], 2);
	arr[1] = pow(arr[1], 2);

	arr[2] = pow(arr[2], 2);
	arr[3] = pow(arr[3], 2);

	int lay = goalLayer;
	int node = currentMove.getNode();
	while(lay > 0)
	{
		if(lay == goalLayer)
		{
			if(lay % 2 == 0) //Even - opp
			{
				arr[5] = arr[5] + currentMove.getAdvance();
			}
			else //Odd - my
			{
				arr[4] = arr[4] + currentMove.getAdvance();
			}
		}
		else
		{
			if(lay % 2 == 0) //Even
			{
				arr[5] = arr[5] + pMoves[lay][node].getAdvance();
				node = pMoves[lay][node].getNode();
			}
			else //Odd
			{
				arr[4] = arr[4] + pMoves[lay][node].getAdvance();
				node = pMoves[lay][node].getNode();
			}
		}

		lay = lay - 1;
	}


	int heuristic = (arr[1] - arr[0]) + ((arr[3] - arr[2])/100) + int((arr[4] - arr[5])*.4);

	currentMove.setHeuristic(heuristic);
	return currentMove;
}

moves minValue(moves currentMove, int alpha, int beta, int node);

moves maxValue(moves currentMove, int alpha, int beta, int node)
{
	if(layer == goalLayer) //Terminal Test
	{
		//Calculate Heuristic
		updatePieces();
		moves p = getUtility(currentMove);
		return p;
	}

	// V <== -infinity
	moves v = moves(-1, -1, -1, -1, 'V', -1 , -100000000);

	layer = layer + 1;

	//Find all legal moves from state
	unsigned int x = pMoves[layer].size();
	updatePieces();
	getLegalMoves(myColorPiece, node);
	unsigned int y = x + myCount;
	int w = pMoves[layer].size();

	//for each a in actions(state) do
	for(int z = y; z < w; z++)
	{
		//create new board state from move
		swapBoard(pMoves[layer][z]);
		v = compareMax(v, minValue(pMoves[layer][z], alpha, beta, z));
		if(v.getHeuristic() >= beta)
		{
			swapBoard(pMoves[layer][z]);
			layer = layer - 1;
			return v;
		}
		alpha = Max(alpha, v);
		//change back to previous board
		swapBoard(pMoves[layer][z]);
	}
	layer = layer - 1;
	return v;
}

moves minValue(moves currentMove, int alpha, int beta, int node)
{
	if(layer == goalLayer) //Terminal Test
	{
		//Calculate Heuristic
		updatePieces();
		moves p = getUtility(currentMove);
		return p;
	}

	// V <== +infinity
	moves v = moves(-1, -1, -1, -1, 'V', -1 , 100000000);

	layer = layer + 1;

	//Find all legal moves from state
	unsigned int x = pMoves[layer].size();
	updatePieces();
	getLegalMoves(oppColorPiece, node);
	unsigned int y = x + oppCount;
	int w = pMoves[layer].size();

	//for each a in actions(state) do
	for(int z = y; z < w; z++)
	{
		//create new board state from move
		swapBoard(pMoves[layer][z]);
		v = compareMin(v, maxValue(pMoves[layer][z], alpha, beta, z));
		if(v.getHeuristic() <= alpha)
		{
			swapBoard(pMoves[layer][z]);
			layer = layer - 1;
			return v;
		}
		beta = Min(beta, v);
		//change back to previous board
		swapBoard(pMoves[layer][z]);
	}
	layer = layer - 1;
	return v;
}

moves alphaBetaSearch()
{
	moves w = moves(-1, -1, -1, -1, 'W', -1 , -1);
	moves v = maxValue(w, -100000000, 100000000, 0);
	return v;
}

int main() {


	readInput();
	if(gameType == "GAME")
	{
		if(turn < 21)
		{
			startingMoves();
		}
		else
		{
			if(goalIt >= 16)
			{
				goalLayer = 1;
			}
			moves s = alphaBetaSearch();
			createOutput(s);
		}

		//Increment Turn
		if(turn == 0)
		{
			ofstream pdata("playdata.txt");
			turn = turn + 1;
			pdata << turn;
			pdata.close();
		}
		else if(turn > 0)
		{
			remove("playdata.txt");
			ofstream pdata("playdata.txt");
			turn = turn + 1;
			pdata << turn;
			pdata.close();
		}
	}
	else
	{
		moves s = alphaBetaSearch();
		createOutput(s);
	}

	return 0;
}
