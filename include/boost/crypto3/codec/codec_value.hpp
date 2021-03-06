//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2019 Moskvin Aleksey <zerg1996@yandex.ru>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_CODEC_VALUE_HPP
#define CRYPTO3_CODEC_VALUE_HPP

#include <boost/assert.hpp>
#include <boost/concept_check.hpp>

#include <boost/range/concepts.hpp>

#include <boost/crypto3/codec/accumulators/codec.hpp>

namespace boost {
    namespace crypto3 {
        namespace codec {
            namespace detail {
                template<typename CodecAccumulatorSet>
                struct ref_codec_impl {
                    typedef CodecAccumulatorSet accumulator_set_type;
                    typedef
                        typename boost::mpl::front<typename accumulator_set_type::features_type>::type accumulator_type;

                    typedef typename accumulator_type::mode_type mode_type;
                    typedef typename mode_type::codec_type codec_type;

                    ref_codec_impl(accumulator_set_type &&acc) : accumulator_set(acc) {
                    }

                    accumulator_set_type &accumulator_set;
                };

                template<typename CodecAccumulatorSet>
                struct value_codec_impl {
                    typedef CodecAccumulatorSet accumulator_set_type;
                    typedef
                        typename boost::mpl::front<typename accumulator_set_type::features_type>::type accumulator_type;

                    typedef typename accumulator_type::mode_type mode_type;
                    typedef typename mode_type::codec_type codec_type;

                    value_codec_impl(accumulator_set_type &&acc) :
                        accumulator_set(std::forward<accumulator_set_type>(acc)) {
                    }

                    mutable accumulator_set_type accumulator_set;
                };

                template<typename CodecStateImpl>
                struct range_codec_impl : public CodecStateImpl {
                    typedef CodecStateImpl codec_state_impl_type;

                    typedef typename codec_state_impl_type::accumulator_type accumulator_type;
                    typedef typename codec_state_impl_type::accumulator_set_type accumulator_set_type;

                    typedef typename codec_state_impl_type::mode_type mode_type;
                    typedef typename codec_state_impl_type::codec_type codec_type;

                    typedef typename boost::mpl::apply<accumulator_set_type, accumulator_type>::type::result_type
                        result_type;

                    template<typename SinglePassRange>
                    range_codec_impl(const SinglePassRange &range, accumulator_set_type &&ise) :
                        CodecStateImpl(std::forward<accumulator_set_type>(ise)) {
                        BOOST_RANGE_CONCEPT_ASSERT((boost::SinglePassRangeConcept<const SinglePassRange>));

                        typedef
                            typename std::iterator_traits<typename SinglePassRange::iterator>::value_type value_type;
                        BOOST_STATIC_ASSERT(std::numeric_limits<value_type>::is_specialized);
                        typedef typename codec_type::template stream_processor<
                            mode_type, accumulator_set_type,
                            std::numeric_limits<value_type>::digits + std::numeric_limits<value_type>::is_signed>::type
                            stream_processor;

                        stream_processor(this->accumulator_set)(range.begin(), range.end());
                    }

                    template<typename InputIterator>
                    range_codec_impl(InputIterator first, InputIterator last, accumulator_set_type &&ise) :
                        CodecStateImpl(std::forward<accumulator_set_type>(ise)) {
                        BOOST_CONCEPT_ASSERT((boost::InputIteratorConcept<InputIterator>));

                        typedef typename std::iterator_traits<InputIterator>::value_type value_type;
                        BOOST_STATIC_ASSERT(std::numeric_limits<value_type>::is_specialized);
                        typedef typename codec_type::template stream_processor<
                            mode_type, accumulator_set_type,
                            std::numeric_limits<value_type>::digits + std::numeric_limits<value_type>::is_signed>::type
                            stream_processor;

                        stream_processor(this->accumulator_set)(first, last);
                    }

                    template<typename OutputRange>
                    inline operator OutputRange() const {
                        result_type result =
                            boost::accumulators::extract_result<accumulator_type>(this->accumulator_set);
                        return OutputRange(result.begin(), result.end());
                    }

                    inline operator result_type() const {
                        return boost::accumulators::extract_result<accumulator_type>(this->accumulator_set);
                    }

                    inline operator accumulator_set_type &() const {
                        return this->accumulator_set;
                    }

#ifdef CRYPTO3_ASCII_STRING_CODEC_OUTPUT

                    template<typename Char, typename CharTraits, typename Alloc>
                    inline operator std::basic_string<Char, CharTraits, Alloc>() const {
                        return std::to_string(
                            boost::accumulators::extract_result<accumulator_type>(this->accumulator_set));
                    }

#endif
                };

                template<typename CodecStateImpl, typename OutputIterator>
                struct itr_codec_impl : public CodecStateImpl {
                private:
                    mutable OutputIterator out;

                public:
                    typedef CodecStateImpl codec_state_impl_type;

                    typedef typename codec_state_impl_type::accumulator_type accumulator_type;
                    typedef typename codec_state_impl_type::accumulator_set_type accumulator_set_type;

                    typedef typename codec_state_impl_type::mode_type mode_type;
                    typedef typename codec_state_impl_type::codec_type codec_type;

                    typedef typename boost::mpl::apply<accumulator_set_type, accumulator_type>::type::result_type
                        result_type;

                    template<typename SinglePassRange>
                    itr_codec_impl(const SinglePassRange &range, OutputIterator out, accumulator_set_type &&ise) :
                        CodecStateImpl(std::forward<accumulator_set_type>(ise)), out(std::move(out)) {
                        BOOST_CONCEPT_ASSERT((boost::SinglePassRangeConcept<const SinglePassRange>));

                        typedef
                            typename std::iterator_traits<typename SinglePassRange::iterator>::value_type value_type;
                        BOOST_STATIC_ASSERT(std::numeric_limits<value_type>::is_specialized);
                        typedef typename codec_type::template stream_processor<
                            mode_type, accumulator_set_type,
                            std::numeric_limits<value_type>::digits + std::numeric_limits<value_type>::is_signed>::type
                            stream_processor;

                        stream_processor(this->accumulator_set)(range.begin(), range.end());
                    }

                    template<typename InputIterator>
                    itr_codec_impl(InputIterator first, InputIterator last, OutputIterator out,
                                   accumulator_set_type &&ise) :
                        CodecStateImpl(std::forward<accumulator_set_type>(ise)),
                        out(std::move(out)) {
                        BOOST_CONCEPT_ASSERT((boost::InputIteratorConcept<InputIterator>));

                        typedef typename std::iterator_traits<InputIterator>::value_type value_type;
                        BOOST_STATIC_ASSERT(std::numeric_limits<value_type>::is_specialized);
                        typedef typename codec_type::template stream_processor<
                            mode_type, accumulator_set_type,
                            std::numeric_limits<value_type>::digits + std::numeric_limits<value_type>::is_signed>::type
                            stream_processor;

                        stream_processor(this->accumulator_set)(first, last);
                    }

                    inline operator OutputIterator() const {
                        result_type result =
                            boost::accumulators::extract_result<accumulator_type>(this->accumulator_set);

                        return std::move(result.cbegin(), result.cend(), out);
                    }

                    inline operator accumulator_set_type &() const {
                        return this->accumulator_set;
                    }
                };
            }    // namespace detail
        }        // namespace codec
    }            // namespace crypto3
}    // namespace boost

#endif    // CRYPTO3_CODEC_VALUE_HPP
