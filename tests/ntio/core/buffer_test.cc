// Copyright (c) 2020-2021 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "ntio/core/buffer.h"

#include <catch2/catch.hpp>

using ntio::core::Buffer;
using Bytes = std::vector<uint8_t>;

TEST_CASE("Buffer") {
  SECTION("should decode") {
    SECTION("uint8") {
      Buffer buf(Bytes{0x01});
      uint8_t var;

      buf >> var;
      REQUIRE(var == 0x01);
      REQUIRE(buf.is_valid());
    }

    SECTION("int8") {
      Buffer buf(Bytes{0xff});
      int8_t var;

      buf >> var;
      REQUIRE(var == -1);
      REQUIRE(buf.is_valid());
    }

    SECTION("uint16") {
      Buffer buf(Bytes{0x01, 0x02});
      uint16_t var;

      buf >> var;
      REQUIRE(var == 0x0201);
      REQUIRE(buf.is_valid());
    }

    SECTION("int16") {
      Buffer buf(Bytes{0xfe, 0xff});
      int16_t var;

      buf >> var;
      REQUIRE(var == -2);
      REQUIRE(buf.is_valid());
    }

    SECTION("uint32") {
      Buffer buf(Bytes{0x01, 0x02, 0x03, 0x04});
      uint32_t var;

      buf >> var;
      REQUIRE(var == 0x04030201);
      REQUIRE(buf.is_valid());
    }

    SECTION("int32") {
      Buffer buf(Bytes{0xfd, 0xff, 0xff, 0xff});
      int32_t var;

      buf >> var;
      REQUIRE(var == -3);
      REQUIRE(buf.is_valid());
    }

    SECTION("uint64") {
      Buffer buf(Bytes{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});
      uint64_t var;

      buf >> var;
      REQUIRE(var == 0x0807060504030201l);
      REQUIRE(buf.is_valid());
    }

    SECTION("int64") {
      Buffer buf(Bytes{0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff});
      int64_t var;

      buf >> var;
      REQUIRE(var == -4);
      REQUIRE(buf.is_valid());
    }

    SECTION("float") {
      Buffer buf(Bytes{0, 0, 0x28, 0x41});
      float var;

      buf >> var;
      REQUIRE(var == 10.5f);
      REQUIRE(buf.is_valid());
    }

    SECTION("double") {
      Buffer buf(Bytes{0, 0, 0, 0, 0, 0, 0x25, 0x40});
      double var;

      buf >> var;
      REQUIRE(var == 10.5);
      REQUIRE(buf.is_valid());
    }
  }

  Buffer buf;
  SECTION("should encode") {
    SECTION("uint8") {
      buf << uint8_t{0x01};
      REQUIRE(buf.data() == Bytes{0x01});
    }

    SECTION("int8") {
      buf << int8_t{-1};
      REQUIRE(buf.data() == Bytes{0xff});
    }

    SECTION("uint16") {
      buf << uint16_t{0x0201};
      REQUIRE(buf.data() == Bytes{0x01, 0x02});
    }

    SECTION("int16") {
      buf << int16_t{-2};
      REQUIRE(buf.data() == Bytes{0xfe, 0xff});
    }

    SECTION("uint32") {
      buf << uint32_t{0x04030201};
      REQUIRE(buf.data() == Bytes{0x01, 0x02, 0x03, 0x04});
    }

    SECTION("uint32") {
      buf << int32_t{-3};
      REQUIRE(buf.data() == Bytes{0xfd, 0xff, 0xff, 0xff});
    }

    SECTION("uint64") {
      buf << uint64_t{0x0807060504030201};
      REQUIRE(buf.data() == Bytes{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});
    }

    SECTION("int64") {
      buf << int64_t{-4};
      REQUIRE(buf.data() == Bytes{0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff});
    }

    SECTION("float") {
      buf << float{10.5};
      REQUIRE(buf.data() == Bytes{0, 0, 0x28, 0x41});
    }

    SECTION("double") {
      buf << double{10.5};
      REQUIRE(buf.data() == Bytes{0, 0, 0, 0, 0, 0, 0x25, 0x40});
    }
  }
}