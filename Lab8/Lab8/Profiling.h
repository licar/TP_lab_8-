#pragma once

#include "stdafx.h"
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <Windows.h>

using namespace std;

void Init(LARGE_INTEGER &start, __int64 &g_freq);
__int64 GetTicks(LARGE_INTEGER &start);
double GetMicroTickCount(LARGE_INTEGER &start, __int64 const &g_freq);

void AddIntoMap(map<string, double> &mapProfiling,
	string const &strName, double &time);

void Result(map<string, double> &mapProfiling, string const &strName,
	int &countCall, __int64 const &g_freq, LARGE_INTEGER &start, double const &t);

void PrintResultOfProfiling(map<string, double> const &mapProfiling, int const &countCall);