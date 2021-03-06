//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2019 Aleksey Moskvin <zerg1996@yandex.ru>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_VARLENGTH_BLOCK_STREAM_PROCESSOR_HPP
#define CRYPTO3_VARLENGTH_BLOCK_STREAM_PROCESSOR_HPP

#include <array>
#include <iterator>

#include <boost/crypto3/detail/pack.hpp>
#include <boost/crypto3/detail/digest.hpp>

#include <boost/integer.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/range/algorithm/copy.hpp>

namespace boost {
    namespace crypto3 {
        namespace codec {
            template<typename Mode, typename StateAccumulator, typename Params>
            struct varlength_block_stream_processor {
            private:
                typedef Mode mode_type;
                typedef StateAccumulator accumulator_type;
                typedef Params params_type;

                constexpr static const std::size_t input_block_bits = mode_type::input_block_bits;
                typedef typename mode_type::input_block_type input_block_type;

                constexpr static const std::size_t input_value_bits = mode_type::input_value_bits;
                typedef typename input_block_type::value_type input_value_type;

                constexpr static const std::size_t output_block_bits = mode_type::output_block_bits;
                typedef typename mode_type::output_block_type output_block_type;

                constexpr static const std::size_t output_value_bits = mode_type::output_value_bits;
                typedef typename output_block_type::value_type output_value_type;

            public:
                typedef typename params_type::endian_type endian_type;

                constexpr static const std::size_t value_bits = params_type::value_bits;
                typedef typename boost::uint_t<value_bits>::least value_type;
                BOOST_STATIC_ASSERT(input_block_bits % value_bits == 0);
                constexpr static const std::size_t block_values = input_block_bits / value_bits;
                typedef std::array<value_type, block_values> value_array_type;

            private:
                constexpr static const std::size_t length_bits = params_type::length_bits;
                // FIXME: do something more intelligent than capping at 64
                constexpr static const std::size_t length_type_bits =
                    length_bits < input_block_bits ? input_block_bits : length_bits > 64 ? 64 : length_bits;
                typedef typename boost::uint_t<length_type_bits>::least length_type;

                BOOST_STATIC_ASSERT(!length_bits || length_bits % input_block_bits == 0);
                BOOST_STATIC_ASSERT(output_block_bits % value_bits == 0);

                BOOST_STATIC_ASSERT(!length_bits || value_bits <= length_bits);

            public:
                varlength_block_stream_processor(StateAccumulator &s) : state(s) {
                }

                template<typename InputIterator>
                inline void operator()(InputIterator first, InputIterator last, std::random_access_iterator_tag) {
                    input_block_type block =
                        {};    // TODO: fill it with zero value for base32/64, and find true size for base58
                    ::boost::crypto3::detail::pack_to<endian_type, value_bits, input_value_bits>(
                            first, last, std::inserter(block, block.begin()));
                    state(block);
                }

                template<typename InputIterator, typename Category>
                inline void operator()(InputIterator first, InputIterator last, Category) {
                    input_block_type block = {0};
                    ::boost::crypto3::detail::pack_to<endian_type, value_bits, input_value_bits>(
                            first, last, block.begin());
                    state(block);
                }

                template<typename ValueType,
                         typename = typename std::enable_if<std::is_same<ValueType, input_value_type>::value>::type>
                inline void operator()(const ValueType &value) {
                    state(value);
                }

                template<typename InputIterator>
                inline void operator()(InputIterator first, InputIterator last) {
                    typedef typename std::iterator_traits<InputIterator>::iterator_category cat;
                    return operator()(first, last, cat());
                }

                template<typename ValueType>
                inline void operator()(const std::initializer_list<ValueType> &il) {
                    return operator()(il.begin(), il.end());
                }

                void reset() {
                }

                StateAccumulator &state;
            };
        }    // namespace codec
    }    // namespace crypto3
}    // namespace boost

#endif    // CRYPTO3_FIXED_BLOCK_STREAM_PROCESSOR_HPP
