//---------------------------------------------------------------------------//
// Copyright (c) 2018-2020 Mikhail Komarov <nemo@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_WHIRLPOOL_FUNCTIONS_HPP
#define CRYPTO3_WHIRLPOOL_FUNCTIONS_HPP

#include <boost/crypto3/detail/basic_functions.hpp>

namespace boost {
    namespace crypto3 {
        namespace hash {
            namespace detail {
                struct whirlpool_functions : public ::boost::crypto3::detail::basic_functions<64> {

                };
            }
        }
    }
}

#endif //CRYPTO3_WHIRLPOOL_FUNCTIONS_HPP