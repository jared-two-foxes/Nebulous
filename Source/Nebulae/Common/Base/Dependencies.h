#ifndef NEBULAE_COMMON_BASE_DEPENDENCIES_H_
#define NEBULAE_COMMON_BASE_DEPENDENCIES_H_

// Std header includes
#include <assert.h>
#include <cctype>
#include <cstdarg>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <ctime>
#include <source_location>

// IO Streams
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

// STL containers
#include <list>
#include <map>
#include <memory>
#include <deque>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>

// STL algorithms & functions
#include <algorithm>
#include <atomic>
#include <chrono> // cpp11
#include <condition_variable>
#include <functional>
#include <future>
#include <limits>
#include <mutex>
#include <random> // cpp11
#include <ranges>
#include <thread>
#include <utility>

// boost includes
#include <boost/any.hpp>
// #define BOOST_FILESYSTEM_NO_DEPRECATED //< Prevent use of depreciated functions
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

// json
#include <json/json.h>


#endif // NEBULAE_COMMON_BASE_DEPENDENCIES_H_
