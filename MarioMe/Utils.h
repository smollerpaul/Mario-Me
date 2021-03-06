#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <signal.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

#include <algorithm>
#include <assert.h>
#include <set>


using namespace std;

void DebugOut(wchar_t *fmt, ...);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);

