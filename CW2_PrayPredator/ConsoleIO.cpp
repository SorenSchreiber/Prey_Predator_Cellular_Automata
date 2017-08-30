#include "ConsoleIO.h"


using namespace std;

void printArray(int** buffer, int heightX, int heightY, bool AscII)
{
	//printing resources
	HWND console = GetConsoleWindow();
	HDC dc = GetDC(console);

	if (AscII)	//if ASCII output
	{
		for (int x = 0; x < heightX; x++)
		{
			for (int y = 0; y < heightY; y++)
			{
				if (buffer[x][y] > 0)	//if fish
				{
					cout << "O";		//print O
				}
				else if (buffer[x][y] < 0)	//if shark
				{
					cout << "X";		//print X
				}
				else
				{
					cout << ".";		//print .
				}
				cout << " ";
			}
			cout << endl;
		}
		cout << "------------------------------------" << endl;
	}
	else //if pixel output
	{
		for (int x = 0; x < heightX; x++)
		{
			for (int y = 0; y < heightY; y++)
			{
				if (buffer[x][y] > 0)	//if fish
				{
					SetPixel(dc, x, y, RGB(0, 255, 0)); //set pixel to Green
				}
				else if (buffer[x][y] < 0)	//if shark
				{
					SetPixel(dc, x, y, RGB(255, 0, 0));//set pixel red
				}
				else
				{
					SetPixel(dc, x, y, RGB(0, 0, 255)); //set pixel blue
				}
			}
		}
	}

	ReleaseDC(console, dc);
}

void printArrayMPI(int** buffer, int rank, bool Xside,  int heightX, int heightY, bool AscII, HDC dc)
{

	int Xoffset;					//offset X side
	int Yoffset;					//offset Y side

	int xstart = (int)Xside;		//start point
	int ystart = (int)!Xside;		//start point

	//calculacte start point and offest
	if (Xside)
	{
		Xoffset = rank*(heightX-2);
		Yoffset = 0;
	}
	else
	{
		Xoffset = 0;
		Yoffset = rank*(heightY-2);
	}

	if (AscII)		//if ASCII 
	{
		if (Xside)
		{
			for (int x = xstart; x <heightX-xstart; x++)
			{
				for (int y = heightY - 1; y >= ystart; y--)
				{
					if (buffer[x][y] > 0)	//if fish
					{
						cout << "O";		//print O
					}
					else if (buffer[x][y] < 0)	//if shark
					{
						cout << "X";		//print X
					}
					else
					{
						cout << ".";		//print .
					}
					cout << " ";
				}
				std::cout << endl;
			}
		}
		else
		{
			for (int x = xstart; x < heightX-xstart; x++)
			{
				for (int y = ystart; y < heightY-ystart; y++)
				{
					if (buffer[x][y] > 0)	//if fish
					{
						cout << "O";		//print O
					}
					else if (buffer[x][y] < 0)	//if shark
					{
						cout << "X";		//print X
					}
					else
					{
						cout << ".";		//print .
					}
					cout << " ";
				}
				std::cout << endl;
			}
		}
	}
	else	//if pixel
	{
		for (int x = xstart; x < heightX-xstart; x++)
		{
			for (int y = ystart; y < heightY-ystart; y++)
			{
				if (buffer[x][y] > 0)		//if cell is fish
				{
					SetPixel(dc, x+Xoffset, y+Yoffset, RGB(0, 255, 0));	//set pixel green
				}
				else if (buffer[x][y] < 0)	//if cell is shark
				{
					SetPixel(dc, x + Xoffset, y + Yoffset, RGB(255, 0, 0)); //set pixel red
				}
				else
				{
					SetPixel(dc, x + Xoffset, y + Yoffset, RGB(0, 0, 255));	//set pixel blue
				}
			}
		}
	}
}

