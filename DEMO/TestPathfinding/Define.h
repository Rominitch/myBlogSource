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

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///                             BOOST
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#pragma warning(push) 
#pragma warning(disable:4456)
#pragma warning(disable:4458)
#pragma warning(disable:4244)
#pragma warning(disable:4267) // perte possible de données
#pragma warning(disable:4459)

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/two_bit_color_map.hpp>
#include <boost/graph/undirected_graph.hpp>

#include <boost/property_map/property_map.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/arithmetic/dot_product.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/random.hpp>

#pragma warning(pop)

namespace bg  = boost::geometry;
namespace bgi = bg::index;

using BoostPoint    = boost::geometry::model::point<float, 2, bg::cs::cartesian>;
using BoostBox      = boost::geometry::model::box<BoostPoint>;
using BoostPolygon  = boost::geometry::model::polygon<BoostPoint, false, false>;   ///< None-closed CounterClock polygon

// GRRRRRrrrr !
#undef min
#undef max

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///                         My Define
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

namespace BT
{
    using String     = std::string;
    using StringUTF8 = std::string;

    using uint8     = unsigned char;    ///< Integer of 8 bits
    using int8      = char;             ///< Signed Integer of 8 bits
    using uint16    = unsigned short;   ///< Integer of 16 bits
    using int16     = short;            ///< Signed Integer of 16 bits
    using uint32    = unsigned int;     ///< Integer of 32 bits
    using int32     = int;              ///< Signed  Integer of 32 bits
    using uint64    = unsigned __int64; ///< Integer of 64 bits
    using int64     = __int64;          ///< Signed Integer of 64 bits

    /// OS definition
#ifdef _WIN32
    using StringOS = std::wstring;
    using StringStreamOS = std::wstringstream;

    /// Converter system
    using ConverterWUTF8 = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>;
    static ConverterWUTF8 g_converter;
#endif

    //------------------------------------------------------------------------
    /// \brief  Convert UTF8 string to OS string.
    /// 
    /// \param[in] data: string in UTF8.
    /// \returns OS string
    inline StringOS convertToOS(const StringUTF8& data)
    {
        return StringOS(g_converter.from_bytes(data).c_str());
    }

    //------------------------------------------------------------------------
    /// \brief  Convert OS string to UTF8 string.
    /// 
    /// \param[in] data: OS string.
    /// \returns UTF8 string
    inline StringUTF8 convertToU8(const StringOS& data)
    {
        return StringUTF8(g_converter.to_bytes(data).c_str());
    }
}

#ifndef NDEBUG
#   define BT_ASSERT_HEADER(condition, header)                                                                                  \
    if(!static_cast<bool>(condition))                                                                                           \
    {                                                                                                                           \
        BT::String message;                                                                                                     \
        message += BT::String(__FILE__) + BT::String(u8"(") + std::to_string(__LINE__)                                          \
                + BT::String(u8"): [ERROR] ")+ BT::String(#header) + BT::String(u8" - ")                                        \
                + BT::String(#condition) + BT::String(u8"\n");                                                                  \
        std::cerr << message;                                                                                                   \
        OutputDebugString(BT::convertToOS(message).c_str());                                                                    \
        assert(condition);                                                                                                      \
    }

#   define BT_PRE_CONDITION(condition)  BT_ASSERT_HEADER(condition, u8"Pre-condition assert")
#   define BT_POST_CONDITION(condition) BT_ASSERT_HEADER(condition, u8"Post-condition assert")

#   define BT_ASSERT(condition) BT_ASSERT_HEADER(condition, u8"Application assert")

#   define LAST_REFERENCED(iterable)                                                                                            \
    for(const auto& iter : iterable)                                                                                            \
    {                                                                                                                           \
        BT_ASSERT(iter.use_count() == 1);                                                                                       \
    }

#   define BT_ASSERT_EQ(ref, cmp)                                                                                               \
    if(ref != cmp)                                                                                                              \
    {                                                                                                                           \
        BT::String message;                                                                                                     \
        message += BT::String(__FILE__) + BT::String(u8"(") + std::to_string(__LINE__)                                          \
                + BT::String(u8"): [ERROR] Application assert - Compare:\n")                                                    \
                + BT::String(u8"[ERROR] ") + BT::String(#ref) + BT::String(u8" = ") + std::to_string(ref) + BT::String(u8"\n")  \
                + BT::String(u8"[ERROR] ") + BT::String(#cmp) + BT::String(u8" = ") + std::to_string(cmp) + BT::String(u8"\n"); \
        std::cerr << message;                                                                                                   \
        OutputDebugString(BT::convertToOS(message).c_str());                                                                    \
        assert(ref == cmp);                                                                                                     \
    }
#   define BT_PRINT_MESSAGE(message)                                                                                            \
    {                                                                                                                           \
        std::cerr << message;                                                                                                   \
        OutputDebugString(BT::convertToOS(message).c_str());                                                                    \
    }
#else
#   define BT_PRE_CONDITION(condition)  void(0)
#   define BT_POST_CONDITION(condition) void(0)
#   define BT_ASSERT(condition)         void(0)

#   define BT_ASSERT_EQ(ref, cmp)       void(0)
#   define LAST_REFERENCED(iterable)    void(0)
#endif

#define BT_UNUSED(variable) variable;

//Define operator for short
inline BT::int16 operator "" _i16 (unsigned long long int number)
{
    return static_cast<BT::int16>(number);
}

#define BT_NOMOVE(ClassName)                                \
           ClassName(ClassName&&) = delete;                 \
           ClassName& operator=(ClassName&&) = delete

#define BT_NOCOPY(ClassName)                                \
           ClassName(const ClassName&) = delete;            \
           ClassName& operator=(const ClassName&) = delete

#define BT_NOCOPY_NOMOVE(ClassName)                         \
           BT_NOCOPY(ClassName);                            \
           BT_NOMOVE(ClassName)


namespace BT
{
    namespace Maths
    {
        //------------------------------------------------------------------------
        /// \brief  Clamp value between range (inclusive).
        ///
        /// \param[in] dValue: value to clamp.
        /// \param[in] dMin: minimal value.
        /// \param[in] dMax: maximal value.
        /// \returns Clamped value.
        template<typename DataType>
        static DataType clamp(const DataType dValue, const DataType dMin, const DataType dMax)
        {
            BT_ASSERT(dMin < dMax);
            return dValue < dMin ? dMin : (dValue > dMax ? dMax : dValue);
        }
    }
}