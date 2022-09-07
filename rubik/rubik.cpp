// rubik.cpp

#include <stdio.h>
#include <Windows.h>

#define FACES 6
#define CUBELENGTH 2
#define CUBEWIDTH 2
#define CUBESZ (CUBELENGTH * CUBEWIDTH * FACES)

#define ATTACHMENTPOINT 1	//where the sides of the cube are attached on the 2d display

char cubeData[CUBESZ];

HANDLE hCursor;

void initCursor() {
	hCursor = GetStdHandle(STD_OUTPUT_HANDLE);
}

void writeCharAt(const char* text, COORD coord) {
	SetConsoleCursorPosition(hCursor, coord);
	WriteConsole(hCursor, text, 1, 0, 0);
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
	cubeData[0] = 'x';
	for (int i = 0; i < FACES; i++) {
		cube.cubeFace[i].idx = i;
		cube.cubeFace[i].tl = &cubeData[0];
		cube.cubeFace[i].tr = &cubeData[0];
		cube.cubeFace[i].bl = &cubeData[0];
		cube.cubeFace[i].br = &cubeData[0];
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

void main()
{
	initCursor();
	initCube();
	displayCube();
	getchar();
}