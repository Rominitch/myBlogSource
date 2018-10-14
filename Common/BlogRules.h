#pragma once

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///                         My Define
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

namespace RM
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
        RM::String message;                                                                                                     \
        message += RM::String(__FILE__) + RM::String(u8"(") + std::to_string(__LINE__)                                          \
                + RM::String(u8"): [ERROR] ")+ RM::String(#header) + RM::String(u8" - ")                                        \
                + RM::String(#condition) + RM::String(u8"\n");                                                                  \
        std::cerr << message;                                                                                                   \
        OutputDebugString(RM::convertToOS(message).c_str());                                                                    \
        assert(condition);                                                                                                      \
    }

#   define BT_PRE_CONDITION(condition)  BT_ASSERT_HEADER(condition, u8"Pre-condition assert")
#   define BT_POST_CONDITION(condition) BT_ASSERT_HEADER(condition, u8"Post-condition assert")

#   define RM_ASSERT(condition) BT_ASSERT_HEADER(condition, u8"Application assert")

#   define LAST_REFERENCED(iterable)                                                                                            \
    for(const auto& iter : iterable)                                                                                            \
    {                                                                                                                           \
        RM_ASSERT(iter.use_count() == 1);                                                                                       \
    }

#   define BT_ASSERT_EQ(ref, cmp)                                                                                               \
    if(ref != cmp)                                                                                                              \
    {                                                                                                                           \
        RM::String message;                                                                                                     \
        message += RM::String(__FILE__) + RM::String(u8"(") + std::to_string(__LINE__)                                          \
                + RM::String(u8"): [ERROR] Application assert - Compare:\n")                                                    \
                + RM::String(u8"[ERROR] ") + RM::String(#ref) + RM::String(u8" = ") + std::to_string(ref) + RM::String(u8"\n")  \
                + RM::String(u8"[ERROR] ") + RM::String(#cmp) + RM::String(u8" = ") + std::to_string(cmp) + RM::String(u8"\n"); \
        std::cerr << message;                                                                                                   \
        OutputDebugString(RM::convertToOS(message).c_str());                                                                    \
        assert(ref == cmp);                                                                                                     \
    }
#   define BT_PRINT_MESSAGE(message)                                                                                            \
    {                                                                                                                           \
        std::cerr << message;                                                                                                   \
        OutputDebugString(RM::convertToOS(message).c_str());                                                                    \
    }
#else
#   define RM_PRE_CONDITION(condition)  void(0)
#   define RM_POST_CONDITION(condition) void(0)
#   define RM_ASSERT(condition)         void(0)

#   define RM_ASSERT_EQ(ref, cmp)       void(0)
#   define LAST_REFERENCED(iterable)    void(0)
#endif

#define RM_UNUSED(variable) variable;

//Define operator for short
inline RM::int16 operator "" _i16 (unsigned long long int number)
{
    return static_cast<RM::int16>(number);
}

#define RM_NOMOVE(ClassName)                                \
           ClassName(ClassName&&) = delete;                 \
           ClassName& operator=(ClassName&&) = delete

#define RM_NOCOPY(ClassName)                                \
           ClassName(const ClassName&) = delete;            \
           ClassName& operator=(const ClassName&) = delete

#define RM_NOCOPY_NOMOVE(ClassName)                         \
           RM_NOCOPY(ClassName);                            \
           RM_NOMOVE(ClassName)


namespace RM
{
    namespace Maths
    {
        /// Default constant
        const float precision = 1e-6f;
        const float PI        = 3.14159265358979323846f;
        const float PI_4      = 0.785398163397448309616f;
        const float sqrt_2    = sqrt(2.0f);

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
            RM_ASSERT(dMin < dMax);
            return dValue < dMin ? dMin : (dValue > dMax ? dMax : dValue);
        }
    }
}