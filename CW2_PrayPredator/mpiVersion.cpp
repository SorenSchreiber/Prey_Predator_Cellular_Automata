#include "mpiVersion.h"
#include "ConsoleIO.h"

#include <iostream>
#include "stdlib.h"
#include "time.h"
#include <ctime>
#include <Windows.h>
#include <array>
#include <math.h>

#include <mpi.h>

using namespace std;

#define maxLoops 1						//maximum run loops
#define maxIterations 11000				//generation
#define fishMaxAge 10					//max age for fish
#define sharkMaxAge -20					//max age for shark

#define fishBreedingAge 2				//age of fish breeding
#define sharkBreedingAge -3				//age of sharks breeding

void runMPIVersion(int argc, char** argv, int xHeight, int yHeight)
{
	cout << "Running MPI version with X=" << xHeight << ",Y=" << yHeight << endl;

	bool arrayIsSquare = false;				//is grid square
	bool XBigSide = true;					//is X big side of grid

	int xseg=xHeight;						//grid x length
	int yseg=yHeight;						//grid y length
	int messageSize=yHeight;				//communication message size

	int** bufferWorld;						//buffer world array
	int** oldWorld;							//current wolrd array
	int** newWorld;							//new world array
	int* sendLeft;							//parse array
	int* sendRight;							//parse array
	int* receiveLeft;						//parse array
	int* receiveRight;						//parse array

	int ProcCount, rank;					//process count and rank

	MPI_Init(&argc, &argv);						// Initialise MPI

	MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);	// Get Comm Size
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);		// Get rank 
	MPI_Status status;

	//used for printing
	HWND console = GetConsoleWindow();			 
	HDC dc = GetDC(console);					

	//check for array orientation
	if (xHeight == yHeight)
	{
		arrayIsSquare = true;
	}
	else
	{
		arrayIsSquare = false;
		if (xHeight > yHeight)
		{
			XBigSide = true;
		}
		else
		{
			XBigSide = false;
		}
	}

	//calculate array partitions
	if (arrayIsSquare || XBigSide)
	{
		yseg = yHeight;
		messageSize = yHeight;

		if (xHeight % ProcCount == 0)
		{
			xseg = xHeight / ProcCount;
		}
		else
		{
			if (rank < xHeight % ProcCount)
			{
				xseg = (xHeight / ProcCount) + 1;
			}
			else
			{
				xseg = xHeight / ProcCount;
			}
		}
		xseg = xseg + 2;						//add ghost cells for communication
	}
	else
	{
		xseg = xHeight;
		messageSize = xHeight;

		if (yHeight % ProcCount == 0)
		{
			yseg = yHeight / ProcCount;
		}
		else
		{
			if (rank < yHeight % ProcCount)
			{
				yseg = (yHeight / ProcCount) + 1;
			}
			else
			{
				yseg = yHeight /ProcCount;
			}
		}
		yseg = yseg + 2;						//add ghost cells for communication
	}

	//memory allocation
	bufferWorld = new int*[xseg];
	oldWorld = new int*[xseg];
	newWorld = new int*[xseg];

	for (int x = 0; x < xseg; x++)
	{
		bufferWorld[x] = new int[yseg];
		oldWorld[x] = new int[yseg];
		newWorld[x] = new int[yseg];
	}

	clock_t start, end;							//clocks for run time

	double diff = 0;							//timer difference
	int run = 0;								//run count

	int count = 0;								//generation count

	int fishBreeding = 0;						//neighbourhood fish in breeding age
	int sharkBreeding = 0;						//neighbourhood shark in breeding age
	int fishCount = 0;							//neighbourhood fish count
	int sharkCount = 0;							//neighbourhood shark count

	int xState = (int)XBigSide;					//start position for for loops relative to array orientation
	int yState = (int)!XBigSide;				//start position for for loops relative to array orientation

	srand(rank);								//init random with seed

	for (int x = xState; x < xseg; x++)		//run x length
	{
		for (int y = yState; y < yseg; y++)	//run y length
		{
			int random = rand() % 4;			//get random number

			if (random == 0)					//if 0 set to empty cell
			{
				oldWorld[x][y] = 0;
				newWorld[x][y] = 0;
				bufferWorld[x][y] = 0;
			}
			else if (random == 1)				//if 1 set to shark age 1
			{
				oldWorld[x][y] = -1;
				newWorld[x][y] = -1;
				bufferWorld[x][y] = -1;
			}
			else								//if other set to fish age 1
			{
				oldWorld[x][y] = 1;
				newWorld[x][y] = 1;
				bufferWorld[x][y] = 1;
			}
		}
	}

	//printArrayMPI(oldWorld, rank, XBigSide, xseg, yseg, false,dc);

	while (run < maxLoops)							//used to average time
	{
		count = 0;									//set generation to 0
		start = clock();							//start the clock
		int destL, destR, srcL, srcR;			//id of destinations and sources

		if (ProcCount > 1)						//if multi process execution
		{
			//if first process
			if (rank == 0)
			{
				destL = ProcCount - 1;
				destR = rank + 1;

				srcL = ProcCount - 1;
				srcR = rank + 1;
			}
			//if last process
			else if (rank == ProcCount - 1)
			{
				destL = rank - 1;
				destR = 0;

				srcL = rank - 1;
				srcR = 0;
			}
			//any other process
			else
			{
				destL = rank - 1;
				destR = rank + 1;

				srcL = rank - 1;
				srcR = rank + 1;
			}
		}

		while (count < maxIterations)				//run for x generations
		{
			if (ProcCount > 1)						//if multi process execution
			{
				//if even rank
				if (rank  == 0)
				{
					//if X big side
					if (XBigSide)
					{
						MPI_Send(oldWorld[1], messageSize, MPI_INT, destL, 0, MPI_COMM_WORLD);
						MPI_Recv(oldWorld[xseg - 1], messageSize, MPI_INT, srcR, 0, MPI_COMM_WORLD, &status);
						MPI_Send(oldWorld[xseg - 2], messageSize, MPI_INT, destR, 1, MPI_COMM_WORLD);
						MPI_Recv(oldWorld[0], messageSize, MPI_INT, srcL, 1, MPI_COMM_WORLD, &status);
					}
					//if Y big side
					else
					{
						//allocate memroy for data parsing
						sendLeft = new int[xseg];
						sendRight = new int[xseg];
						receiveLeft = new int[xseg];
						receiveRight = new int[xseg];

						//parse data to be send
						for (int x = 0; x < xseg; x++)
						{
							sendLeft[x] = oldWorld[x][1];
							sendRight[x] = oldWorld[x][yseg - 2];
						}

						MPI_Send(sendLeft, messageSize, MPI_INT, destL, 0, MPI_COMM_WORLD);
						MPI_Recv(receiveRight, messageSize, MPI_INT, srcR, 0, MPI_COMM_WORLD, &status);
						MPI_Send(sendRight, messageSize, MPI_INT, destR, 1, MPI_COMM_WORLD);
						MPI_Recv(receiveLeft, messageSize, MPI_INT, srcL, 1, MPI_COMM_WORLD, &status);

						//parse received data into array
						for (int x = 0; x < xseg; x++)
						{
							oldWorld[x][0] = receiveLeft[x];
							oldWorld[x][yseg - 1] = receiveRight[x];
						}
					}
				}
				//if uneven rank
				else
				{
					//if X big side
					if (XBigSide)
					{
						MPI_Recv(oldWorld[xseg - 1], messageSize, MPI_INT, srcR, 0, MPI_COMM_WORLD, &status);
						MPI_Send(oldWorld[1], messageSize, MPI_INT, destL, 0, MPI_COMM_WORLD);
						MPI_Recv(oldWorld[0], messageSize, MPI_INT, srcL, 1, MPI_COMM_WORLD, &status);
						MPI_Send(oldWorld[xseg - 2], messageSize, MPI_INT, destR, 1, MPI_COMM_WORLD);
					}
					//if Y big side
					else
					{
						//allocate memroy for data parsing
						sendLeft = new int[xseg];
						sendRight = new int[xseg];
						receiveLeft = new int[xseg];
						receiveRight = new int[xseg];

						//parse data to be send
						for (int x = 0; x < xseg; x++)
						{
							sendLeft[x] = oldWorld[x][1];
							sendRight[x] = oldWorld[x][yseg - 2];
						}

						MPI_Recv(receiveRight, messageSize, MPI_INT, srcR, 0, MPI_COMM_WORLD, &status);
						MPI_Send(sendLeft, messageSize, MPI_INT, destL, 0, MPI_COMM_WORLD);
						MPI_Recv(receiveLeft, messageSize, MPI_INT, srcL, 1, MPI_COMM_WORLD, &status);
						MPI_Send(sendRight, messageSize, MPI_INT, destR, 1, MPI_COMM_WORLD);

						//parse received data into array
						for (int x = 0; x < xseg; x++)
						{
							oldWorld[x][0] = receiveLeft[x];
							oldWorld[x][yseg - 1] = receiveRight[x];
						}
					}
				}
			}

			for (int x = xState; x < xseg-xState; x++)		//run x side
			{
				for (int y = yState; y < yseg-yState; y++)	//run y side
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
					if (XBigSide)
					{
						if (y == yseg - 1)
							yp = 0;
						else if (y == 0)
							ym = yseg - 1;
					}
					else
					{
						if (x == xseg - 1)
							xp = 0;
						else if (x == 0)
							xm = xseg - 1;
					}
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
						if (fishCount >= 3 && fishBreeding >= 3 && sharkCount < 4)
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
						if (oldWorld[x][y] > fishMaxAge || sharkCount >= 5 || fishCount == 8)
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
						int randomDeath = rand() % 48;

						if (oldWorld[x][y] < sharkMaxAge || (sharkCount >= 6 && fishCount == 0) || randomDeath == 0)
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

			//grid shifting
			for (int x = xState; x < xseg - xState; x++)		//run x side
			{
				for (int y = yState; y < yseg - yState; y++)	//run y side
				{
					bufferWorld[x][y] = oldWorld[x][y];
					oldWorld[x][y] = newWorld[x][y];
				}
			}
		
			MPI_Barrier(MPI_COMM_WORLD);			//barrier

			//if (count == 100 || count == 1000 || count == 5000 || count == 10000)
			//{
			//	printArrayMPI(oldWorld, rank, XBigSide, xseg, yseg, false, dc);
			//}
			//MPI_Barrier(MPI_COMM_WORLD);
			count++;							//increase generation count
		}

		end = clock();							//stop timer

		run++;									//increase run counter

		/*printArrayMPI(oldWorld, rank, XBigSide, xseg, yseg, false, dc);*/

		diff = ((float)end - (float)start);		//calculate runtime

		std::cout << endl << "Runtime: " << diff/60000 << endl;	//print runtime
	}
	ReleaseDC(console, dc);					//relsease resources

	MPI_Barrier(MPI_COMM_WORLD);			//barrier
	std::cout << "FINILIZE"<<endl;			//print finallize
	MPI_Finalize(); /* Finalise */
}