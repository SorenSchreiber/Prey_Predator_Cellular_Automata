#pragma once
#include <Windows.h>
#include <iostream>
#include "stdlib.h"

void printArray(int** buffer, int heightX, int heightY, bool AscII);

void printArrayMPI(int** buffer, int rank, bool Xside, int heightX, int heightY, bool AscII, HDC dc);