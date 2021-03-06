//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_SECURE_ALLOCATOR_HPP
#define CRYPTO3_SECURE_ALLOCATOR_HPP

#include <boost/crypto3/block/detail/utilities/memory_operations.hpp>

namespace boost {
    namespace crypto3 {
        template<typename T>
        class secure_allocator {
        public:
            /*
             * Assert exists to prevent someone from doing something that will
             * probably crash anyway (like secure_vector<non_POD_t> where ~non_POD_t
             * deletes a member pointer which was zeroed before it ran).
             * MSVC in debug mode uses non-integral proxy types in container types
             * like std::vector, thus we disable the check there.
             */
#if !defined(_ITERATOR_DEBUG_LEVEL) || _ITERATOR_DEBUG_LEVEL == 0
            static_assert(std::is_integral<T>::value, "secure_allocator supports only integer types");
#endif

            typedef T value_type;
            typedef std::size_t size_type;

#ifdef CRYPTO3_BUILD_COMPILER_IS_MSVC_2013
            secure_allocator() = default;
            secure_allocator(const secure_allocator &) = default;
            secure_allocator &operator=(const secure_allocator &) = default;
            ~secure_allocator() = default;

            template<typename U>
            struct rebind {
                typedef secure_allocator<U> other;
            };

            void construct(value_type *mem, const value_type &value) {
                std::_Construct(mem, value);
            }

            void destroy(value_type *mem) {
                std::_Destroy(mem);
            }
#else

            secure_allocator() BOOST_NOEXCEPT = default;

            secure_allocator(const secure_allocator &) BOOST_NOEXCEPT = default;

            secure_allocator &operator=(const secure_allocator &) BOOST_NOEXCEPT = default;

            ~secure_allocator() BOOST_NOEXCEPT = default;

#endif

            template<typename U>
            secure_allocator(const secure_allocator<U> &) BOOST_NOEXCEPT {
            }

            T *allocate(std::size_t n) {
                return static_cast<T *>(allocate_memory(n, sizeof(T)));
            }

            void deallocate(T *p, std::size_t n) {
                deallocate_memory(p, n, sizeof(T));
            }
        };

        template<typename T, typename U>
        inline bool operator==(const secure_allocator<T> &, const secure_allocator<U> &) {
            return true;
        }

        template<typename T, typename U>
        inline bool operator!=(const secure_allocator<T> &, const secure_allocator<U> &) {
            return false;
        }
    }    // namespace crypto3
}    // namespace boost

#endif    // CRYPTO3_SECURE_ALLOCATOR_HPP
