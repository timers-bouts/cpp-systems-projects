#pragma once

#include <cstddef>
#include <cstdint>
#include <array>

namespace telemetry::format {

inline constexpr std::size_t kMagicSize   = 4;
inline constexpr std::size_t kVersionSize = 2;
inline constexpr std::size_t kFlagsSize   = 2;

inline constexpr std::size_t kHeaderSize =
    kMagicSize + kVersionSize + kFlagsSize;

inline constexpr std::uint16_t kCurrentVersion = 1;

inline constexpr std::array<std::uint8_t, 4> kMagic{'T','L','R','Y'};

} // namespace telemetry::format
