#include "serialVersion.h"
#include "ConsoleIO.h"

#include <iostream>
#include "stdlib.h"
#include "time.h"
#include <ctime>
#include <Windows.h>
#include <array>
#include <math.h>

using namespace std;

#define maxLoops 1						//maximum run loops
#define maxIterations 11000				//generation
#define fishMaxAge 10					//max age for fish
#define sharkMaxAge -20					//max age for shark

#define fishBreedingAge 2				//age of fish breeding
#define sharkBreedingAge -3				//age of sharks breeding


void runSerialVersion(int xHeight, int yHeight)
{
	cout << "Running SERIAL version with X=" << xHeight << ",Y=" << yHeight << endl;
	int** bufferWorld;					//buffer world array
	int** oldWorld;						//current wolrd array
	int** newWorld;						//new world array

	//memory allocation
	bufferWorld = new int*[xHeight];
	oldWorld = new int*[xHeight];
	newWorld = new int*[xHeight];

	for (int x = 0; x <xHeight; x++)
	{
		bufferWorld[x] = new int[yHeight];
		oldWorld[x] = new int[yHeight];
		newWorld[x] = new int[yHeight];
	}

	clock_t start, end;								//clocks for run time

	double diff = 0;								//timer difference
	int run = 0;									//run count

	int count = 0;									//generation count

	int fishBreeding=0;								//neighbourhood fish in breeding age
	int sharkBreeding=0;							//neighbourhood shark in breeding age
	int fishCount=0;								//neighbourhood fish count
	int sharkCount=0;								//neighbourhood shark count


	srand(10);										//init random with seed

	for (int x = 0; x < xHeight; x++)				//run x length
	{
		for (int y = 0; y < yHeight; y++)			//run y length
		{
			int random = rand() % 4;				//get random number

			if (random == 0)						//if 0 set to empty cell
			{
				oldWorld[x][y] = 0;
				newWorld[x][y] = 0;
				bufferWorld[x][y] = 0;
			}
			else if (random == 1)					//if 1 set to shark age 1
			{
				oldWorld[x][y] = -1;
				newWorld[x][y] = -1;
				bufferWorld[x][y] = -1;
			}
			else									//if other set to fish age 1
			{
				oldWorld[x][y] = 1;
				newWorld[x][y] = 1;
				bufferWorld[x][y] = 1;
			}
		}
	}

	//printArray(oldWorld, xHeight, yHeight, false);
	//Sleep(10000);

	while (run < maxLoops)							//used to average time
	{
		count = 0;									//set generation to 0
		start = clock();							//start the clock

		while (count < maxIterations)				//run for x generations
		{
			for (int x = 0; x < xHeight; x++)		//run x side
			{
				for (int y = 0; y < yHeight; y++)	//run y side
				{
					fishCount = 0;					//reset fish count
					fishBreeding = 0;				//reset fish in breeding age	

					sharkCount = 0;					//reset shark count
					sharkBreeding = 0;				//reset sharks in breeding age

					int xp = x + 1;					//cells to the right
					int xm = x - 1;					//cells to the left
					int yp = y + 1;					//cells below
					int ym = y - 1;					//cells above

#pragma region torus wrap
					if (x == xHeight - 1)
						xp = 0;
					else if (x == 0)
						xm = xHeight - 1;

					if (y == yHeight - 1)
						yp = 0;
					else if (y == 0)
						ym = yHeight - 1;
#pragma endregion

#pragma region top left neighbour
					if (oldWorld[xm][ym] > 0)
					{
						fishCount++;
						if (oldWorld[xm][ym] >= fishBreedingAge)
						{
							fishBreeding++;
						}
					}
					else if (oldWorld[xm][ym] < 0)
					{
						sharkCount++;
						if (oldWorld[xm][ym] <= sharkBreedingAge)
						{
							sharkBreeding++;
						}
					}
#pragma endregion									
#pragma region left mid neighbour
					if (oldWorld[xm][y] > 0)
					{
						fishCount++;
						if (oldWorld[xm][y] >= fishBreedingAge)
						{
							fishBreeding++;
						}
					}
					else if (oldWorld[xm][y] < 0)
					{
						sharkCount++;
						if (oldWorld[xm][y] <= sharkBreedingAge)
						{
							sharkBreeding++;
						}
					}
#pragma endregion
#pragma region left bottom neighbour
					if (oldWorld[xm][yp] > 0)
					{
						fishCount++;
						if (oldWorld[xm][yp] >= fishBreedingAge)
						{
							fishBreeding++;
						}
					}
					else if (oldWorld[xm][yp] < 0)
					{
						sharkCount++;
						if (oldWorld[xm][yp] <= sharkBreedingAge)
						{
							sharkBreeding++;
						}
					}
#pragma endregion
#pragma region mid top neighbour
					if (oldWorld[x][ym] > 0)
					{
						fishCount++;
						if (oldWorld[x][ym] >= fishBreedingAge)
						{
							fishBreeding++;
						}
					}
					else if (oldWorld[x][ym] < 0)
					{
						sharkCount++;
						if (oldWorld[x][ym] <= sharkBreedingAge)
						{
							sharkBreeding++;
						}
					}
#pragma endregion
#pragma region mid bottom neighbour
					if (oldWorld[x][yp] > 0)
					{
						fishCount++;
						if (oldWorld[x][yp] >= fishBreedingAge)
						{
							fishBreeding++;
						}
					}
					else if (oldWorld[x][yp] < 0)
					{
						sharkCount++;
						if (oldWorld[x][yp] <= sharkBreedingAge)
						{
							sharkBreeding++;
						}
					}
#pragma endregion
#pragma region top right neighbour
					if (oldWorld[xp][ym] > 0)
					{
						fishCount++;
						if (oldWorld[xp][ym] >= fishBreedingAge)
						{
							fishBreeding++;
						}
					}
					else if (oldWorld[xp][ym] < 0)
					{
						sharkCount++;
						if (oldWorld[xp][ym] <= sharkBreedingAge)
						{
							sharkBreeding++;
						}
					}
#pragma endregion
#pragma region right mid neighbour
					if (oldWorld[xp][y] > 0)
					{
						fishCount++;
						if (oldWorld[xp][y] >= fishBreedingAge)
						{
							fishBreeding++;
						}
					}
					else if (oldWorld[xp][y] < 0)
					{
						sharkCount++;
						if (oldWorld[xp][y] <= sharkBreedingAge)
						{
							sharkBreeding++;
						}
					}
#pragma endregion
#pragma region right bottom neighbour
					if (oldWorld[xp][yp] > 0)
					{
						fishCount++;
						if (oldWorld[xp][yp] >= fishBreedingAge)
						{
							fishBreeding++;
						}
					}
					else if (oldWorld[xp][yp] < 0)
					{
						sharkCount++;
						if (oldWorld[xp][yp] <= sharkBreedingAge)
						{
							sharkBreeding++;
						}
					}
#pragma endregion

					if (oldWorld[x][y] == 0)		//current cell is empty
					{
						if (fishCount >= 3 && fishBreeding>=3 && sharkCount < 4)
						{
							newWorld[x][y] = 1;
						}
						else if (sharkCount >= 3 && sharkBreeding >= 3 && fishCount < 4)
						{
							newWorld[x][y] = -1;
						}
						else
						{
							newWorld[x][y] = oldWorld[x][y];
						}
					}
					else if (oldWorld[x][y] > 0)	//current cell is fish
					{
						if (oldWorld[x][y] > fishMaxAge || sharkCount >= 5 || fishCount==8)
						{
							newWorld[x][y] = 0;
						}
						else
						{
							newWorld[x][y] = oldWorld[x][y] + 1;
						}
					}
					else if (oldWorld[x][y] < 0)	//current cell is shark
					{
						int randomDeath =rand()%48;

						if (oldWorld[x][y] < sharkMaxAge ||(sharkCount >= 6 && fishCount == 0)|| randomDeath == 0)
						{
							newWorld[x][y] = 0;
						}
						else
						{
							newWorld[x][y] = oldWorld[x][y] - 1;
						}
					}
					else							//error catch
					{
						newWorld[x][y] = 0;			//set default error value empty
					}
				}
			}

			//shift grids
			for (int x = 0; x < xHeight; x++)
			{
				for (int y = 0; y < yHeight; y++)
				{
					bufferWorld[x][y] = oldWorld[x][y];
					oldWorld[x][y] = newWorld[x][y];
				}
			}

			//print on screen
			//if (count == 100 || count == 1000 || count == 5000 || count == 10000)
			//{
			//	printArray(oldWorld, xHeight, yHeight, false);
			//	Sleep(10000);
			//}

			count++;		//increase generation count
		}
		
		end = clock();		//stop clock

		run++;				//increase run counter

		//printArray(oldWorld, xHeight, yHeight, false);
		//Sleep(10000);
		diff= ((float)end - (float)start);				//calculate runtime

		cout << endl << "Runtime: " << diff/60000 << endl;		//print runtime
	}
}
