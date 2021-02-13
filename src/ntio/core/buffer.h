// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_CORE_BUFFER_H_
#define NTIO_CORE_BUFFER_H_

#include <cstddef>
#include <cstdint>
#include <vector>

namespace ntio::core {

enum Order { kLittleEndian = 0, kBigEndian };

/**
 * @class Buffer
 *
 * @brief Implements decode and encode data in little endian format
 *
 * An example:
 * @code
 * Buffer buffer();
 * uint16_t var1 = 1;
 * uint32_t var2 = 0xaa00000;
 *
 * buffer << var1 << var2;
 *
 * buffer.data(); # => {0x01, 0x0, 0x0 ,0x0, 0x0, 0xaa}
 * @endcode
 */
template <Order O = Order::kLittleEndian>
class Buffer {
 public:
  /**
   * Creates an empty buffer
   * @param capacity the size that will be reserved in the buffer
   */
  explicit Buffer(size_t capacity);

  /**
   * Creates a buffer that contains the given data
   * @param data The data to encode
   */
  explicit Buffer(const std::vector<uint8_t> &data);

  /**
   * Creates an empty buffer
   */
  Buffer();

  Buffer &operator<<(uint8_t val);
  Buffer &operator>>(uint8_t &val);

  Buffer &operator<<(int8_t val);
  Buffer &operator>>(int8_t &val);

  Buffer &operator<<(uint16_t val);
  Buffer &operator>>(uint16_t &val);

  Buffer &operator<<(int16_t val);
  Buffer &operator>>(int16_t &val);

  Buffer &operator<<(uint32_t val);
  Buffer &operator>>(uint32_t &val);

  Buffer &operator<<(int32_t val);
  Buffer &operator>>(int32_t &val);

  Buffer &operator<<(uint64_t val);
  Buffer &operator>>(uint64_t &val);

  Buffer &operator<<(int64_t val);
  Buffer &operator>>(int64_t &val);

  Buffer &operator<<(float val);
  Buffer &operator>>(float &val);

  Buffer &operator<<(double val);
  Buffer &operator>>(double &val);

  Buffer &operator<<(const std::vector<uint8_t> &val);
  Buffer &operator>>(std::vector<uint8_t> &val);

  Buffer &operator<<(const std::vector<uint16_t> &val);
  Buffer &operator>>(std::vector<uint16_t> &val);

  [[nodiscard]] std::vector<uint8_t> &data() { return buffer_; }
  [[nodiscard]] const std::vector<uint8_t> &data() const { return buffer_; }
  [[nodiscard]] size_t size() const { return buffer_.size(); }
  [[nodiscard]] size_t pos() const { return position_; }
  [[nodiscard]] bool is_valid() const { return position_ <= buffer_.size(); }
  [[nodiscard]] bool empty() const { return position_ >= buffer_.size(); }

 private:
  std::vector<uint8_t> buffer_;
  size_t position_;
};

template <Order O>
Buffer<O>::Buffer(size_t capacity) : buffer_(0), position_{0} {
  buffer_.reserve(capacity);
}

template <Order O>
Buffer<O>::Buffer(const std::vector<uint8_t> &data) : buffer_(data), position_{0} {}

template <Order O>
Buffer<O>::Buffer() : buffer_(0), position_{0} {}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(uint8_t val) {
  buffer_.push_back(val);
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(uint8_t &val) {
  val = buffer_[position_++];
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(int8_t val) {
  buffer_.push_back(static_cast<uint8_t>(val));
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(int8_t &val) {
  val = static_cast<uint8_t>(buffer_[position_++]);
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(uint16_t val) {
  if constexpr (O == Order::kLittleEndian) {
    buffer_.push_back(static_cast<uint8_t>(val));
    buffer_.push_back(static_cast<uint8_t>(val >> 8));
  } else {
    buffer_.push_back(static_cast<uint8_t>(val >> 8));
    buffer_.push_back(static_cast<uint8_t>(val));
  }
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(uint16_t &val) {
  if constexpr (O == Order::kLittleEndian) {
    val = static_cast<uint16_t>(buffer_[position_++]);
    val += static_cast<uint16_t>(buffer_[position_++] << 8);
  } else {
    val += static_cast<uint16_t>(buffer_[position_++] << 8);
    val = static_cast<uint16_t>(buffer_[position_++]);
  }
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(int16_t val) {
  return *this << static_cast<uint16_t>(val);
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(int16_t &val) {
  return *this >> reinterpret_cast<uint16_t &>(val);
}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(uint32_t val) {
  if constexpr (O == Order::kLittleEndian) {
    buffer_.push_back(static_cast<uint8_t>(val));
    buffer_.push_back(static_cast<uint8_t>(val >> 8));
    buffer_.push_back(static_cast<uint8_t>(val >> 16));
    buffer_.push_back(static_cast<uint8_t>(val >> 24));
  } else {
    buffer_.push_back(static_cast<uint8_t>(val >> 24));
    buffer_.push_back(static_cast<uint8_t>(val >> 16));
    buffer_.push_back(static_cast<uint8_t>(val >> 8));
    buffer_.push_back(static_cast<uint8_t>(val));
  }
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(uint32_t &val) {
  if constexpr (O == Order::kLittleEndian) {
    val = static_cast<uint32_t>(buffer_[position_++]);
    val += static_cast<uint32_t>(buffer_[position_++] << 8);
    val += static_cast<uint32_t>(buffer_[position_++] << 16);
    val += static_cast<uint32_t>(buffer_[position_++] << 24);
  } else {
    val += static_cast<uint32_t>(buffer_[position_++] << 24);
    val += static_cast<uint32_t>(buffer_[position_++] << 16);
    val += static_cast<uint32_t>(buffer_[position_++] << 8);
    val = static_cast<uint32_t>(buffer_[position_++]);
  }
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(int32_t val) {
  return *this << static_cast<uint32_t>(val);
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(int32_t &val) {
  return *this >> reinterpret_cast<uint32_t &>(val);
}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(uint64_t val) {
  if constexpr (O == Order::kLittleEndian) {
    buffer_.push_back(static_cast<uint8_t>(val));
    buffer_.push_back(static_cast<uint8_t>(val >> 8));
    buffer_.push_back(static_cast<uint8_t>(val >> 16));
    buffer_.push_back(static_cast<uint8_t>(val >> 24));
    buffer_.push_back(static_cast<uint8_t>(val >> 32));
    buffer_.push_back(static_cast<uint8_t>(val >> 40));
    buffer_.push_back(static_cast<uint8_t>(val >> 48));
    buffer_.push_back(static_cast<uint8_t>(val >> 56));
  } else {
    buffer_.push_back(static_cast<uint8_t>(val >> 56));
    buffer_.push_back(static_cast<uint8_t>(val >> 48));
    buffer_.push_back(static_cast<uint8_t>(val >> 40));
    buffer_.push_back(static_cast<uint8_t>(val >> 32));
    buffer_.push_back(static_cast<uint8_t>(val >> 24));
    buffer_.push_back(static_cast<uint8_t>(val >> 16));
    buffer_.push_back(static_cast<uint8_t>(val >> 8));
    buffer_.push_back(static_cast<uint8_t>(val));
  }
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(uint64_t &val) {
  if constexpr (O == Order::kLittleEndian) {
    val = static_cast<uint64_t>(buffer_[position_++]);
    val += static_cast<uint64_t>(buffer_[position_++]) << 8;
    val += static_cast<uint64_t>(buffer_[position_++]) << 16;
    val += static_cast<uint64_t>(buffer_[position_++]) << 24;
    val += static_cast<uint64_t>(buffer_[position_++]) << 32;
    val += static_cast<uint64_t>(buffer_[position_++]) << 40;
    val += static_cast<uint64_t>(buffer_[position_++]) << 48;
    val += static_cast<uint64_t>(buffer_[position_++]) << 56;
  } else {
    val += static_cast<uint64_t>(buffer_[position_++]) << 56;
    val += static_cast<uint64_t>(buffer_[position_++]) << 48;
    val += static_cast<uint64_t>(buffer_[position_++]) << 40;
    val += static_cast<uint64_t>(buffer_[position_++]) << 32;
    val += static_cast<uint64_t>(buffer_[position_++]) << 24;
    val += static_cast<uint64_t>(buffer_[position_++]) << 16;
    val += static_cast<uint64_t>(buffer_[position_++]) << 8;
    val = static_cast<uint64_t>(buffer_[position_++]);
  }
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(int64_t val) {
  return *this << static_cast<uint64_t>(val);
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(int64_t &val) {
  return *this >> reinterpret_cast<uint64_t &>(val);
}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(float val) {
  return *this << reinterpret_cast<uint32_t &>(val);
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(float &val) {
  return *this >> reinterpret_cast<uint32_t &>(val);
}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(double val) {
  return *this << reinterpret_cast<uint64_t &>(val);
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(double &val) {
  return *this >> reinterpret_cast<uint64_t &>(val);
}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(const std::vector<uint8_t> &val) {
  buffer_.insert(buffer_.end(), val.begin(), val.end());
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(std::vector<uint8_t> &val) {
  std::copy(buffer_.begin() + position_, buffer_.begin() + position_ + val.size(), val.begin());
  position_ += val.size();
  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator<<(const std::vector<uint16_t> &val) {
  for (auto &v : val) {
    if constexpr (O == Order::kLittleEndian) {
      buffer_.push_back(static_cast<uint8_t>(v));
      buffer_.push_back(static_cast<uint8_t>(v >> 8));
    } else {
      buffer_.push_back(static_cast<uint8_t>(v >> 8));
      buffer_.push_back(static_cast<uint8_t>(v));
    }
  }

  return *this;
}

template <Order O>
Buffer<O> &Buffer<O>::operator>>(std::vector<uint16_t> &val) {
  for (auto &v : val) {
    v = static_cast<int16_t>(buffer_[position_++]);
    v += static_cast<int16_t>(buffer_[position_++] << 8);
  }

  return *this;
}

}  // namespace ntio::core
// namespace ntio::core
#endif  // NTIO_CORE_BUFFER_H_
