// rubik.cpp

#include <stdio.h>
#include <Windows.h>
#include <cstdlib>
#include <ctime>

#define FACES 6
#define CUBELENGTH 2
#define CUBEWIDTH 2
#define FACEAREA (CUBELENGTH * CUBEWIDTH)
#define CUBESZ (CUBELENGTH * CUBEWIDTH * FACES)

#define ATTACHMENTPOINT 1	//where the sides of the cube are attached on the 2d display

char cubeData[CUBESZ];

HANDLE hCursor;

const COORD promptLoc = { 0, 8 + 2 };

void initCursor() {
	hCursor = GetStdHandle(STD_OUTPUT_HANDLE);
}

void writeCharAt(const char* text, COORD coord) {
	SetConsoleCursorPosition(hCursor, coord);
	WriteConsoleA(hCursor, text, 1, 0, 0);
}

struct Adjacent {
	char* upL;
	char* downL;
	char* leftU;
	char* rightU;
	char* upR;
	char* downR;
	char* leftD;
	char* rightD;
};

struct CubeFace {
	int idx;
	char* upL;
	char* upR;
	char* downL;
	char* downR;
	struct Adjacent adjacent;
	COORD upLpos;
	COORD upRpos;
	COORD downLpos;
	COORD downRpos;
};

struct Cube {
	CubeFace cubeFace[6];
};

Cube cube;

void initCube() {
	for (int i = 0; i < 6 * 4; i++) {
		cubeData[i] = 'x';
	}
	for (int i = 0; i < FACES; i++) {
		cube.cubeFace[i].idx = i;
		cube.cubeFace[i].upL = cubeData + (i * FACEAREA) + 0;
		cube.cubeFace[i].upR = cubeData + (i * FACEAREA) + 1;
		cube.cubeFace[i].downL = cubeData + (i * FACEAREA) + 2;
		cube.cubeFace[i].downR = cubeData + (i * FACEAREA) + 3;

		char ch[2];
		sprintf_s(ch, "%d", i);
		for (int j = 0; j < 4; j++) {
			cubeData[(i * FACEAREA) + j] = ch[0];
		}
	}

	cube.cubeFace[0].adjacent.upL = cube.cubeFace[3].downL;
	cube.cubeFace[0].adjacent.upR = cube.cubeFace[3].downR;
	cube.cubeFace[0].adjacent.rightU = cube.cubeFace[5].upR;
	cube.cubeFace[0].adjacent.rightD = cube.cubeFace[5].upL;
	cube.cubeFace[0].adjacent.downR = cube.cubeFace[1].upR;
	cube.cubeFace[0].adjacent.downL = cube.cubeFace[1].upL;
	cube.cubeFace[0].adjacent.leftD = cube.cubeFace[4].upR;
	cube.cubeFace[0].adjacent.leftU = cube.cubeFace[4].upL;
	cube.cubeFace[1].adjacent.upL = cube.cubeFace[0].downL;
	cube.cubeFace[1].adjacent.upR = cube.cubeFace[0].downR;
	cube.cubeFace[1].adjacent.rightU = cube.cubeFace[5].upL;
	cube.cubeFace[1].adjacent.rightD = cube.cubeFace[5].downL;
	cube.cubeFace[1].adjacent.downR = cube.cubeFace[2].upR;
	cube.cubeFace[1].adjacent.downL = cube.cubeFace[2].upL;
	cube.cubeFace[1].adjacent.leftD = cube.cubeFace[4].downR;
	cube.cubeFace[1].adjacent.leftU = cube.cubeFace[4].upR;
	cube.cubeFace[2].adjacent.upL = cube.cubeFace[1].downL;
	cube.cubeFace[2].adjacent.upR = cube.cubeFace[1].downR;
	cube.cubeFace[2].adjacent.rightU = cube.cubeFace[5].downL;
	cube.cubeFace[2].adjacent.rightD = cube.cubeFace[5].downR;
	cube.cubeFace[2].adjacent.downR = cube.cubeFace[3].upR;
	cube.cubeFace[2].adjacent.downL = cube.cubeFace[3].upL;
	cube.cubeFace[2].adjacent.leftD = cube.cubeFace[4].downL;
	cube.cubeFace[2].adjacent.leftU = cube.cubeFace[4].downR;
	cube.cubeFace[3].adjacent.upL = cube.cubeFace[2].downL;
	cube.cubeFace[3].adjacent.upR = cube.cubeFace[2].downR;
	cube.cubeFace[3].adjacent.rightU = cube.cubeFace[5].downR;
	cube.cubeFace[3].adjacent.rightD = cube.cubeFace[5].upR;
	cube.cubeFace[3].adjacent.downR = cube.cubeFace[0].upR;
	cube.cubeFace[3].adjacent.downL = cube.cubeFace[0].upL;
	cube.cubeFace[3].adjacent.leftD = cube.cubeFace[4].upL;
	cube.cubeFace[3].adjacent.leftU = cube.cubeFace[4].downL;
	cube.cubeFace[4].adjacent.upL = cube.cubeFace[0].upL;
	cube.cubeFace[4].adjacent.upR = cube.cubeFace[0].downL;
	cube.cubeFace[4].adjacent.rightU = cube.cubeFace[1].upL;
	cube.cubeFace[4].adjacent.rightD = cube.cubeFace[1].downL;
	cube.cubeFace[4].adjacent.downR = cube.cubeFace[2].upL;
	cube.cubeFace[4].adjacent.downL = cube.cubeFace[2].downL;
	cube.cubeFace[4].adjacent.leftD = cube.cubeFace[3].upL;
	cube.cubeFace[4].adjacent.leftU = cube.cubeFace[3].downL;
	cube.cubeFace[5].adjacent.upL = cube.cubeFace[0].downR;
	cube.cubeFace[5].adjacent.upR = cube.cubeFace[0].upR;
	cube.cubeFace[5].adjacent.rightU = cube.cubeFace[3].downR;
	cube.cubeFace[5].adjacent.rightD = cube.cubeFace[3].upR;
	cube.cubeFace[5].adjacent.downR = cube.cubeFace[2].downR;
	cube.cubeFace[5].adjacent.downL = cube.cubeFace[2].upR;
	cube.cubeFace[5].adjacent.leftD = cube.cubeFace[1].downR;
	cube.cubeFace[5].adjacent.leftU = cube.cubeFace[1].upR;

	int basepos = 0;
	for (int i = 0; i < FACES; i++) {
		if (i < 4) {
			cube.cubeFace[i].upLpos.X = 2;
			cube.cubeFace[i].upRpos.X = 3;
			cube.cubeFace[i].downLpos.X = 2;
			cube.cubeFace[i].downRpos.X = 3;
			cube.cubeFace[i].upLpos.Y = (i * 2);
			cube.cubeFace[i].upRpos.Y = (i * 2);
			cube.cubeFace[i].downLpos.Y = (i * 2) + 1;
			cube.cubeFace[i].downRpos.Y = (i * 2) + 1;
		}
		if (i == 4) {
			cube.cubeFace[i].upLpos.X = 0;
			cube.cubeFace[i].upRpos.X = 1;
			cube.cubeFace[i].downLpos.X = 0;
			cube.cubeFace[i].downRpos.X = 1;
			cube.cubeFace[i].upLpos.Y = (ATTACHMENTPOINT * 2);
			cube.cubeFace[i].upRpos.Y = (ATTACHMENTPOINT * 2);
			cube.cubeFace[i].downLpos.Y = (ATTACHMENTPOINT * 2) + 1;
			cube.cubeFace[i].downRpos.Y = (ATTACHMENTPOINT * 2) + 1;
		}
		if (i == 5) {
			cube.cubeFace[i].upLpos.X = 4;
			cube.cubeFace[i].upRpos.X = 5;
			cube.cubeFace[i].downLpos.X = 4;
			cube.cubeFace[i].downRpos.X = 5;
			cube.cubeFace[i].upLpos.Y = (ATTACHMENTPOINT * 2);
			cube.cubeFace[i].upRpos.Y = (ATTACHMENTPOINT * 2);
			cube.cubeFace[i].downLpos.Y = (ATTACHMENTPOINT * 2) + 1;
			cube.cubeFace[i].downRpos.Y = (ATTACHMENTPOINT * 2) + 1;
		}
	}
}

const char* getCellValue(int x) {
	char result[2];
	sprintf_s(result, "%d", x);
	return result;
}

void displayFace(int face) {
	writeCharAt(cube.cubeFace[face].upL, cube.cubeFace[face].upLpos);
	writeCharAt(cube.cubeFace[face].upR, cube.cubeFace[face].upRpos);
	writeCharAt(cube.cubeFace[face].downL, cube.cubeFace[face].downLpos);
	writeCharAt(cube.cubeFace[face].downR, cube.cubeFace[face].downRpos);
}

void displayCube() {
	for (int face = 0; face < FACES; face++) {
		displayFace(face);
	}
}

void displayFaceHelper(int face) {
	char ch[2];
	sprintf_s(ch, "%d", face);
	writeCharAt(ch, cube.cubeFace[face].upLpos);
	writeCharAt(ch, cube.cubeFace[face].upRpos);
	writeCharAt(ch, cube.cubeFace[face].downLpos);
	writeCharAt(ch, cube.cubeFace[face].downRpos);
}

void displayHelperCube() {
	for (int face = 0; face < FACES; face++) {
		displayFaceHelper(face);
	}
}

void resetCursorLoc() {
	SetConsoleCursorPosition(hCursor, promptLoc);
}

enum class State : int {
	none,
	left,
	right
};

void rotate(int face, State state) {
	char tmp, tmp1, tmp2;
	tmp = *cube.cubeFace[face].upL;
	tmp1 = *cube.cubeFace[face].adjacent.upL;
	tmp2 = *cube.cubeFace[face].adjacent.upR;
	if (state == State::right) {
		*cube.cubeFace[face].adjacent.upL = *cube.cubeFace[face].adjacent.leftD;
		*cube.cubeFace[face].adjacent.upR = *cube.cubeFace[face].adjacent.leftU;
		*cube.cubeFace[face].adjacent.leftD = *cube.cubeFace[face].adjacent.downR;
		*cube.cubeFace[face].adjacent.leftU = *cube.cubeFace[face].adjacent.downL;
		*cube.cubeFace[face].adjacent.downL = *cube.cubeFace[face].adjacent.rightD;
		*cube.cubeFace[face].adjacent.downR = *cube.cubeFace[face].adjacent.rightU;
		*cube.cubeFace[face].adjacent.rightD = tmp2;
		*cube.cubeFace[face].adjacent.rightU = tmp1;

		*cube.cubeFace[face].upL = *cube.cubeFace[face].downL;
		*cube.cubeFace[face].downL = *cube.cubeFace[face].downR;
		*cube.cubeFace[face].downR = *cube.cubeFace[face].upR;
		*cube.cubeFace[face].upR = tmp;
	}
	else if (state == State::left) {
		*cube.cubeFace[face].adjacent.upL = *cube.cubeFace[face].adjacent.rightU;
		*cube.cubeFace[face].adjacent.upR = *cube.cubeFace[face].adjacent.rightD;
		*cube.cubeFace[face].adjacent.rightD = *cube.cubeFace[face].adjacent.downL;
		*cube.cubeFace[face].adjacent.rightU = *cube.cubeFace[face].adjacent.downR;
		*cube.cubeFace[face].adjacent.downL = *cube.cubeFace[face].adjacent.leftU;
		*cube.cubeFace[face].adjacent.downR = *cube.cubeFace[face].adjacent.leftD;
		*cube.cubeFace[face].adjacent.leftD = tmp1;
		*cube.cubeFace[face].adjacent.leftU = tmp2;

		*cube.cubeFace[face].upL = *cube.cubeFace[face].upR;
		*cube.cubeFace[face].upR = *cube.cubeFace[face].downR;
		*cube.cubeFace[face].downR = *cube.cubeFace[face].downL;
		*cube.cubeFace[face].downL = tmp;
	}
}

#define WINDOWS 1

void clrscr() {
	system("cls");
}

void shuffle(int n) {
	State state;
	int tmp;
	for (int i = 0; i < n; i++) {
		tmp = (rand() % 2);
		switch (tmp) {
		case 0:
			state = State::left;
			break;
		case 1:
			state = State::right;
			break;
		}
		tmp = (rand() % 6);
		rotate(tmp, state);
	}
}

void gameLoop() {
	int face = 0;
	State state = State::none;
	char ch;
	char statusline[256] = { 0 };
	do {
		displayCube();
		//displayHelperCube();
		resetCursorLoc();
		printf("%s\r\n", statusline);
		ch = getchar();
		switch (ch) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
			face = ch - '0';
			if (state == State::right) {
				sprintf_s(statusline, "rotating face %d right\r\n", face);
			}
			else if (state == State::left) {
				sprintf_s(statusline, "rotating face %d left\r\n", face);
			}
			rotate(face, state);
			break;
		case 'l':
			state = State::left;
			strcpy_s(statusline, "rotating left\r\n");
			break;
		case 'r':
			state = State::right;
			strcpy_s(statusline, "rotating right\r\n");
			break;
		case 'R':
			initCube();
			strcpy_s(statusline, "reset cube\r\n");
			break;
		case 's':
			shuffle(1);
			strcpy_s(statusline, "shuffling cube\r\n");
			break;
		}

		clrscr();
	} while (ch != 'q');
}

void main()
{
	srand(time(0));
	initCursor();
	initCube();
	gameLoop();
}