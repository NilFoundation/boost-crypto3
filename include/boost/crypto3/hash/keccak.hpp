//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_HASH_KECCAK_HPP
#define CRYPTO3_HASH_KECCAK_HPP

#include <string>

#include <boost/crypto3/hash/detail/keccak/keccak_functions.hpp>

#include <boost/crypto3/hash/detail/block_stream_processor.hpp>
#include <boost/crypto3/hash/detail/sponge_construction.hpp>

namespace boost {
    namespace crypto3 {
        namespace hash {
            template<std::size_t DigestBits = 512>
            class keccak_1600_compressor {
                typedef detail::keccak_1600_functions<DigestBits> policy_type;

            public:
            };

            /*!
             * @brief
             * @tparam DigestBits
             * @ingroup hash
             */
            template<std::size_t DigestBits = 512>
            class keccak_1600 {
                typedef detail::keccak_1600_functions<DigestBits> policy_type;

            public:
                struct construction {
                    struct params_type {
                        typedef typename stream_endian::little_octet_big_bit digest_endian;

                        constexpr static const std::size_t length_bits = 0;    // No length padding
                        constexpr static const std::size_t digest_bits = policy_type::digest_bits;
                    };
                    typedef sponge_construction<params_type, typename policy_type::iv_generator,
                                                keccak_1600_compressor<DigestBits>>
                        type;
                };

                template<typename StateAccumulator, std::size_t ValueBits>
                struct stream_processor {
                    struct params_type {
                        typedef typename stream_endian::little_octet_big_bit endian;

                        constexpr static const std::size_t value_bits = ValueBits;
                    };

                    typedef block_stream_processor<construction, StateAccumulator, params_type> type;
                };

                constexpr static const std::size_t digest_bits = policy_type::digest_bits;
                typedef typename policy_type::digest_type digest_type;
            };
        }    // namespace hash
    }        // namespace crypto3
}    // namespace nil

#endif