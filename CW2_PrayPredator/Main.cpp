#include "serialVersion.h"
#include "ompVersion.h"
#include "mpiVersion.h"
#include "hybridVersion.h"

#include <iostream>
#include "stdlib.h"
#include "time.h"
#include <ctime>
#include<Windows.h>
#include <array>
#include <math.h>


#define worldXheight 500
#define worldYheight 500

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Invalid number of executing arguments for programm execution. \r\nPlease make sure input arguments are valid and try again." << endl << endl;
	}
	else
	{
		int selection = atoi(argv[1]);
		int xHeight;
		int yHeight;

		if (argc == 3)
		{
			xHeight = atoi(argv[2]);
			yHeight = worldYheight;
		}
		else if (argc > 3)
		{
			xHeight = atoi(argv[2]);
			yHeight = atoi(argv[3]);
		}
		else
		{
			xHeight = worldXheight;
			yHeight = worldYheight;
		}

		switch (selection)
		{
		case 0:				//run serial version
			runSerialVersion(xHeight, yHeight);
			break;
		case 1:				//run omp version
			runOMPVersion(xHeight, yHeight);
			break;
		case 2:				//run mpi version
			runMPIVersion(argc, argv, xHeight, yHeight);
			break;
		case 3:				//run hybrid version
			runhybridVersion(argc, argv, xHeight, yHeight);
			break;
		default:			//run default serial version
			cout << "Default running";
			runSerialVersion(xHeight, yHeight);
			break;
		}
	}

	return 0;
}