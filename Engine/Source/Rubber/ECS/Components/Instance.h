#pragma once
#include <cstdint>

// TODO :: MAKE THE NAMESPACE SHORTER
namespace Rubber::Component {

    struct Instance {
        uint32_t index;
    };

    inline Instance makeInstance(uint32_t index) { 
        return Instance{ index };
    }

    // Fix for E2486 and C4244 errors: Ensure the type is explicitly defined as uint8_t and avoid any ambiguity.
    inline constexpr uint8_t RESCALE_FACTOR = 2ui8;

}