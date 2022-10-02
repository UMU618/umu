#pragma once

#if _HAS_CXX17
#include <string_view>
#endif

#include <conio.h>
#include <tchar.h>

namespace umu {
namespace console {
#if _HAS_CXX17
inline int ColorWrite(HANDLE console_output,
                 WORD color,
                 std::basic_string_view<TCHAR> buffer) {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(console_output, &csbi)) {
    return -1;
  }

  ::SetConsoleTextAttribute(console_output, color);
  DWORD count;
  if (!WriteConsole(console_output, buffer.data(),
                    static_cast<DWORD>(buffer.size()), &count, nullptr)) {
    count = 0;
  }
  ::SetConsoleTextAttribute(console_output, csbi.wAttributes);
  return count;
}

inline int ColorPrint(WORD color, std::basic_string_view<TCHAR> buffer) {
  return ColorWrite(GetStdHandle(STD_OUTPUT_HANDLE), color, buffer);
}

inline int ColorError(WORD color, std::basic_string_view<TCHAR> buffer) {
  return ColorWrite(GetStdHandle(STD_ERROR_HANDLE), color, buffer);
}

inline int Write(HANDLE console_output, std::basic_string_view<TCHAR> buffer) {
  DWORD count;
  if (!WriteConsole(console_output, buffer.data(),
                    static_cast<DWORD>(buffer.size()), &count, nullptr)) {
    count = 0;
  }
  return count;
}

inline int Print(std::basic_string_view<TCHAR> buffer) {
  return Write(GetStdHandle(STD_OUTPUT_HANDLE), buffer);
}

inline int Error(std::basic_string_view<TCHAR> buffer) {
  return Write(GetStdHandle(STD_ERROR_HANDLE), buffer);
}
#else
inline int ColorWrite(HANDLE console_output, WORD color, LPCTSTR buffer) {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(console_output, &csbi)) {
    return -1;
  }

  SetConsoleTextAttribute(console_output, color);
  int count = _cputts(buffer);
  SetConsoleTextAttribute(console_output, csbi.wAttributes);
  return count;
}

inline int ColorPrint(WORD color, LPCTSTR buffer) {
  return ColorWrite(GetStdHandle(STD_OUTPUT_HANDLE), color, buffer);
}

inline int ColorError(WORD color, LPCTSTR buffer) {
  return ColorWrite(GetStdHandle(STD_ERROR_HANDLE), color, buffer);
}
#endif

inline int ColorWritef(HANDLE console_output, WORD color, LPCTSTR format, ...) {
  HANDLE std_out = ::GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(std_out, &csbi)) {
    return -1;
  }

  ::SetConsoleTextAttribute(std_out, color);
  va_list args;
  va_start(args, format);
  int count = _vtcprintf_s(format, args);
  ::SetConsoleTextAttribute(std_out, csbi.wAttributes);
  return count;
}

template <typename... Args>
inline int ColorPrintf(WORD color, LPCTSTR format, const Args&... rest) {
  return ColorWritef(GetStdHandle(STD_OUTPUT_HANDLE), color, format, rest...);
}

template <typename... Args>
inline int ColorErrorf(WORD color, LPCTSTR format, const Args&... rest) {
  return ColorWritef(GetStdHandle(STD_ERROR_HANDLE), color, format, rest...);
}

inline void Pause(LPCTSTR tip = nullptr) {
  _cputts(nullptr == tip ? _T("Press any key to continue...") : tip);
  _gettch();
}

inline void PressEscToContinue(LPCTSTR tip = nullptr) {
  _cputts(nullptr == tip ? _T("Press ESC to continue...") : tip);
  while (0x1B != _gettch())
    ;
}

#if _WIN32_WINNT >= _WIN32_WINNT_WINXP
inline bool IsRunAlone() {
  DWORD process_list[2];
  return GetConsoleProcessList(process_list, _countof(process_list)) == 1;
}
#endif
}  // end of namespace console
}  // end of namespace umu
