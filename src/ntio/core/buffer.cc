//
// Created by flipback on 11/16/19.
//

#include "ntio/core/buffer.h"

namespace ntio::core {

Buffer::Buffer(size_t capacity) : buffer_(0), position_{0} { buffer_.reserve(capacity); }

Buffer::Buffer(const std::vector<uint8_t> &data) : buffer_(data), position_{0} {}

Buffer::Buffer() : buffer_(0), position_{0} {}

Buffer &Buffer::operator<<(uint8_t val) {
  buffer_.push_back(val);
  return *this;
  ;
}

Buffer &Buffer::operator>>(uint8_t &val) {
  val = buffer_[position_++];
  return *this;
}

Buffer &Buffer::operator<<(int8_t val) {
  buffer_.push_back(static_cast<uint8_t>(val));
  return *this;
}

Buffer &Buffer::operator>>(int8_t &val) {
  val = static_cast<uint8_t>(buffer_[position_++]);
  return *this;
}

Buffer &Buffer::operator<<(uint16_t val) {
  buffer_.push_back(static_cast<uint8_t>(val));
  buffer_.push_back(static_cast<uint8_t>(val >> 8));
  return *this;
}

Buffer &Buffer::operator>>(uint16_t &val) {
  val = static_cast<uint16_t>(buffer_[position_++]);
  val += static_cast<uint16_t>(buffer_[position_++] << 8);
  return *this;
}

Buffer &Buffer::operator<<(int16_t val) {
  return *this << static_cast<uint16_t>(val);
  ;
}

Buffer &Buffer::operator>>(int16_t &val) { return *this >> reinterpret_cast<uint16_t &>(val); }

Buffer &Buffer::operator<<(uint32_t val) {
  buffer_.push_back(static_cast<uint8_t>(val));
  buffer_.push_back(static_cast<uint8_t>(val >> 8));
  buffer_.push_back(static_cast<uint8_t>(val >> 16));
  buffer_.push_back(static_cast<uint8_t>(val >> 24));

  return *this;
}

Buffer &Buffer::operator>>(uint32_t &val) {
  val = static_cast<uint32_t>(buffer_[position_++]);
  val += static_cast<uint32_t>(buffer_[position_++] << 8);
  val += static_cast<uint32_t>(buffer_[position_++] << 16);
  val += static_cast<uint32_t>(buffer_[position_++] << 24);

  return *this;
}

Buffer &Buffer::operator<<(int32_t val) { return *this << static_cast<uint32_t>(val); }

Buffer &Buffer::operator>>(int32_t &val) { return *this >> reinterpret_cast<uint32_t &>(val); }

Buffer &Buffer::operator<<(uint64_t val) {
  buffer_.push_back(static_cast<uint8_t>(val));
  buffer_.push_back(static_cast<uint8_t>(val >> 8));
  buffer_.push_back(static_cast<uint8_t>(val >> 16));
  buffer_.push_back(static_cast<uint8_t>(val >> 24));
  buffer_.push_back(static_cast<uint8_t>(val >> 32));
  buffer_.push_back(static_cast<uint8_t>(val >> 40));
  buffer_.push_back(static_cast<uint8_t>(val >> 48));
  buffer_.push_back(static_cast<uint8_t>(val >> 56));

  return *this;
}

Buffer &Buffer::operator>>(uint64_t &val) {
  val = static_cast<uint64_t>(buffer_[position_++]);
  val += static_cast<uint64_t>(buffer_[position_++]) << 8;
  val += static_cast<uint64_t>(buffer_[position_++]) << 16;
  val += static_cast<uint64_t>(buffer_[position_++]) << 24;
  val += static_cast<uint64_t>(buffer_[position_++]) << 32;
  val += static_cast<uint64_t>(buffer_[position_++]) << 40;
  val += static_cast<uint64_t>(buffer_[position_++]) << 48;
  val += static_cast<uint64_t>(buffer_[position_++]) << 56;

  return *this;
}

Buffer &Buffer::operator<<(int64_t val) { return *this << static_cast<uint64_t>(val); }

Buffer &Buffer::operator>>(int64_t &val) { return *this >> reinterpret_cast<uint64_t &>(val); }

Buffer &Buffer::operator<<(float val) { return *this << reinterpret_cast<uint32_t &>(val); }

Buffer &Buffer::operator>>(float &val) { return *this >> reinterpret_cast<uint32_t &>(val); }

Buffer &Buffer::operator<<(double val) { return *this << reinterpret_cast<uint64_t &>(val); }

Buffer &Buffer::operator>>(double &val) { return *this >> reinterpret_cast<uint64_t &>(val); }

Buffer &Buffer::operator<<(const std::vector<uint8_t> &val) {
  buffer_.insert(buffer_.end(), val.begin(), val.end());
  return *this;
}

Buffer &Buffer::operator>>(std::vector<uint8_t> &val) {
  std::copy(buffer_.begin() + position_, buffer_.begin() + position_ + val.size(), val.begin());
  position_ += val.size();
  return *this;
}

Buffer &Buffer::operator<<(const std::vector<uint16_t> &val) {
  for (auto &v : val) {
    buffer_.push_back(static_cast<uint8_t>(v));
    buffer_.push_back(static_cast<uint8_t>(v >> 8));
  }

  return *this;
}

Buffer &Buffer::operator>>(std::vector<uint16_t> &val) {
  for (auto &v : val) {
    v = static_cast<int16_t>(buffer_[position_++]);
    v += static_cast<int16_t>(buffer_[position_++] << 8);
  }

  return *this;
}
}  // namespace ntio::core
