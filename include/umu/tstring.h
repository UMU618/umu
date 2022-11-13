#pragma once

#include <tchar.h>

#include <string>
#include <string_view>

namespace umu {

using tstring = std::basic_string_view<TCHAR>;
using tstring_view = std::basic_string_view<TCHAR>;

}  // namespace umu
