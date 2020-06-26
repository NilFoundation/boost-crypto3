//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Alexander Sokolov <asokolov@nil.foundation>
// Copyright (c) 2020 Nikita Kaskov <nbering@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#define BOOST_TEST_MODULE md4_test

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/crypto3/hash/algorithm/hash.hpp>

#include <boost/crypto3/hash/md4.hpp>
#include <boost/crypto3/hash/hash_state.hpp>

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

BOOST_TEST_DONT_PRINT_LOG_VALUE(md4::construction::type::digest_type)

class fixture {
public:
    accumulator_set<md4> acc;
    virtual ~fixture() {
    }
};

const char *test_data = "data/md4.json";

boost::property_tree::ptree string_data() {
    boost::property_tree::ptree string_data;
    boost::property_tree::read_json(test_data, string_data);

    return string_data; 
}

BOOST_AUTO_TEST_SUITE(md4_stream_processor_filedriven_test_suite)

BOOST_DATA_TEST_CASE(md4_string_various_range_value_hash, string_data(), array_element) {
    std::string out = hash<md4>(array_element.first);

    BOOST_CHECK_EQUAL(out, array_element.second.data());
}

BOOST_DATA_TEST_CASE(md4_string_various_itr_value_hash, string_data(), array_element) {
    std::string out = hash<md4>(array_element.first.begin(), array_element.first.end());

    BOOST_CHECK_EQUAL(out, array_element.second.data());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(md4_stream_processor_test_suite)

BOOST_AUTO_TEST_CASE(md4_shortmsg_byte1) { 
    // "a"
    std::array<char, 1> a = {'\x61'};
    md4::digest_type d = hash<md4>(a);

    BOOST_CHECK_EQUAL("bde52cb31de33e46245e05fbdbd6fb24", std::to_string(d).data());
}

BOOST_AUTO_TEST_CASE(md4_shortmsg_byte2) {
    // "abc"
    std::array<char, 3> a = {'\x61', '\x62', '\x63'};
    md4::digest_type d = hash<md4>(a);

    BOOST_CHECK_EQUAL("a448017aaf21d8525fc10ae87aa6729d", std::to_string(d).data());
}

BOOST_AUTO_TEST_CASE(md4_shortmsg_byte3) {
    // "message digest"
    std::array<char, 14> a = {'\x6d', '\x65', '\x73', '\x73', '\x61', '\x67', '\x65', '\x20', '\x64', '\x69', '\x67', 
                             '\x65', '\x73', '\x74'};
    md4::digest_type d = hash<md4>(a);

    BOOST_CHECK_EQUAL("d9130a8164549fe818874806e1c7014b", std::to_string(d).data());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(md4_accumulator_test_suite)

BOOST_FIXTURE_TEST_CASE(md4_accumulator1, fixture) {
    // "a"
    md4::block_type m = {{}};
    m[0] = 0x00000061;
    acc(m, nil::crypto3::accumulators::bits = 8);
    md4::digest_type s = extract::hash<md4>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s));
#endif

    BOOST_CHECK_EQUAL("bde52cb31de33e46245e05fbdbd6fb24", std::to_string(s).data());
}

BOOST_FIXTURE_TEST_CASE(md4_accumulator2, fixture) {
    // "abc" 
    md4::construction::type::block_type m = {{}};
    m[0] = 0x00636261;
    acc(m, nil::crypto3::accumulators::bits = 24);
    md4::construction::type::digest_type s = extract::hash<md4>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s));
#endif

    BOOST_CHECK_EQUAL("a448017aaf21d8525fc10ae87aa6729d", std::to_string(s));
}

BOOST_FIXTURE_TEST_CASE(md4_accumulator3, fixture) {
    // 80 times of "a"
    md4::block_type m1 = {
        {0x61616161, 0x61616161, 0x61616161, 0x61616161, 0x61616161, 0x61616161, 0x61616161, 
         0x61616161, 0x61616161, 0x61616161, 0x61616161, 0x61616161, 0x61616161, 0x61616161, 
         0x00010261, 0x67283609}};
    acc(m1, nil::crypto3::accumulators::bits = 14 * 32 + 8);

    md4::digest_type s = extract::hash<md4>(acc);

    BOOST_CHECK_EQUAL("872097e6f78e3b53f890459d03bc6fb7", std::to_string(s).data());

    md4::construction::type::block_type m2 = {
        {0x61616161, 0x61616161, 0x61616161, 0x61616161, 0x61616161, 0x00616161, 0x82934724, 
         0xa0a93453, 0x293c203d, 0x6e6f7071, 0x6f707172, 0x70717273, 0x71727374, 0x72737475, 
         0x00000000, 0x00000000}};

    acc(m2, nil::crypto3::accumulators::bits = 6 * 32 - 8);

    s = extract::hash<md4>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s).data());
#endif

    BOOST_CHECK_EQUAL("721a93b051049c47487b06a59acc7d64", std::to_string(s).data());
}

BOOST_AUTO_TEST_CASE(md4_preprocessor1) {
    accumulator_set<md4> acc;
    md4::construction::type::digest_type s = extract::hash<md4>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s));
#endif

    BOOST_CHECK_EQUAL("31d6cfe0d16ae931b73c59d7e0c089c0", std::to_string(s));
}

BOOST_AUTO_TEST_CASE(md4_preprocessor2) {
    accumulator_set<md4> acc;
    acc(0x00000061, nil::crypto3::accumulators::bits = 8);
    acc(0x00000062, nil::crypto3::accumulators::bits = 8);
    acc(0x00000063, nil::crypto3::accumulators::bits = 8);

    md4::construction::type::digest_type s = extract::hash<md4>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s));
#endif

    BOOST_CHECK_EQUAL("a448017aaf21d8525fc10ae87aa6729d", std::to_string(s));
}

BOOST_AUTO_TEST_CASE(md4_preprocessor3) {
    // million repetitions of "a"
    accumulator_set<md4> acc;
    for (unsigned i = 0; i < 1000000; ++i) {
        acc(0x00000061, nil::crypto3::accumulators::bits = 8);
    }
    md4::construction::type::digest_type s = extract::hash<md4>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s));
#endif

    BOOST_CHECK_EQUAL("bbce80cc6bb65e5c6745e30d4eeca9a4", std::to_string(s));
}

BOOST_AUTO_TEST_CASE(md4_preprocessor4) {
    // 8 repetitions of "1234567890"
    accumulator_set<md4> acc;
    for (unsigned i = 0; i < 8; ++i) {
        acc(0x00000031, nil::crypto3::accumulators::bits = 8);
        acc(0x00000032, nil::crypto3::accumulators::bits = 8);
        acc(0x00000033, nil::crypto3::accumulators::bits = 8);
        acc(0x00000034, nil::crypto3::accumulators::bits = 8);
        acc(0x00000035, nil::crypto3::accumulators::bits = 8);
        acc(0x00000036, nil::crypto3::accumulators::bits = 8);
        acc(0x00000037, nil::crypto3::accumulators::bits = 8);
        acc(0x00000038, nil::crypto3::accumulators::bits = 8);
        acc(0x00000039, nil::crypto3::accumulators::bits = 8);
        acc(0x00000030, nil::crypto3::accumulators::bits = 8);
    }
    md4::construction::type::digest_type s = extract::hash<md4>(acc);

#ifdef CRYPTO3_HASH_SHOW_PROGRESS
    std::printf("%s\n", std::to_string(s));
#endif

    BOOST_CHECK_EQUAL("e33b4ddc9c38f2199c3e7b164fcc0536", std::to_string(s));
}

BOOST_AUTO_TEST_SUITE_END()