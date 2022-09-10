// rubik.cpp

#include <stdio.h>
#include <Windows.h>

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
	char* tl;
	char* tr;
	char* bl;
	char* br;
	struct Adjacent adjacent;
	COORD tlpos;
	COORD trpos;
	COORD blpos;
	COORD brpos;
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
		cube.cubeFace[i].tl = cubeData + (i * FACEAREA) + 0;
		cube.cubeFace[i].tr = cubeData + (i * FACEAREA) + 1;
		cube.cubeFace[i].bl = cubeData + (i * FACEAREA) + 2;
		cube.cubeFace[i].br = cubeData + (i * FACEAREA) + 3;

		char ch[2];
		sprintf_s(ch, "%d", i);
		for (int j = 0; j < 4; j++) {
			cubeData[(i * FACEAREA) + j] = ch[0];
		}
	}

	cube.cubeFace[0].adjacent.upL = cube.cubeFace[3].bl;
	cube.cubeFace[0].adjacent.upR = cube.cubeFace[3].br;
	cube.cubeFace[0].adjacent.downL = cube.cubeFace[1].tl;
	cube.cubeFace[0].adjacent.downR = cube.cubeFace[1].tr;
	cube.cubeFace[0].adjacent.leftU = cube.cubeFace[4].tl;
	cube.cubeFace[0].adjacent.leftD = cube.cubeFace[4].tr;
	cube.cubeFace[0].adjacent.rightU = cube.cubeFace[5].tr;
	cube.cubeFace[0].adjacent.rightD = cube.cubeFace[5].tl;
	cube.cubeFace[1].adjacent.upL = cube.cubeFace[0].bl;
	cube.cubeFace[1].adjacent.upR = cube.cubeFace[0].br;
	cube.cubeFace[1].adjacent.downL = cube.cubeFace[2].tl;
	cube.cubeFace[1].adjacent.downR = cube.cubeFace[2].tr;
	cube.cubeFace[1].adjacent.leftU = cube.cubeFace[4].tr;
	cube.cubeFace[1].adjacent.leftD = cube.cubeFace[4].br;
	cube.cubeFace[1].adjacent.rightU = cube.cubeFace[5].tl;
	cube.cubeFace[1].adjacent.rightD = cube.cubeFace[5].bl;

	int basepos = 0;
	for (int i = 0; i < FACES; i++) {
		if (i < 4) {
			cube.cubeFace[i].tlpos.X = 2;
			cube.cubeFace[i].trpos.X = 3;
			cube.cubeFace[i].blpos.X = 2;
			cube.cubeFace[i].brpos.X = 3;
			cube.cubeFace[i].tlpos.Y = (i * 2);
			cube.cubeFace[i].trpos.Y = (i * 2);
			cube.cubeFace[i].blpos.Y = (i * 2) + 1;
			cube.cubeFace[i].brpos.Y = (i * 2) + 1;
		}
		if (i == 4) {
			cube.cubeFace[i].tlpos.X = 0;
			cube.cubeFace[i].trpos.X = 1;
			cube.cubeFace[i].blpos.X = 0;
			cube.cubeFace[i].brpos.X = 1;
			cube.cubeFace[i].tlpos.Y = (ATTACHMENTPOINT * 2);
			cube.cubeFace[i].trpos.Y = (ATTACHMENTPOINT * 2);
			cube.cubeFace[i].blpos.Y = (ATTACHMENTPOINT * 2) + 1;
			cube.cubeFace[i].brpos.Y = (ATTACHMENTPOINT * 2) + 1;
		}
		if (i == 5) {
			cube.cubeFace[i].tlpos.X = 4;
			cube.cubeFace[i].trpos.X = 5;
			cube.cubeFace[i].blpos.X = 4;
			cube.cubeFace[i].brpos.X = 5;
			cube.cubeFace[i].tlpos.Y = (ATTACHMENTPOINT * 2);
			cube.cubeFace[i].trpos.Y = (ATTACHMENTPOINT * 2);
			cube.cubeFace[i].blpos.Y = (ATTACHMENTPOINT * 2) + 1;
			cube.cubeFace[i].brpos.Y = (ATTACHMENTPOINT * 2) + 1;
		}
	}
}

const char* getCellValue(int x) {
	char result[2];
	sprintf_s(result, "%d", x);
	return result;
}

void displayFace(int face) {
	writeCharAt(cube.cubeFace[face].tl, cube.cubeFace[face].tlpos);
	writeCharAt(cube.cubeFace[face].tr, cube.cubeFace[face].trpos);
	writeCharAt(cube.cubeFace[face].bl, cube.cubeFace[face].blpos);
	writeCharAt(cube.cubeFace[face].br, cube.cubeFace[face].brpos);
}

void displayCube() {
	for (int face = 0; face < FACES; face++) {
		displayFace(face);
	}
}

void displayFaceHelper(int face) {
	char ch[2];
	sprintf_s(ch, "%d", face);
	writeCharAt(ch, cube.cubeFace[face].tlpos);
	writeCharAt(ch, cube.cubeFace[face].trpos);
	writeCharAt(ch, cube.cubeFace[face].blpos);
	writeCharAt(ch, cube.cubeFace[face].brpos);
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
	tmp = *cube.cubeFace[face].tl;
	tmp1 = *cube.cubeFace[face].adjacent.upL;
	tmp2 = *cube.cubeFace[face].adjacent.upR;
	if (state == State::right) {
		*cube.cubeFace[face].adjacent.upL = *cube.cubeFace[face].adjacent.leftD;
		*cube.cubeFace[face].adjacent.upR = *cube.cubeFace[face].adjacent.leftU;
		*cube.cubeFace[face].adjacent.leftD = *cube.cubeFace[face].adjacent.downR;
		*cube.cubeFace[face].adjacent.leftU = *cube.cubeFace[face].adjacent.downL;
		*cube.cubeFace[face].adjacent.downL = *cube.cubeFace[face].adjacent.rightD;
		*cube.cubeFace[face].adjacent.downR = *cube.cubeFace[face].adjacent.rightU;
		*cube.cubeFace[face].adjacent.rightD = tmp1;
		*cube.cubeFace[face].adjacent.rightU = tmp2;

		*cube.cubeFace[face].tl = *cube.cubeFace[face].bl;
		*cube.cubeFace[face].bl = *cube.cubeFace[face].br;
		*cube.cubeFace[face].br = *cube.cubeFace[face].tr;
		*cube.cubeFace[face].tr = tmp;
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

		*cube.cubeFace[face].tl = *cube.cubeFace[face].tr;
		*cube.cubeFace[face].tr = *cube.cubeFace[face].br;
		*cube.cubeFace[face].br = *cube.cubeFace[face].bl;
		*cube.cubeFace[face].bl = tmp;
	}
}

#define WINDOWS 1

void clrscr() {
	system("cls");
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
			} else if (state == State::left) {
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
		}
		clrscr();
	} while (ch != 'q');
}

void main()
{
	initCursor();
	initCube();
	gameLoop();
}