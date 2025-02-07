#pragma once
#include <type_traits>
#include <utility>

#include <redx/core.hpp>

namespace redx {

// to pack C enums

template <typename EnumT>
struct packed_enum
{
  using enum_type = EnumT;
  using underlying_type = std::underlying_type_t<EnumT>;

  FORCE_INLINE packed_enum& operator=(const enum_type& value)
  {
    ut_value = static_cast<underlying_type>(value);
    return *this;
  }

  FORCE_INLINE operator enum_type() const
  {
    return static_cast<enum_type>(ut_value);
  }

private:

  underlying_type ut_value;
};

} // namespace redx

