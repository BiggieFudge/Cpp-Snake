#include <iostream>
#include <windows.h>
#include <synchapi.h>
#include <stdlib.h>     
#include <conio.h>
#include <time.h> 
using namespace std;




struct snake {
	int size = 1;
	int* x; int* y;
	int direction; /*1-up, 2-left, 3-down, 4-right*/

};


struct apple {
	int x;
	int y;
};


snake Snake;
apple Apple;


bool gameover(int rows,int columns) {
	if (Snake.x[0] == -1 || Snake.x[0] == rows -2 || Snake.y[0] == -1 || Snake.y[0] == columns-2) {
		system("CLS");
		cout << "Game Over";
		return false;
	}
	return true;
}


bool SearchArray(int i, int j) {
	int x = 0, y = 0;

	for (int x = 0; x < Snake.size; x++) {
		if (Snake.x[x] == j && Snake.y[x] == i) {
			return true;
		}
	}
	return false;
}

void ForwardTheLast() {
	if (Snake.size == 1)
		return;

	int x = Snake.x[Snake.size - 1], i;
	for (i = Snake.size - 1; i > 0; i--)
		Snake.x[i] = Snake.x[i - 1];
	Snake.x[0] = x;

	x = Snake.y[Snake.size - 1], i;
	for (i = Snake.size - 1; i > 0; i--)
		Snake.y[i] = Snake.y[i - 1];
	Snake.y[0] = x;
}

void GenerateApple(int rows,int columns){
	Apple.x = (rand() % (rows - 2)) ;
	Apple.y = (rand() % (columns - 2)) ;
}

bool ChangeDirection() {
	SHORT UpkeyState = GetKeyState(VK_UP/*(UP)*/);
	bool UpisToggled = (UpkeyState & 0x8000);
	SHORT LeftkeyState = GetKeyState(VK_LEFT/*(Left)*/);
	bool LeftisToggled = (LeftkeyState & 0x8000 );
	SHORT DownkeyState = GetKeyState(VK_DOWN/*(down)*/);
	bool DownisToggled = (DownkeyState & 0x8000);
	SHORT RightkeyState = GetKeyState(VK_RIGHT/*(right)*/);
	bool RightisToggled = (RightkeyState & 0x8000);

	if (UpisToggled && Snake.direction!=3) {
		Snake.direction = 1;
	}
	else if (LeftisToggled&& Snake.direction != 4) {
		Snake.direction = 2;
	}
	else if (DownisToggled&& Snake.direction != 1) {
		Snake.direction = 3;
	}
	else if (RightisToggled&& Snake.direction != 2) {
		Snake.direction = 4;
	}
	return true;
}


bool move(int rows,int columns) {

	if ((Snake.direction==1 && 
		Snake.y[0] == Apple.y && Snake.x[0]-1 == Apple.x) ||
		(Snake.direction == 2 &&
			Snake.y[0]-1 == Apple.y && Snake.x[0] == Apple.x) || 
			(Snake.direction == 3 &&
				Snake.y[0] == Apple.y && Snake.x[0] + 1 == Apple.x) ||
				(Snake.direction == 4 &&
					Snake.y[0] +1 == Apple.y && Snake.x[0] == Apple.x) ) { 
		/* Eats Apple*/
		GenerateApple(rows, columns);

		Snake.x = (int*)realloc(Snake.x, ++Snake.size * sizeof(int));
		Snake.y = (int*)realloc(Snake.y, Snake.size * sizeof(int));
		switch(Snake.direction) {
			
		case(1):
			Snake.x[Snake.size-1] = Snake.x[0] - 1;
			Snake.y[Snake.size-1] = Snake.y[0];
			break;
		case(2):
			Snake.x[Snake.size-1] = Snake.x[0];
			Snake.y[Snake.size-1] = Snake.y[0] - 1;
			break;
		case(3):
			Snake.x[Snake.size-1] = Snake.x[0] + 1;
			Snake.y[Snake.size-1] = Snake.y[0];
			break;
		case(4):
			Snake.x[Snake.size-1] = Snake.x[0];
			Snake.y[Snake.size-1] = Snake.y[0] + 1;
			break;
		}
		ForwardTheLast();
	}

	else {
		ForwardTheLast();
		if (Snake.direction == 1) {
			
			if (Snake.size > 1) {
				Snake.x[0] = Snake.x[1] - 1;
				Snake.y[0] = Snake.y[1];
			}
			else {
				Snake.x[0]--;
			}
		}
		else if (Snake.direction == 2) {
			
			if (Snake.size > 1) {
				Snake.x[0] = Snake.x[1];
				Snake.y[0] = Snake.y[1] - 1;
			}
			else {
				Snake.y[0]--;
			}
		}
		else if (Snake.direction == 3) {
			
			if (Snake.size > 1) {
				Snake.x[0] = Snake.x[1] + 1;
				Snake.y[0] = Snake.y[1];
			}
			else {
				Snake.x[0]++;
			}
		}
		else if (Snake.direction == 4) {
			
			if (Snake.size > 1) {
				Snake.x[0] = Snake.x[1];
				Snake.y[0] = Snake.y[1] + 1;
			}
			else {
				Snake.y[0]++;
			}
		}
	
		
	}
	ChangeDirection();
	Sleep(100);
	ChangeDirection();
	return gameover(rows, columns);
}


bool play(int rows,int columns) {
	system("CLS");
	for (int i = 0; i < columns; i++) {
		cout << "!";
	}


	for (int j = 0; j < rows - 2; j++) {


		cout << "|";
		for (int i = 0; i < columns - 2; i++) {

			if (SearchArray(i,j)) {
				cout << "S";
			}
			else if (Apple.x == j && Apple.y == i) {
				cout << "A";
			}
			else {
				cout << " ";
			}
		}
		cout << "|";
	}

	for (int i = 0; i < columns; i++) {
		cout << "!";
	}

	ChangeDirection();

	return move(rows,columns);
	

	
}


int main(int argc, char *argv[]) {
	srand(time(NULL));
	//Check console size
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	//End Check

	Snake.x = (int*) calloc(1,sizeof(int));
	Snake.y = (int*)calloc(1,sizeof(int));
	Snake.x[0] = (rand() % (rows-2)) + 1;
	Snake.y[0] = (rand() % (columns-2)) + 1;
	GenerateApple(rows,columns);
	do {
		ChangeDirection();
		

	} while (play(rows, columns));

}