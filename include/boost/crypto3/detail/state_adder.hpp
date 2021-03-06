//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_DETAIL_STATE_ADDER_HPP
#define CRYPTO3_DETAIL_STATE_ADDER_HPP

namespace boost {
    namespace crypto3 {
        namespace detail {
            struct state_adder {
                template<typename T>
                void operator()(T &s1, T const &s2) {
                    typedef typename T::size_type size_type;
                    size_type n = (s2.size() < s1.size() ? s2.size() : s1.size());
                    for (typename T::size_type i = 0; i < n; ++i) {
                        s1[i] += s2[i];
                    }
                }
            };

        }    // namespace detail
    }        // namespace crypto3
}    // namespace boost

#endif    // CRYPTO3_BLOCK_DETAIL_STATE_ADDER_HPP
