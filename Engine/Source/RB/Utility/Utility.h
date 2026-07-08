#pragma once

#define USE_STL_VECTOR 1
#define USE_STL_DEQUE 1
#define USE_STL_SMART_POINTER 1
#define USE_RANDOM_ENGINE 1

#if USE_STL_VECTOR
#include <vector>
namespace RB {
   template<typename T>
   using Vector = std::vector<T>;
}
#endif

#if USE_STL_DEQUE
#include <deque>
namespace RB {
   template<typename T>
   using Deque = std::deque<T>;
}
#endif

#if USE_STL_SMART_POINTER
#include <memory>
namespace RB {
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using weakRef = std::weak_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> makeRef(Args&&... args)
    {
        // If one day you swap in a custom memory pool,
        // you only touch this line.
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    constexpr Scope<T> makeScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
#endif
}

#if USE_RANDOM_ENGINE
#include <random>
#include <limits> 
namespace RB {
   class RandomEngineImpl {
   public:
       static void init() {
           s_RandomEngine.seed(std::random_device()());
       }

       static float Float() { 
#undef max
           return static_cast<float>(s_Distribution(s_RandomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
       }

       static void shutdown(){

       }

   private:
       static std::mt19937 s_RandomEngine;
       static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
   };
   using RandomEngine = RandomEngineImpl;
}
#endif

namespace RB::HashFunc {
    constexpr uint64_t fnv1a64(std::string_view s) noexcept
    {
        uint64_t h = 14695981039346656037ULL;          // offset basis
        for (unsigned char c : s)
            h = (h ^ c) * 1099511628211ULL;           // prime

        return h;
    }
}

