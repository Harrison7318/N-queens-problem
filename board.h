#include <iostream>
#include <vector>
#include <string>

using namespace std;

class board 
{
private:
	int boardSize;
	int maxH;
	int** hueristicBoard;
	char** queenBoard;
	int* queenPos;
	int iterations;
	double startTemp;
	double temperature;

public:
	//constructor bSize = n from main()
	board(int bSize);

	//fills queen board with ' ' and calls clearHueristicBoard()
	void initializeBoard();

	//sets all of hueristic board to max hueristic
	void clearHueristicBoard();

	//places queens randomly in each column
	void placeQueensRandomly();

	//fills hueristic board using getThreats()
	//returns true if finishState() is true
	bool getHueristics();

	//finds threats from other queens on spot [x][y]
	int getThreats(int x, int y);

	//sets queenBoard = to oldBoard, and if moving, moves the queen in column x to x,y.
	void setBoard(char *oldBoard[], bool moving, int x, int y);

	//use hueristic board and temperature to find best neighbor and go there
	bool findBestNeighbor();

	//if temperature is to low return true and finish
	bool finishTemp();

	//If there are 0 threats on the board return true and finish
	bool finishState();

	//returns queen Board in string.
	string printBoard();

	//returns hueristic board in string.
	string printHueristicBoard();

	//returns iterations
	int getIterations();
};

board::board(int bSize) {
	boardSize = bSize;

	//initalize hueristic board to be boardSize x boardSize
	hueristicBoard = new int* [boardSize];
	for (int i = 0; i < boardSize; i++)
		hueristicBoard[i] = new int[boardSize];

	//initalize queen board to be boardSize x boardSize
	queenBoard = new char* [boardSize];
	for (int i = 0; i < boardSize; i++)
		queenBoard[i] = new char[boardSize];

	//initalize queen Hueristics array
	queenPos = new int [boardSize];

	iterations = 0;
	startTemp = 100000; //adjust starting temp for program here
	temperature = startTemp;

	srand(time(0));
}

//fills hueristic board and queen hueristic with max hueristic and queen board with ' '
void board::initializeBoard()
{
	clearHueristicBoard();
	
	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize; j++)
			queenBoard[i][j] = 'X';

	for (int i = 0; i < boardSize; i++)
		queenPos[i] = boardSize;
}

//sets all of hueristic board to max hueristic
void board::clearHueristicBoard()
{
	int queenCount = boardSize;
	maxH = 0;

	for (int i = 0; i < boardSize; i++)
	{
		maxH += queenCount;
		queenCount--;
	}
		
	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize; j++)
			hueristicBoard[i][j] = maxH;
}

//places queens randomly in each column
void board::placeQueensRandomly()
{
	
	for (int i = 0; i < boardSize; i++)
	{
		int randNum = rand() % boardSize;
		queenBoard[randNum][i] = 'Q';
	}
}

//fills board with all threats on the board if queen was in that space
bool board::getHueristics()
{
	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize; j++)
			if (queenBoard[j][i] == 'Q')
				queenPos[i] = j;

	int queenHueristic = 0;
	for (int i = 0; i < boardSize; i++)
		queenHueristic += getThreats(i, queenPos[i]);

	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize; j++)
		{
			//fill hueristic board
			if (queenBoard[j][i] == 'Q')
				hueristicBoard[j][i] = queenHueristic;
			else
			{
				int hueristic = getThreats(i, j);

				board tempboard(boardSize);
				tempboard.setBoard(queenBoard, true, j, i);

				for (int k = 0; k < boardSize; k++)
					if (k != i)//Don't get same column we already got.
						hueristic += tempboard.getThreats(k, queenPos[k]);

				hueristicBoard[j][i] = hueristic;
			}
		}

	if (finishState())
		return true;//exit
	return false;//continue
}

//finds threats from other queens on spot [x][y]
int board::getThreats(int x, int y) 
{
	int hueristic = 0;

	//search horizontally for threatinging queens
	for (int i = x + 1; i < boardSize; i++)
		if (queenBoard[y][i] == 'Q')
			hueristic++;

	//search vertically for threatinging queens
	/*for (int k = 0; k < boardSize; k++)
		if (k != y && queenBoard[x][k] == 'Q')
			h++;*/

	//search diagonally for threatinging queens
	int i = x + 1;
	int j = y + 1;
	while (i < boardSize && j < boardSize)
	{
		if (queenBoard[j][i] == 'Q')
			hueristic++;
		i++;
		j++;
	}

	i = x + 1;
	j = y - 1;
	while (i < boardSize && j >= 0)
	{
		if (queenBoard[j][i] == 'Q')
			hueristic++;
		i++;
		j--;
	}

	return hueristic; //total threats on board[x][y]
}

//sets queenBoard = to oldBoard, and if moving, moves the queen in column x to x,y.
void board::setBoard(char* oldBoard[], bool moving, int x, int y)
{
	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize; j++)
			queenBoard[i][j] = oldBoard[i][j];

	if (moving)
	{
		for (int i = 0; i < boardSize; i++)
			if (queenBoard[i][x] == 'Q')
				queenBoard[i][x] == 'X';
		queenBoard[x][y] = 'Q';
	}
}

//use hueristic board and temperature to find best neighbor and go there
bool board::findBestNeighbor()
{
	int lowestHueristic = maxH;
	vector<int> lowestHPosX;
	vector<int> lowestHPosY;
	lowestHPosX.push_back(0);
	lowestHPosY.push_back(queenPos[0]);

	//find lowest hueristic position
	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize; j++)
			if (hueristicBoard[j][i] < lowestHueristic)
				lowestHueristic = hueristicBoard[j][i];

	//if two hueristics are the same put then in the vector then choose one randomly
	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize; j++)
			if (hueristicBoard[j][i] == lowestHueristic)
			{
				lowestHPosX.push_back(i);
				lowestHPosY.push_back(j);
			}
	int size = lowestHPosX.size();
	int randNum = rand() % size;
		

	if (queenBoard[lowestHPosY[randNum]][lowestHPosX[randNum]] != 'Q')
	{
		queenBoard[queenPos[lowestHPosX[randNum]]][lowestHPosX[randNum]] = 'X';
		queenBoard[lowestHPosY[randNum]][lowestHPosX[randNum]] = 'Q';
		queenPos[lowestHPosX[randNum]] = lowestHPosY[randNum];
		
		iterations++;
		temperature = (temperature / (iterations * 0.25));
		return true;
	}
	else //queen is lowest hueristic, possibly make a worse move
	{
		lowestHueristic = maxH;
		lowestHPosX.clear();
		lowestHPosY.clear();

		//find lowest hueristic on board thats not a queen.
		for (int i = 0; i < boardSize; i++)
			for (int j = 0; j < boardSize; j++)
				if (j != queenPos[i])//ignore if queen
					if (hueristicBoard[j][i] < lowestHueristic)
						lowestHueristic = hueristicBoard[j][i];

		//if two hueristics are the same put then in the vector then choose one randomly
		for (int i = 0; i < boardSize; i++)
			for (int j = 0; j < boardSize; j++)
				if (hueristicBoard[j][i] == lowestHueristic)
				{
					lowestHPosX.push_back(i);
					lowestHPosY.push_back(j);
				}

		//percentage chance to use worse state
		double percentageChance = exp((hueristicBoard[0][queenPos[0]] - lowestHueristic) / temperature);
		percentageChance = percentageChance * startTemp;

		if (rand() % (int)temperature > percentageChance) //if rand is greater than % chance don't make change
		{
			iterations++;
			temperature = (temperature / (iterations * 0.25));
			return false;
		}
		else //make a "worse" move
		{
			bool switched = false;
			int maxLoops = boardSize * boardSize;
			int loops = 0;

			while (!switched)
			{
				int size = lowestHPosX.size();
				int randNum = rand() % size;

				if (queenBoard[lowestHPosY[randNum]][lowestHPosX[randNum]] == 'Q')
					switched = false;
				else
				{
					queenBoard[queenPos[lowestHPosX[randNum]]][lowestHPosX[randNum]] = 'X';
					queenBoard[lowestHPosY[randNum]][lowestHPosX[randNum]] = 'Q';
					queenPos[lowestHPosX[randNum]] = lowestHPosY[randNum];
					switched = true;
				}

				if (loops > maxLoops)//in case of all indexs in vector are a 'Q'
					break;
				loops++;
			}

			iterations++;
			temperature = (temperature / (iterations * 0.25));
			return true;
		}
	}
}

//if temperature is to low return true and finish
bool board::finishTemp()
{
	if (temperature < 1)
		return true;
	return false;
}

//returns queen Board in string.
string board::printBoard()
{
	string wholeBoard = "";
	string lines = "";

	for (int i = 0; i < boardSize; i++)
		lines += "__";
	lines += "_";
	wholeBoard += lines + "\n";
	
	for (int i = 0; i < boardSize; i++)
	{
		string thisLine = "|";
		for (int j = 0; j < boardSize; j++)
		{
			thisLine += queenBoard[i][j];
			thisLine += "|";
		}
		wholeBoard += thisLine + "\n";
		wholeBoard += lines + "\n";
	}

	return wholeBoard;
}

//returns hueristic board in string.
string board::printHueristicBoard()
{
	string wholeBoard = "";
	string lines = "";

	for (int i = 0; i < boardSize; i++)
		lines += "__";
	lines += "_";
	wholeBoard += lines + "\n";

	for (int i = 0; i < boardSize; i++)
	{
		string thisLine = "|";
		for (int j = 0; j < boardSize; j++)
		{
			if (queenBoard[i][j] == 'Q')
			{
				thisLine += 'Q';
				thisLine += "|";
			}
			else
			{
				int currentNum = hueristicBoard[i][j];
				thisLine += to_string(currentNum);
				thisLine += "|";
			}
		}
		wholeBoard += thisLine + "\n";
		wholeBoard += lines + "\n";
	}

	return wholeBoard;
}

//If there are 0 threats on the board return true and finish
bool board::finishState()
{
	if (hueristicBoard[queenPos[0]][0] == 0)
		return true;
	return false;
}

//returns iterations
int board::getIterations(){
	return iterations;
}