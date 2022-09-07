// rubik.cpp

#include <stdio.h>
#include <Windows.h>

#define FACES 6
#define CUBELENGTH 2
#define CUBEWIDTH 2
#define CUBESZ (CUBELENGTH * CUBEWIDTH * FACES)
int cubeData[CUBESZ];

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
	int* tl;
	int* tr;
	int* bl;
	int* br;
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
	cubeData[0] = 1;
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
			cube.cubeFace[i].tlpos.Y = (3 * 2);
			cube.cubeFace[i].trpos.Y = (3 * 2);
			cube.cubeFace[i].blpos.Y = (3 * 2) + 1;
			cube.cubeFace[i].brpos.Y = (3 * 2) + 1;
		}
		if (i == 5) {
			cube.cubeFace[i].tlpos.X = 4;
			cube.cubeFace[i].trpos.X = 5;
			cube.cubeFace[i].blpos.X = 4;
			cube.cubeFace[i].brpos.X = 5;
			cube.cubeFace[i].tlpos.Y = (3 * 2);
			cube.cubeFace[i].trpos.Y = (3 * 2);
			cube.cubeFace[i].blpos.Y = (3 * 2) + 1;
			cube.cubeFace[i].brpos.Y = (3 * 2) + 1;
		}
	}
}

void displayCube() {
	for (int face = 0; face < FACES; face++) {
		writeCharAt("x", cube.cubeFace[face].tlpos);
		writeCharAt("x", cube.cubeFace[face].trpos);
		writeCharAt("x", cube.cubeFace[face].blpos);
		writeCharAt("x", cube.cubeFace[face].brpos);
		/*
		if (face < 3)
			printf("  ");
		if (face == 4)
			printf("  ");
		printf("%d", *cube.cubeFace[face].tl);
		printf("%d", *cube.cubeFace[face].tr);
		printf("\n");
		if (face != 3)
			printf("  ");
		if (face == 4)
			printf("  ");
		printf("%d", *cube.cubeFace[face].bl);
		printf("%d", *cube.cubeFace[face].br);
		if (face != 3 && face != 4)
			printf("\n");
			*/
	}
}

void main()
{
	initCursor();
	initCube();
	displayCube();
	getchar();
}