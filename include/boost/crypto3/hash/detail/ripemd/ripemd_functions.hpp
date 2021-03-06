//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Nikita Kaskov <nbering@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_RIPEMD_FUNCTIONS_HPP
#define CRYPTO3_RIPEMD_FUNCTIONS_HPP

#include <boost/crypto3/hash/detail/ripemd/ripemd_policy.hpp>

namespace boost {
    namespace crypto3 {
        namespace hashes {
            namespace detail {
                template<std::size_t DigestBits>
                struct ripemd_functions : public ripemd_policy<DigestBits> {
                    typedef ripemd_policy<DigestBits> policy_type;

                    typedef typename policy_type::word_type word_type;

                    struct f1 {
                        inline word_type operator()(word_type x, word_type y, word_type z) const {
                            return x ^ y ^ z;
                        }
                    };

                    struct f2 {
                        inline word_type operator()(word_type x, word_type y, word_type z) const {
                            return (x & y) | (~x & z);
                        }
                    };

                    struct f3 {
                        inline word_type operator()(word_type x, word_type y, word_type z) const {
                            return (x | ~y) ^ z;
                        }
                    };

                    struct f4 {
                        inline word_type operator()(word_type x, word_type y, word_type z) const {
                            return (x & z) | (y & ~z);
                        }
                    };

                    struct f5 {
                        inline word_type operator()(word_type x, word_type y, word_type z) const {
                            return x ^ (y | ~z);
                        }
                    };

                    template<typename F>
                    inline static void transform(word_type &a, word_type &b, word_type &c, word_type &d, word_type x,
                                                 word_type k, word_type s) {
                        word_type T = policy_type::rotl(a + F()(b, c, d) + x + k, s);
                        a = d;
                        d = c;
                        c = b;
                        b = T;
                    }

                    template<typename Functor>
                    inline static void transform(word_type &a, word_type &b, word_type &c, word_type &d, word_type &e,
                                                 word_type x, word_type k, word_type s) {
                        word_type T = policy_type::rotl(a + Functor()(b, c, d) + x + k, s) + e;
                        a = e;
                        e = d;
                        d = policy_type::template rotl<10>(c);
                        c = b;
                        b = T;
                    }
                };
            }    // namespace detail
        }        // namespace hashes
    }            // namespace crypto3
}    // namespace boost

#endif    // CRYPTO3_RIPEMD_FUNCTIONS_HPP
