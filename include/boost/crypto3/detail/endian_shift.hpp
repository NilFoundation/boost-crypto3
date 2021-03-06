//---------------------------------------------------------------------------//
// Copyright (c) 2018-2019 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Nikita Kaskov <nbering@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_DETAIL_ENDIAN_SHIFT_HPP
#define CRYPTO3_DETAIL_ENDIAN_SHIFT_HPP

#include <boost/assert.hpp>

#include <boost/crypto3/detail/stream_endian.hpp>
#include <boost/crypto3/detail/basic_functions.hpp>
#include <boost/crypto3/detail/unbounded_shift.hpp>

namespace boost {
    namespace crypto3 {
        namespace detail {

            template<typename Endianness, std::size_t WordBits>
            struct endian_shift;

            template<int UnitBits, std::size_t WordBits>
            struct endian_shift<stream_endian::big_unit_big_bit<UnitBits>, WordBits>
                : public basic_functions<WordBits> {

                constexpr static const std::size_t word_bits = basic_functions<WordBits>::word_bits;
                typedef typename basic_functions<WordBits>::word_type word_type;

                static word_type &to_msb(word_type &w, std::size_t shift) {
                    // shift to most significant bits according to endianness
                    w = unbounded_shl(w, shift);
                    return w;
                }
            };

            template<int UnitBits, std::size_t WordBits>
            struct endian_shift<stream_endian::little_unit_big_bit<UnitBits>, WordBits>
                : public basic_functions<WordBits> {

                constexpr static const std::size_t word_bits = basic_functions<WordBits>::word_bits;
                typedef typename basic_functions<WordBits>::word_type word_type;

                static word_type &to_msb(word_type &w, std::size_t shift) {
                    // shift to most significant bits according to endianness
                    std::size_t shift_rem = shift % UnitBits;
                    std::size_t shift_unit_bits = shift - shift_rem;

                    std::size_t sz[2] = {UnitBits - shift_rem, shift_rem};
                    word_type masks[2];
                    masks[0] = unbounded_shl(low_bits<word_bits>(~word_type(), sz[0]), shift_unit_bits);
                    masks[1] =
                        unbounded_shl(low_bits<word_bits>(~word_type(), sz[1]), shift_unit_bits + UnitBits + sz[0]);
                    std::size_t bits_left = word_bits - shift;

                    word_type w_combined = 0;
                    int ind = 0;

                    while (bits_left) {
                        w_combined |= (!ind ? unbounded_shl(w & masks[0], shift_rem) :
                                              unbounded_shr(w & masks[1], UnitBits + sz[0]));
                        bits_left -= sz[ind];
                        masks[ind] = unbounded_shl(masks[ind], UnitBits);
                        ind = 1 - ind;
                    }

                    w = unbounded_shr(w_combined, shift_unit_bits);
                    return w;
                }
            };

            template<int UnitBits, std::size_t WordBits>
            struct endian_shift<stream_endian::big_unit_little_bit<UnitBits>, WordBits>
                : public basic_functions<WordBits> {

                constexpr static const std::size_t word_bits = basic_functions<WordBits>::word_bits;
                typedef typename basic_functions<WordBits>::word_type word_type;

                static word_type &to_msb(word_type &w, std::size_t shift) {
                    // shift to most significant bits according to endianness
                    std::size_t shift_rem = shift % UnitBits;
                    std::size_t shift_unit_bits = shift - shift_rem;

                    std::size_t sz[2] = {UnitBits - shift_rem, shift_rem};
                    word_type masks[2] = {
                        unbounded_shr(high_bits<word_bits>(~word_type(), sz[0]), shift_unit_bits),
                        unbounded_shr(high_bits<word_bits>(~word_type(), sz[1]), shift_unit_bits + UnitBits + sz[0])};

                    std::size_t bits_left = word_bits - shift;
                    word_type w_combined = 0;
                    int ind = 0;

                    while (bits_left) {
                        w_combined |= (!ind ? unbounded_shr(w & masks[0], shift_rem) :
                                              unbounded_shl(w & masks[1], UnitBits + sz[0]));
                        bits_left -= sz[ind];
                        masks[ind] = unbounded_shr(masks[ind], UnitBits);
                        ind = 1 - ind;
                    }

                    w = unbounded_shl(w_combined, shift_unit_bits);
                    return w;
                }
            };

            template<int UnitBits, std::size_t WordBits>
            struct endian_shift<stream_endian::little_unit_little_bit<UnitBits>, WordBits>
                : public basic_functions<WordBits> {

                constexpr static const std::size_t word_bits = basic_functions<WordBits>::word_bits;
                typedef typename basic_functions<WordBits>::word_type word_type;

                static word_type &to_msb(word_type &w, std::size_t shift) {

                    // shift to most significant bits according to endianness
                    w = unbounded_shr(w, shift);
                    return w;
                }
            };
        }    // namespace detail
    }        // namespace crypto3
}    // namespace boost

#endif    // CRYPTO3_DETAIL_ENDIAN_SHIFT_HPP
