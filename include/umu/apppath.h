#pragma once

#include <Windows.h>
#include "tstring.h"

namespace umu::apppath {

inline tstring GetProgramPath(HMODULE module_handle = nullptr) {
  tstring program_path;
  for (DWORD buffer_size = MAX_PATH;; buffer_size += MAX_PATH) {
    program_path.resize(buffer_size);
    DWORD size =
        ::GetModuleFileName(module_handle, program_path.data(), buffer_size);
    if (0 == size) {
      program_path.clear();
      return program_path;
    }

    if (size < buffer_size) {
      program_path.resize(size);
      return program_path;
    }

    if (ERROR_INSUFFICIENT_BUFFER != ::GetLastError()) {
      program_path.clear();
      return program_path;
    }
  }
}

// 返回的路径最后有带 '\'
inline tstring GetProgramDirectory(HMODULE module_handle = nullptr) {
  tstring program_path(GetProgramPath(module_handle));
  program_path.resize(program_path.rfind(_T('\\')) + 1);
  return program_path;
}

inline tstring GetProgramBaseName(HMODULE module_handle = nullptr) {
  tstring program_path(GetProgramPath(module_handle));
  return program_path.substr(program_path.rfind(_T('\\')) + 1);
}

inline tstring GetProductDirectory(HMODULE module_handle = nullptr) {
  tstring path(GetProgramDirectory(module_handle));
  // bin 区分大小写
  size_t pos = path.rfind(_T("\\bin\\"));
  if (std::string::npos == pos) {
    // Parent Directory
    pos = path.rfind(L'\\', path.size() - 2);
  }
  path.resize(pos + 1);
  return path;
}

}  // namespace umu::apppath
