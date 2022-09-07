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

struct CubeFace {
	int idx;
	char* tl;
	char* tr;
	char* bl;
	char* br;
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
//		cube.cubeFace[i].tl = &cubeData[(i * FACEAREA) + 0];
		cube.cubeFace[i].tl = cubeData + (i * FACEAREA) + 0;
		cube.cubeFace[i].tr = &cubeData[(i * FACEAREA) + 1];
		cube.cubeFace[i].bl = &cubeData[(i * FACEAREA) + 2];
		cube.cubeFace[i].br = &cubeData[(i * FACEAREA) + 3];

		char ch[2];
		sprintf_s(ch, "%d", i);
		for (int j = 0; j < 4; j++) {
			cubeData[(i * FACEAREA) + j] = ch[0];
		}
	}
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

void gameLoop() {
	char ch;
	do {
		displayCube();
		//displayHelperCube();
		resetCursorLoc();
		ch = getchar();
		switch (ch) {
		case 'h':
			break;
		}
	} while (ch != 'q');
}

void main()
{
	initCursor();
	initCube();
	gameLoop();
}