/*
 *  parameters.hpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <cstddef>

namespace sapphire {
    namespace limits {
        constexpr std::size_t array_max{250};
        constexpr std::size_t max_leaves{12};
        constexpr char terminal_delimiter{'\\'};
    }
}

#endif // end PARAMETERS_HPP
