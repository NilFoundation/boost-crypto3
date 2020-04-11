//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_SEED_FUNCTIONS_CPP_HPP
#define CRYPTO3_SEED_FUNCTIONS_CPP_HPP

#include <boost/endian/arithmetic.hpp>

#include <boost/crypto3/detail/basic_functions.hpp>

namespace boost {
    namespace crypto3 {
        namespace block {
            namespace detail {
                template<std::size_t WordBits>
                struct seed_functions : public ::boost::crypto3::detail::basic_functions<WordBits> {
                    typedef typename ::boost::crypto3::detail::basic_functions<WordBits>::word_type word_type;

                    constexpr static const std::size_t constants_size = 256;
                    typedef std::array<word_type, constants_size> constants_type;

                    inline static word_type g(word_type X, const constants_type &s0, const constants_type &s1,
                                              const constants_type &s2, const constants_type &s3) {
                        return (s0[::boost::crypto3::detail::extract_uint_t<CHAR_BIT>(X, 3)] ^
                                s1[::boost::crypto3::detail::extract_uint_t<CHAR_BIT>(X, 2)] ^
                                s2[::boost::crypto3::detail::extract_uint_t<CHAR_BIT>(X, 1)] ^
                                s3[::boost::crypto3::detail::extract_uint_t<CHAR_BIT>(X, 0)]);
                    }
                };
            }    // namespace detail
        }        // namespace block
    }            // namespace crypto3
}    // namespace boost

#endif    // CRYPTO3_SEED_FUNCTIONS_CPP_HPP
