#pragma once

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///                             CPP
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// C
#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <sstream>
#include <assert.h>
#include <memory>
#include <limits>
#include <locale>
//#include <algorithm>

// STD
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <array>

// STD C++ 11
#include <atomic>
#include <chrono>
#include <codecvt>
#include <functional>
#include <future>
#include <mutex>
#include <random>
#include <thread>

// Load filesystem of C++17
#include <filesystem>

// OS
#ifdef _WIN32
#   include <windows.h>
#   define FILE_SYSTEM std::experimental
#else
#   define CALLBACK
#   define FILE_SYSTEM std
#endif