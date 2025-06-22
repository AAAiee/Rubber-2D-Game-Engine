#pragma once

/**
* Precompiled Header file that includes common header files used in 
* development.
*/

// C++ Standard Library
#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>
#include <algorithm>
#include <functional>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <map>
#include <set>
#include <numeric>
#include <fstream>
#include <cstdint>
#include <filesystem>

// Windows
#ifdef WINDOW_64_API
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
#endif // #ifdef WINDOW_64_API


#include "Rubber/Log/Log.h"
#include "Rubber/Debug/Profiler.h"
#include "Rubber/Core/Primitives.h"
#include "Rubber/Core/Assert.h"
#include "Rubber/Utility/Utility.h"
