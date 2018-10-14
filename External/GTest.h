#pragma once

//Add include in project
#include <gtest/gtest.h>

#ifdef NDEBUG
// Mode for benchmark -> useless in debug mode
#   define PERFORMANCE
#endif

//Link with lib
#ifdef _WIN64
#   ifdef NDEBUG
#       pragma comment(lib, "gtest.lib")
#   else
#       pragma comment(lib, "gtestd.lib")
#   endif
#endif