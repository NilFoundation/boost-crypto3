//---------------------------------------------------------------------------//
// Copyright (c) 2018-2019 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Alexander Sokolov <asokolov@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_HASH_SPONGE_CONSTRUCTION_HPP
#define CRYPTO3_HASH_SPONGE_CONSTRUCTION_HPP

#include <boost/crypto3/detail/static_digest.hpp>
#include <boost/crypto3/detail/pack.hpp>

#include <boost/crypto3/hash/detail/nop_finalizer.hpp>

namespace boost {
    namespace crypto3 {
        namespace hashes {
            /*!
             * @brief
             * @tparam DigestEndian
             * @tparam DigestBits
             * @tparam IV
             * @tparam Compressor
             * @tparam Finalizer
             *
             * The Sponge construction builds a block hashes from a
             * one-way compressor.  As this version operated on the block
             * level, it doesn't contain any padding or other strengthening.
             * For a Wide Pipe construction, use a digest that will
             * truncate the internal state.
             */
            template<typename Params,
                     typename IV,
                     typename Compressor,
                     typename Padding,
                     typename Finalizer = detail::nop_finalizer>
            class sponge_construction {
            public:
                typedef IV iv_generator;
                typedef Compressor compressor_functor;
                typedef Padding padding_functor;
                typedef Finalizer finalizer_functor;

                typedef typename Params::digest_endian endian_type;

                constexpr static const std::size_t word_bits = compressor_functor::word_bits;
                typedef typename compressor_functor::word_type word_type;

                constexpr static const std::size_t state_bits = compressor_functor::state_bits;
                constexpr static const std::size_t state_words = compressor_functor::state_words;
                typedef typename compressor_functor::state_type state_type;

                constexpr static const std::size_t block_bits = compressor_functor::block_bits;
                constexpr static const std::size_t block_words = compressor_functor::block_words;
                typedef typename compressor_functor::block_type block_type;

                constexpr static const std::size_t digest_bits = Params::digest_bits;
                constexpr static const std::size_t digest_bytes = digest_bits / octet_bits;
                constexpr static const std::size_t digest_words = digest_bits / word_bits;
                typedef static_digest<digest_bits> digest_type;

                template<typename Integer = std::size_t>
                inline sponge_construction &process_block(const block_type &block, Integer seen = Integer()) {
                    compressor_functor::process_block(state_, block);
                    return *this;
                }

                inline digest_type digest(const block_type &block = block_type(),
                                          std::size_t total_seen = std::size_t()) {
                    using namespace boost::crypto3::detail;

                    block_type b = block;
                    std::size_t block_seen = total_seen % block_bits;
                    // Process block if it is full
                    if (total_seen && !block_seen)
                        process_block(b);

                    std::size_t copy_seen = block_seen;
                    // Pad last message block
                    padding_functor padding;
                    padding(b, block_seen);
                    process_block(b);

                    // Process additional block if not all bits were padded
                    if (!padding.is_last_block()) {
                        std::fill(b.begin(), b.end(), 0);
                        padding.process_last(b, copy_seen);
                        process_block(b);
                    }

                    // Apply finalizer
                    finalizer_functor()(state_);

                    // Convert digest to byte representation
                    std::array<octet_type, state_bits / octet_bits> d_full;
                    pack_from<endian_type, word_bits, octet_bits>(state_.begin(), state_.end(), d_full.begin());

                    digest_type d;
                    std::copy(d_full.begin(), d_full.begin() + digest_bytes, d.begin());

                    return d;
                }

                sponge_construction() {
                    reset();
                }

                void reset(state_type const &s) {
                    state_ = s;
                }

                void reset() {
                    iv_generator iv;
                    reset(iv());
                }

                state_type const &state() const {
                    return state_;
                }

            private:
                state_type state_;
            };

        }    // namespace hashes
    }        // namespace crypto3
}    // namespace boost

#endif    // CRYPTO3_HASH_SPONGE_CONSTRUCTION_HPP
