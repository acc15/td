#pragma once

// Copy-pasted and modified from boost/current_function.hpp
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__) || defined(__clang__)
# define TD_FUNCTION __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
# define TD_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
# define TD_FUNCTION __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
# define TD_FUNCTION __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
# define TD_FUNCTION __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
# define TD_FUNCTION __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
# define TD_FUNCTION __func__
#else
# define TD_FUNCTION "(unknown)"
#endif

#ifdef NDEBUG

    #define TD_ASSERT(cond, msg) ((void)0)

#else

    #include <iostream>
    #include <string>

    namespace td {
    inline void assert_check(bool cond, const char* assertion, const char* file, int line, const char* func, const std::string& msg) {
        if (cond) {
            return;
        }

        std::cerr << "Assertion failure (" << assertion << ") in " << func << " at " << file << "(" << line << ")";
        if (!msg.empty()) {
            std::cerr << ": " << msg;
        }
        std::cerr << std::endl;
        std::abort();
    }
    }

    #define TD_ASSERT(cond, msg) ::td::assert_check(cond, (#cond), __FILE__, __LINE__, TD_FUNCTION, (msg))

#endif

