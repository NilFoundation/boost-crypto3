//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_BLOCK_CIPHERS_DETAIL_MD4_POLICY_HPP
#define CRYPTO3_BLOCK_CIPHERS_DETAIL_MD4_POLICY_HPP

#include <array>

#include <boost/crypto3/block/detail/md4/md4_functions.hpp>

namespace boost {
    namespace crypto3 {
        namespace block {
            namespace detail {
                struct md4_policy : md4_functions {

                    constexpr static const std::size_t block_bits = 128;
                    constexpr static const std::size_t block_words = block_bits / word_bits;
                    typedef std::array<word_type, block_words> block_type;

                    constexpr static const std::size_t key_words = 16;
                    constexpr static const std::size_t key_bits = key_words * word_bits;
                    typedef std::array<word_type, key_words> key_type;

                    constexpr static const std::size_t rounds = 48;
                    typedef std::array<unsigned, rounds> key_indexes_type;

                    constexpr static const key_indexes_type key_indexes = {{
                        0, 1, 2, 3,  4, 5,  6, 7,  8, 9, 10, 11, 12, 13, 14, 15,

                        0, 4, 8, 12, 1, 5,  9, 13, 2, 6, 10, 14, 3,  7,  11, 15,

                        0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5,  13, 3,  11, 7,  15,
                    }};
                };

                constexpr md4_policy::key_indexes_type const md4_policy::key_indexes;
            }    // namespace detail
        }        // namespace block
    }            // namespace crypto3
}    // namespace boost

#endif    // CRYPTO3_BLOCK_CIPHERS_DETAIL_MD4_POLICY_HPP
