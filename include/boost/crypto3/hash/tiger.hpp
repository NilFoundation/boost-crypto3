//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Pavel Kharitonov <ipavrus@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_HASH_TIGER_HPP
#define CRYPTO3_HASH_TIGER_HPP

#include <boost/crypto3/hash/detail/tiger/tiger_policy.hpp>

#include <boost/crypto3/hash/detail/merkle_damgard_construction.hpp>
#include <boost/crypto3/hash/detail/tiger/tiger_padding.hpp>
#include <boost/crypto3/hash/detail/block_stream_processor.hpp>

namespace boost {
    namespace crypto3 {
        namespace hashes {

            template<std::size_t DigestBits = 192, std::size_t Passes = 3>
            struct tiger_compressor {
                typedef detail::tiger_policy<DigestBits, Passes> policy_type;

                constexpr static const std::size_t word_bits = policy_type::word_bits;
                typedef typename policy_type::word_type word_type;

                constexpr static const std::size_t state_bits = policy_type::state_bits;
                constexpr static const std::size_t state_words = policy_type::state_words;
                typedef typename policy_type::state_type state_type;

                constexpr static const std::size_t block_bits = policy_type::block_bits;
                constexpr static const std::size_t block_words = policy_type::block_words;
                typedef typename policy_type::block_type block_type;

                static inline void process_block(state_type &state, const block_type &block) {

                    word_type A = state[0], B = state[1], C = state[2];
                    block_type input = block;
                    policy_type::pass(A, B, C, input, 5);
                    policy_type::mix(input);
                    policy_type::pass(C, A, B, input, 7);
                    policy_type::mix(input);
                    policy_type::pass(B, C, A, input, 9);

                    for (size_t j = 3; j != policy_type::passes; ++j) {
                        policy_type::mix(input);
                        policy_type::pass(A, B, C, input, 9);
                        word_type T = A;
                        A = C;
                        C = B;
                        B = T;
                    }

                    state[0] ^= A;
                    state[1] = B - state[1];
                    state[2] += C;
                }
            };

            /*!
             * @brief Tiger. An older 192-bit hashes function, optimized for 64-bit
             * systems. Possibly vulnerable to side channels due to its use of table
             * lookups. Prefer Skein-512 or BLAKE2b in new code.
             *
             * @ingroup hashes
             */
            template<std::size_t DigestBits = 192, std::size_t Passes = 3>
            class tiger {
                typedef detail::tiger_policy<DigestBits, Passes> policy_type;

            public:
                struct construction {
                    struct params_type {
                        typedef typename policy_type::digest_endian digest_endian;

                        constexpr static const std::size_t length_bits = policy_type::word_bits;
                        constexpr static const std::size_t digest_bits = policy_type::digest_bits;
                    };

                    typedef merkle_damgard_construction<params_type, typename policy_type::iv_generator,
                                                        tiger_compressor<DigestBits, Passes>,
                                                        detail::tiger_padding<policy_type>>
                        type;
                };

                template<typename StateAccumulator, std::size_t ValueBits>
                struct stream_processor {
                    struct params_type {
                        typedef typename policy_type::digest_endian digest_endian;

                        constexpr static const std::size_t value_bits = ValueBits;
                    };

                    typedef block_stream_processor<construction, StateAccumulator, params_type> type;
                };

                constexpr static const std::size_t digest_bits = policy_type::digest_bits;
                typedef typename policy_type::digest_type digest_type;
            };
        }    // namespace hashes
    }        // namespace crypto3
}    // namespace boost

#endif
