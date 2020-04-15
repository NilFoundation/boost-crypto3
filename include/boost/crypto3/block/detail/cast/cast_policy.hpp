//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_CAST_POLICY_HPP
#define CRYPTO3_CAST_POLICY_HPP

#include <array>

#include <boost/crypto3/block/detail/cast/cast_functions.hpp>

namespace boost {
    namespace crypto3 {
        namespace block {
            namespace detail {
                template<std::size_t Version, std::size_t KeyBits>
                struct cast_policy;

                template<std::size_t KeyBits>
                struct cast_policy<128, KeyBits> : public cast_functions<128, KeyBits> {
                    constexpr static const std::size_t rounds = cast_functions<128, KeyBits>::rounds;
                };

                template<std::size_t KeyBits>
                struct cast_policy<256, KeyBits> : public cast_functions<256, KeyBits> {
                    constexpr static const std::size_t rounds = cast_functions<256, KeyBits>::rounds;
                };
            }    // namespace detail
        }        // namespace block
    }            // namespace crypto3
}    // namespace boost

#endif    // CRYPTO3_CAST_POLICY_HPP