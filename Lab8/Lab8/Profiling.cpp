#pragma once

#include "stdafx.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <iterator>
#include <Windows.h>
#include "Profiling.h"

using namespace std;

void Init(LARGE_INTEGER &start, __int64 &g_freq)
{
	QueryPerformanceFrequency(&start);
	g_freq = start.QuadPart;
}

__int64 GetTicks(LARGE_INTEGER &start)
{
	QueryPerformanceCounter(&start);
	return start.QuadPart;
}

double GetMicroTickCount(LARGE_INTEGER &start, __int64 const &g_freq)		// время в mcs
{
	return (GetTicks(start) * 1000000 / g_freq);
}

void AddIntoMap(map<string, double> &mapProfiling, string const &strName, double &time)
{
	if (mapProfiling.find(strName) == mapProfiling.end())
	{
		mapProfiling[strName] = time;
	}
}

void Result(map<string, double> &mapProfiling, string const &strName,
	int &countCall, __int64 const &g_freq, LARGE_INTEGER &start, double const &t)
{
	++countCall;
	double time = GetMicroTickCount(start, g_freq) - t;
	AddIntoMap(mapProfiling, strName, time);
}

void PrintResultOfProfiling(map<string, double> const &mapProfiling, int const &countCall)
{
	double sumTime = 0;

	for (auto elem : mapProfiling)
	{
		cout << elem.first << " : ";
		cout << elem.second << endl;

		sumTime += elem.second;
	}

	cout << "Sum time : " << sumTime << endl;
	cout << countCall << endl;
	cout << "Middle time :" << sumTime / countCall;
}

