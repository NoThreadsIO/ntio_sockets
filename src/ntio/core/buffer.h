//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef EIPSCANNER_UTILS_BUFFER_H
#define EIPSCANNER_UTILS_BUFFER_H

#include <cstddef>
#include <cstdint>
#include <vector>

namespace ntio::core {
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
  explicit Buffer(const std::vector<uint8_t>& data);

  /**
   * Creates an empty buffer
   */
  Buffer();

  Buffer& operator<<(uint8_t val);
  Buffer& operator>>(uint8_t& val);

  Buffer& operator<<(int8_t val);
  Buffer& operator>>(int8_t& val);

  Buffer& operator<<(uint16_t val);
  Buffer& operator>>(uint16_t& val);

  Buffer& operator<<(int16_t val);
  Buffer& operator>>(int16_t& val);

  Buffer& operator<<(uint32_t val);
  Buffer& operator>>(uint32_t& val);

  Buffer& operator<<(int32_t val);
  Buffer& operator>>(int32_t& val);

  Buffer& operator<<(uint64_t val);
  Buffer& operator>>(uint64_t& val);

  Buffer& operator<<(int64_t val);
  Buffer& operator>>(int64_t& val);

  Buffer& operator<<(float val);
  Buffer& operator>>(float& val);

  Buffer& operator<<(double val);
  Buffer& operator>>(double& val);

  Buffer& operator<<(const std::vector<uint8_t>& val);
  Buffer& operator>>(std::vector<uint8_t>& val);

  Buffer& operator<<(const std::vector<uint16_t>& val);
  Buffer& operator>>(std::vector<uint16_t>& val);

  [[nodiscard]] std::vector<uint8_t>& data() { return buffer_; }
  [[nodiscard]] const std::vector<uint8_t>& data() const { return buffer_; }
  [[nodiscard]] size_t size() const { return buffer_.size(); }
  [[nodiscard]] size_t pos() const { return position_; }
  [[nodiscard]] bool is_valid() const { return position_ <= buffer_.size(); }
  [[nodiscard]] bool empty() const { return position_ >= buffer_.size(); }

 private:
  std::vector<uint8_t> buffer_;
  size_t position_;
};
}  // namespace ntio::core
// namespace ntio::core
#endif  // EIPSCANNER_BUFFER_H
