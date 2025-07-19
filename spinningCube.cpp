#include <iostream>
#include <cmath>
#include <string.h>
#include <windows.h>

//The angles
float A = -50;
float B = 0;
float C = 0;

//The coords
float x, y, z;

//Cube properties
float incSpd = 0.15;
float cubeWidth = 8;
int scrW = 160;
int scrH = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int bgASCII = ' ';
float distance = 70;

float ooz; //one over z
int xp, yp;
int idx;
float K1 = 40;

float calculateX(int i, int j, int k)
{
	return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) 
		+ j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}
float calculateY(int i, int j, int k)
{
	return j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) * sin(B) * sin(C) + 
		k * cos(A) * sin(B) * sin(C) - i * cos(B) * sin(C);
}
float calculateZ(int i, int j, int k)
{
	return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

//Funzione per verificare se il punto attuale deve andare a essere un
//carattere vuoto oppure un carattere
void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch)
{
	float x = calculateX(cubeX, cubeY, cubeZ);
	float y = calculateY(cubeX, cubeY, cubeZ);
	float z = calculateZ(cubeX, cubeY, cubeZ) + distance;

	ooz = 1 / z;

	xp = (int)(scrW / 2 + K1 * ooz * x * 2);
	yp = (int)(scrH / 2 + K1 * ooz * y);

	idx = xp + yp * scrW;

	if (idx >= 0 && idx < scrW * scrH)
	{
		if (ooz > zBuffer[idx])
		{
			zBuffer[idx] = ooz;
			buffer[idx] = ch;
		}
	}
}

void controlledRotation()
{
	if (GetAsyncKeyState('W') & 0x8000) { A -= 0.05; Sleep(50); }
	if (GetAsyncKeyState('S') & 0x8000) { A += 0.05; Sleep(50); }
	if (GetAsyncKeyState('A') & 0x8000) { B -= 0.05; Sleep(50); }
	if (GetAsyncKeyState('D') & 0x8000) { B += 0.05; Sleep(50); }
}

void freeRotation(float& A, float& B, float C, float _aRotSpd = 0.05, float _bRotSpd = 0.05, float _cRotSpd = 0.05)
{
	A += _aRotSpd;
	B += _bRotSpd;
	C += _cRotSpd;
}

int main()
{
	printf("\x1b[2J");

	int h = 10;
	int maxW = 10;

	while (1)
	{
		memset(buffer, bgASCII, scrW * scrH);
		memset(zBuffer, 0, scrW * scrH * 4);

		//Draw the Sides of the cube
		for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incSpd)
		{
			for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incSpd)
			{
				//base
				calculateForSurface(cubeX + 50, cubeWidth, -cubeY, '*');

				//calculateForSurface(cubeWidth, -cubeX, -cubeY, '.');

				float x = cubeX;
				float y = cubeWidth;
				float z = -cubeY;
				float rt2 = sqrt(2);
				float rt6 = sqrt(6);

				calculateForSurface(x + 5, ((rt2 + rt6) * y + (rt6 - rt2) * z) / 4 - 2, ((rt6 + rt2) * z - (rt6 - rt2) * y) / 4 + 5, '@');

				calculateForSurface(cubeWidth + 50, cubeY, cubeX, '$');
				calculateForSurface(-cubeX + 50, cubeY, -cubeWidth, '-');
				calculateForSurface(-cubeWidth + 50, cubeY, cubeX, '.');
				calculateForSurface(cubeX + 50, cubeY, cubeWidth, '#');
				calculateForSurface(cubeX + 50, cubeWidth, -cubeY, '*');
				calculateForSurface(cubeX + 50, -cubeWidth, cubeY, '^');
			}
		}
		printf("\x1b[H");


		for (float triangleX = -maxW; triangleX < maxW; triangleX++)
		{
			for (float triangleH = -maxW; triangleH <= 2 * triangleX - 1; triangleH++)
			{
				float x = triangleX;
				float y = -triangleH;
				float z = -maxW;
				float root = sqrt(3);

				distance = 200;

				//Triangoli messi in alto----

				calculateForSurface(x, y, z + 100, '.'); //0 - 360
				calculateForSurface(-z, y, x + 100, '.'); //90
				calculateForSurface(-x, y, -z + 100, '.'); //180
				calculateForSurface(z, y, -x + 100, '.'); //270

				//----------------------------

				distance = 70;

				//Triangoli ruotati (piramide)----

				calculateForSurface(x, (root * y + z) / 2, (root * z - y) / 2 + 10, '^'); //0 - 360

				calculateForSurface(-z, y, x, '#');

				y = (root * y + z) / 2;
				z = (root * z - y) / 2;


				x = -x;
				z = -z;

				y = (root * y + z) / 2;
				z = (root * z - y) / 2;

				calculateForSurface((root * x - y) / 2 + 7, (x + root * y) / 2, z + 3, '.');

				//--------------------------------

			}
		}
		printf("\x1b[H");


		for (int k = 0; k < scrW * scrH; k++)
		{
			putchar(k % scrW ? buffer[k] : 10);
		}
		freeRotation(A, B, C, 0, 0.2, 0);
	}
	return 0;
}