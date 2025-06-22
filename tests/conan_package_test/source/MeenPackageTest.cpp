/*
Copyright (c) 2021-2025 Nicolas Beddows <nicolas.beddows@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <iostream>

#include "meen/MachineFactory.h"

void printEnvironment(const char* version)
{
#ifdef NDEBUG
    std::cout << "meen/" << version <<": Machine Emulator Engine Release!\n";
#else
    std::cout << "meen/" << version <<": Machine Emulator Engine Debug!\n";
#endif

// ARCHITECTURES
#ifdef _M_X64
    std::cout << "  meen/" << version <<": _M_X64 defined\n";
#endif

#ifdef _M_IX86
    std::cout << "  meen/" << version <<": _M_IX86 defined\n";
#endif

#ifdef _M_ARM64
    std::cout << "  meen/" << version <<": _M_ARM64 defined\n";
#endif

#if __i386__
    std::cout << "  meen/" << version <<": __i386__ defined\n";
#endif

#if __x86_64__
    std::cout << "  meen/" << version <<": __x86_64__ defined\n";
#endif

#if __aarch64__
    std::cout << "  meen/" << version <<": __aarch64__ defined\n";
#endif

// Libstdc++
#if defined _GLIBCXX_USE_CXX11_ABI
    std::cout << "  meen/" << version <<": _GLIBCXX_USE_CXX11_ABI "<< _GLIBCXX_USE_CXX11_ABI << "\n";
#endif

// MSVC runtime
#if defined(_DEBUG)
    #if defined(_MT) && defined(_DLL)
        std::cout << "  meen/" << version <<": MSVC runtime: MultiThreadedDebugDLL\n";
    #elif defined(_MT)
        std::cout << "  meen/" << version <<": MSVC runtime: MultiThreadedDebug\n";
    #endif
#else
    #if defined(_MT) && defined(_DLL)
        std::cout << "  meen/" << version <<": MSVC runtime: MultiThreadedDLL\n";
    #elif defined(_MT)
        std::cout << "  meen/" << version <<": MSVC runtime: MultiThreaded\n";
    #endif
#endif

// COMPILER VERSIONS
#if _MSC_VER
    std::cout << "  meen/" << version <<": _MSC_VER" << _MSC_VER<< "\n";
#endif

#if _MSVC_LANG
    std::cout << "  meen/" << version <<": _MSVC_LANG" << _MSVC_LANG<< "\n";
#endif

#if __cplusplus
    std::cout << "  meen/" << version <<": __cplusplus" << __cplusplus<< "\n";
#endif

#if __INTEL_COMPILER
    std::cout << "  meen/" << version <<": __INTEL_COMPILER" << __INTEL_COMPILER<< "\n";
#endif

#if __GNUC__
    std::cout << "  meen/" << version <<": __GNUC__" << __GNUC__<< "\n";
#endif

#if __GNUC_MINOR__
    std::cout << "  meen/" << version <<": __GNUC_MINOR__" << __GNUC_MINOR__<< "\n";
#endif

#if __clang_major__
    std::cout << "  meen/" << version <<": __clang_major__" << __clang_major__<< "\n";
#endif

#if __clang_minor__
    std::cout << "  meen/" << version <<": __clang_minor__" << __clang_minor__<< "\n";
#endif

#if __apple_build_version__
    std::cout << "  meen/" << version <<": __apple_build_version__" << __apple_build_version__<< "\n";
#endif

// SUBSYSTEMS
#if __MSYS__
    std::cout << "  meen/" << version <<": __MSYS__" << __MSYS__<< "\n";
#endif

#if __MINGW32__
    std::cout << "  meen/" << version <<": __MINGW32__" << __MINGW32__<< "\n";
#endif

#if __MINGW64__
    std::cout << "  meen/" << version <<": __MINGW64__" << __MINGW64__<< "\n";
#endif

#if __CYGWIN__
    std::cout << "  meen/" << version <<": __CYGWIN__" << __CYGWIN__<< "\n";
#endif
}

// Simple program that tests that the meen package is configured correctly.
// It then prints out the meen version along with environment information.
int main()
{
    printEnvironment(meen::Version());
    return 0;
}
