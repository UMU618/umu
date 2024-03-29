#pragma once

#include <cassert>

#include "tstring.h"

namespace umu {
class SingletonApp {
 public:
  SingletonApp() = default;

  bool Initialize(tstring app_name) noexcept {
    assert(!app_name.empty());
    if (app_name.empty()) {
      return false;
    }
    app_name_ = std::move(app_name);
    return Reinitialize();
  }

  bool CheckWindowProp(HWND hwnd) {
    HANDLE handle = ::GetProp(hwnd, app_name_.data());
    if (nullptr != handle && handle == hwnd) {
      return true;
    }
    return false;
  }

  bool EnableSingleton(HWND hwnd, bool enable = true) noexcept {
    return !!::SetProp(hwnd, app_name_.data(), enable ? hwnd : nullptr);
  }

  void Uninitialize() noexcept {
    mutex_.Close();
  }

  HWND FindPrevInstanceWindow() noexcept {
    ::EnumWindows(EnumWindowsProc, (LPARAM)this);
    return prev_;
  }

  bool Reinitialize() noexcept {
    assert(!app_name_.empty());
    HANDLE h = CreateMutex(nullptr, TRUE, app_name_.data());
    if (nullptr == h) {
      return false;
    }
    mutex_.Attach(h);
    if (ERROR_ALREADY_EXISTS == GetLastError()) {
      return false;
    }
    return true;
  }

  void RemoveSingleton(HWND hwnd) noexcept {
    ::RemoveProp(hwnd, app_name_.data());
  }

 private:
  static BOOL CALLBACK EnumWindowsProc(__in HWND hwnd, __in LPARAM lParam) {
    if (nullptr != hwnd) {
      SingletonApp* this_ptr = (SingletonApp*)lParam;
      if (this_ptr->CheckWindowProp(hwnd)) {
        this_ptr->prev_ = hwnd;
        return FALSE;
      }
    }
    return TRUE;
  }

 private:
  tstring app_name_;
  CHandle mutex_;
  HWND prev_;
};
}  // namespace umu
