#pragma once
#include <string>
#ifdef _WIN32
#include <windows.h>
#include <vector>
#endif

namespace ConsoleUtf8 {
    inline void write(const std::string& s) {
#ifdef _WIN32
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        if (h == INVALID_HANDLE_VALUE || h == nullptr) {
            // fallback
            fwrite(s.c_str(), 1, s.size(), stdout);
            return;
        }
        // Convert UTF-8 to UTF-16
        int wlen = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), NULL, 0);
        if (wlen == 0) {
            fwrite(s.c_str(), 1, s.size(), stdout);
            return;
        }
        std::vector<wchar_t> wbuf(wlen);
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), wbuf.data(), wlen);
        DWORD written = 0;
        WriteConsoleW(h, wbuf.data(), wlen, &written, NULL);
#else
        fwrite(s.c_str(), 1, s.size(), stdout);
#endif
    }

    inline void writeLine(const std::string& s) {
        write(s);
        write("\n");
    }
}
