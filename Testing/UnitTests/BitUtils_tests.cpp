#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "../../src/BitUtils.h"
#include "catch.hpp"

// Testing functions in BitUtils.h
TEST_CASE("Bit Manipulation: Testing BitUtils.h Functions") {
  SECTION("int_to_bytes() Tests:") {

    // Testing random integers
    int num1 = 256;
    std::vector<unsigned char> bytes1 = int_to_bytes(num1);

    REQUIRE(bytes1.size() == 4);
    REQUIRE(bytes1[0] == 0x00);
    REQUIRE(bytes1[1] == 0x00);
    REQUIRE(bytes1[2] == 0x01);
    REQUIRE(bytes1[3] == 0x00);

    int num2 = 8172;
    std::vector<unsigned char> bytes2 = int_to_bytes(num2);

    REQUIRE(bytes2.size() == 4);
    REQUIRE(bytes2[0] == 0x00);
    REQUIRE(bytes2[1] == 0x00);
    REQUIRE(bytes2[2] == 0x1F);
    REQUIRE(bytes2[3] == 0xEC);

    // Testing minimum sized integer 0
    int num3 = 0;
    std::vector<unsigned char> bytes3 = int_to_bytes(num3);

    REQUIRE(bytes3.size() == 4);
    REQUIRE(bytes3[0] == 0x00);
    REQUIRE(bytes3[1] == 0x00);
    REQUIRE(bytes3[2] == 0x00);
    REQUIRE(bytes3[3] == 0x00);

    // Testing maximum sized integer
    int num4 = 2147483647;
    std::vector<unsigned char> bytes4 = int_to_bytes(num4);

    REQUIRE(bytes4.size() == 4);
    REQUIRE(bytes4[0] == 0x7F);
    REQUIRE(bytes4[1] == 0xFF);
    REQUIRE(bytes4[2] == 0xFF);
    REQUIRE(bytes4[3] == 0xFF);

    // Testing negative integer, should throw invalid argument exception
    int num5 = -1;
    REQUIRE_THROWS_AS(int_to_bytes(num5), std::invalid_argument);
  }

  SECTION("byte_to_int() Tests:") {
    // Testing random byte vectors
    std::vector<unsigned char> bytes1{0x00, 0x00, 0x01, 0x00};
    REQUIRE(byte_to_int(bytes1) == 256);

    std::vector<unsigned char> bytes2 = {0x00, 0x00, 0x1F, 0xEC};
    REQUIRE(byte_to_int(bytes2) == 8172);

    // Testing for maximum sized integer
    std::vector<unsigned char> bytes3 = {0x7F, 0xFF, 0xFF, 0xFF};
    REQUIRE(byte_to_int(bytes3) == 2147483647);

    // Testing integer of size 0
    std::vector<unsigned char> bytes4 = {0x00, 0x00, 0x00, 0x00};
    REQUIRE(byte_to_int(bytes4) == 0);

    // Testing with invalid vector size, should throw invalid argument exception
    std::vector<unsigned char> bytes5 = {0x00};
    REQUIRE_THROWS_AS(byte_to_int(bytes5), std::invalid_argument);
  }
}