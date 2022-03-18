#include <iostream>
#include "board.h"

using namespace std;

void runABoard(board thisBoard);

int success = 0;
int fail = 0;

int main()
{
	int n;

	cout << "What size board would you like?" << endl;
	cout << "n = ";
	cin >> n; 
	cout << endl;

	//creates 10 boards the size of n x n
	board theBoard(n);
	board theBoard1(n);
	board theBoard2(n);
	board theBoard3(n);
	board theBoard4(n);
	board theBoard5(n);
	board theBoard6(n);
	board theBoard7(n);
	board theBoard8(n);
	board theBoard9(n);

	//tests all 10 boards
	runABoard(theBoard);
	runABoard(theBoard1);
	runABoard(theBoard2);
	runABoard(theBoard3);
	runABoard(theBoard4);
	runABoard(theBoard5);
	runABoard(theBoard6);
	runABoard(theBoard7);
	runABoard(theBoard8);
	runABoard(theBoard9);
		
	cout << "Total Success: " << success << endl;
	cout << "Total Failures: " << fail << endl;

	system("pause");
	return 0;
}

void runABoard(board thisBoard)
{
	thisBoard.initializeBoard();

	thisBoard.placeQueensRandomly();
	cout << "Starting board: \n" << thisBoard.printBoard() << "\n\n";

	thisBoard.getHueristics();
	cout << "Starting hueristic board: \n" << thisBoard.printHueristicBoard() << "\n\n";

	while (true)
	{
		if (thisBoard.getHueristics())
			break;
		thisBoard.findBestNeighbor();

		if (thisBoard.finishState() == true || thisBoard.finishTemp() == true)
			break;
	}

	cout << "Finished board: \n" << thisBoard.printBoard() << "\n";
	if (thisBoard.finishState()) {
		cout << "Succeded" << endl;
		success++;
	}
	else {
		cout << "Failed" << endl;
		fail++;
	}
		
	cout << "iterations: " << thisBoard.getIterations() << "\n-------------\n\n";
}