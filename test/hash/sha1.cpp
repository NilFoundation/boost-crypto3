//---------------------------------------------------------------------------//
// Copyright (c) 2018-2019 Nil Foundation AG
// Copyright (c) 2018-2019 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Alexander Sokolov <asokolov@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#define BOOST_TEST_MODULE sha1_test

#include <nil/crypto3/hash/algorithm/hash.hpp>

#include <nil/crypto3/hash/sha1.hpp>
#include <nil/crypto3/hash/hash_state.hpp>

#include <cassert>
#include <cstring>
#include <unordered_map>

#include <boost/cstdint.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

using namespace nil::crypto3::hash;
using namespace nil::crypto3::accumulators;

namespace boost {
    namespace test_tools {
        namespace tt_detail {
            template<template<typename, typename> class P, typename K, typename V>
            struct print_log_value<P<K, V>> {
                void operator()(std::ostream&, P<K, V> const&) {
                }
            };
        }    // namespace tt_detail
    }        // namespace test_tools
}    // namespace boost

BOOST_TEST_DONT_PRINT_LOG_VALUE(sha1::construction::type::digest_type)

class fixture {
public:
    accumulator_set<sha1> acc;
    typedef sha1 hash_t;

    virtual ~fixture() {
    }
};

static const std::unordered_map<std::string, std::string> string_data = {
    {"a", "86f7e437faa5a7fce15d1ddcb9eaeaea377667b8"},
    {"abc", "a9993e364706816aba3e25717850c26c9cd0d89d"},
    {"message digest", "c12252ceda8be8994d5fa0290a47231c1d16aae3"},
    {"abcdefghijklmnopqrstuvwxyz", "32d10c7b8cf96570ca04ce37f2a19d84240d3a89"},
    {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "84983e441c3bd26ebaae4aa1f95129e5e54670f1"},
    {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "761c457bf73b14d27e9e9265c46f4b4dda11f940"}, 
    {"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
     "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", "a49b2446a02c645bf419f995b67091253a04a259"}};


BOOST_AUTO_TEST_SUITE(sha1_stream_processor_test_suite)

BOOST_DATA_TEST_CASE(sha1_range_hash, boost::unit_test::data::make(string_data), array_element) {
    std::string out = hash<sha1>(array_element.first);

    BOOST_CHECK_EQUAL(out, array_element.second);
}

BOOST_AUTO_TEST_CASE(sha1_shortmsg_byte1) {
    // echo -n "a" | sha1sum 
    std::array<char, 1> a = {'\x61'};
    sha1::digest_type d = hash<sha1>(a);

    BOOST_CHECK_EQUAL("86f7e437faa5a7fce15d1ddcb9eaeaea377667b8", std::to_string(d).data());
}

BOOST_AUTO_TEST_CASE(sha1_shortmsg_byte2) {
    // echo -n "abc" | sha1sum 
    std::array<char, 3> a = {'\x61', '\x62', '\x63'};
    sha1::digest_type d = hash<sha1>(a);

    BOOST_CHECK_EQUAL("a9993e364706816aba3e25717850c26c9cd0d89d", std::to_string(d).data());
}

BOOST_AUTO_TEST_CASE(sha1_shortmsg_byte3) {
    // echo -n "message digest" | sha1sum
    std::array<char, 14> a = {'\x6d', '\x65', '\x73', '\x73', '\x61', '\x67', '\x65', '\x20', '\x64', '\x69', '\x67', 
                             '\x65', '\x73', '\x74'};
    sha1::digest_type d = hash<sha1>(a);

    BOOST_CHECK_EQUAL("c12252ceda8be8994d5fa0290a47231c1d16aae3", std::to_string(d).data());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(sha1_accumulator_test_suite)

BOOST_FIXTURE_TEST_CASE(sha1_accumulator1, fixture) {
    // echo -n "a" | sha1sum
    hash_t::construction::type::block_type m = {{}};

    m[0] = 0x61000000;
    acc(m, nil::crypto3::accumulators::bits = 8);

    hash_t::digest_type s = extract::hash<hash_t>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s).data());
#endif

    BOOST_CHECK_EQUAL("86f7e437faa5a7fce15d1ddcb9eaeaea377667b8", std::to_string(s).data());
}

BOOST_FIXTURE_TEST_CASE(sha1_accumulator2, fixture) {
    // echo -n "abc" | sha1sum
    hash_t::construction::type::block_type m = {{}};

    m[0] = 0x61626300;
    acc(m, nil::crypto3::accumulators::bits = 24);

    hash_t::digest_type s = extract::hash<hash_t>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s).data());
#endif

    BOOST_CHECK_EQUAL("a9993e364706816aba3e25717850c26c9cd0d89d", std::to_string(s).data());
}

BOOST_FIXTURE_TEST_CASE(sha1_accumulator3, fixture) {
    // echo -n "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" | sha1sum
    hash_t::construction::type::block_type m1 = {
        {0x61626364, 0x62636465, 0x63646566, 0x64656667, 0x65666768, 0x66676869, 0x6768696a, 0x68696a6b, 0x696a6b6c,
         0x6a6b6c6d, 0x6b6c6d6e, 0x6c6d6e6f, 0x6d010101, 0x01010101, 0x80000000, 0x00000000}};
    acc(m1, nil::crypto3::accumulators::bits = 512 - 64 - 64 + 8);

    hash_t::digest_type s = extract::hash<hash_t>(acc);

    BOOST_CHECK_EQUAL("9d47791975c530645ad3568e80f88d7da4c52c3b", std::to_string(s).data());

    hash_t::construction::type::block_type m2 = {
        {0x6e6f706e, 0x6f707100, 0x6d6e6f70, 0x6e6f7071, 0x6d6e6f70, 0x6e6f7071, 0x0168696a, 0x68696a6b, 0x696a6b6c,
         0x6a6b6c6d, 0x6b6c6d6e, 0x6c6d6e6f, 0x6d010170, 0x6e6f7071, 0x80080000, 0x00000000}};

    acc(m2, nil::crypto3::accumulators::bits = 64 - 8);

    s = extract::hash<hash_t>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s).data());
#endif

    BOOST_CHECK_EQUAL("84983e441c3bd26ebaae4aa1f95129e5e54670f1", std::to_string(s).data());
}

BOOST_FIXTURE_TEST_CASE(sha1_accumulator4, fixture) {
    // echo -n "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn (continues)
    //          hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu" | sha1sum
    hash_t::construction::type::block_type m1 = {
        {0x61626364, 0x65666768, 0x62636465, 0x66676869, 0x63646566, 0x6768696a, 0x64656667, 0x68696a6b, 0x65666768, 
         0x696a6b6c, 0x66676869, 0x6a6b6c6d, 0x6768696a, 0x6b6c6d6e, 0x68696a6b, 0x6c6d6e6f}};
    acc(m1, nil::crypto3::accumulators::bits = 512);

    hash_t::digest_type s = extract::hash<hash_t>(acc);

    BOOST_CHECK_EQUAL("b85d6468bd3a73794bceaf812239cc1fe460ab95", std::to_string(s).data());

    hash_t::construction::type::block_type m2 = {
        {0x696a6b6c, 0x6d6e6f70, 0x6a6b6c6d, 0x6e6f7071, 0x6b6c6d6e, 0x6f707172, 0x6c6d6e6f, 0x70717273, 0x6d6e6f70,
         0x71727374, 0x6e6f7071, 0x72737475, 0x6d010170, 0x6e6f7071, 0x80080000, 0x00000000}};

    acc(m2, nil::crypto3::accumulators::bits = 64 * 6);

    s = extract::hash<hash_t>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s).data());
#endif

    BOOST_CHECK_EQUAL("a49b2446a02c645bf419f995b67091253a04a259", std::to_string(s).data());
}

BOOST_AUTO_TEST_CASE(sha1_preprocessor1) {
    accumulator_set<sha1> acc;
    sha1::digest_type s = extract::hash<sha1>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s).data());
#endif

    BOOST_CHECK_EQUAL("da39a3ee5e6b4b0d3255bfef95601890afd80709",
        std::to_string(s).data());
}

BOOST_AUTO_TEST_CASE(sha1_preprocessor2) {
    accumulator_set<sha1> acc;

    acc(0x61000000, nil::crypto3::accumulators::bits = 8);
    acc(0x62000000, nil::crypto3::accumulators::bits = 8);
    acc(0x63000000, nil::crypto3::accumulators::bits = 8);

    sha1::digest_type s = extract::hash<sha1>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s).data());
#endif

    BOOST_CHECK_EQUAL("a9993e364706816aba3e25717850c26c9cd0d89d",
        std::to_string(s).data());
}

BOOST_AUTO_TEST_CASE(sha1_preprocessor3) {
    // perl -e 'for (1..1000000) { print "a"; }' | sha1sum
    accumulator_set<sha1> acc;

    for (unsigned i = 0; i != 1000000; ++i)
        acc(0x61000000, nil::crypto3::accumulators::bits = 8);

    sha1::digest_type s = extract::hash<sha1>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s).data());
#endif

    BOOST_CHECK_EQUAL("34aa973cd4c4daa4f61eeb2bdbad27316534016f",
        std::to_string(s).data());
}

BOOST_AUTO_TEST_SUITE_END()