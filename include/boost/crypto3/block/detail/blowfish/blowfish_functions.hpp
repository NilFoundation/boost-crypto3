//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_BLOWFISH_FUNCTIONS_CPP_HPP
#define CRYPTO3_BLOWFISH_FUNCTIONS_CPP_HPP

#include <boost/crypto3/detail/basic_functions.hpp>

namespace boost {
    namespace crypto3 {
        namespace block {
            namespace detail {
                template<std::size_t WordBits>
                struct blowfish_functions : public ::boost::crypto3::detail::basic_functions<WordBits> {
                    typedef ::boost::crypto3::detail::basic_functions<WordBits> policy_type;
                    typedef typename policy_type::word_type word_type;

                    constexpr static const std::size_t constants_size = 256;
                    typedef std::array<word_type, constants_size> constants_type;

                    constexpr static const std::size_t plain_constants_size = constants_size * 4;
                    typedef std::array<word_type, plain_constants_size> plain_constants_type;

                    inline static word_type bff(word_type X, const plain_constants_type &constants) {
                        return ((constants[::boost::crypto3::detail::extract_uint_t<CHAR_BIT>(X, 0)] +
                                 constants[256 + ::boost::crypto3::detail::extract_uint_t<CHAR_BIT>(X, 1)]) ^
                                constants[512 + ::boost::crypto3::detail::extract_uint_t<CHAR_BIT>(X, 2)]) +
                               constants[768 + ::boost::crypto3::detail::extract_uint_t<CHAR_BIT>(X, 3)];
                    }
                };
            }    // namespace detail
        }        // namespace block
    }            // namespace crypto3
}    // namespace boost

#endif    // CRYPTO3_BLOWFISH_FUNCTIONS_CPP_HPP
