#pragma once
#include <cstdint>
#include "RB/Utility/Utility.h"
#include <string>

namespace RB{
	enum class AssetType : uint16_t {
		None = 0ui16,
		Texture2D,
		AnimationAtlas,
		AnimationSpriteSheet
	};

	struct AssetHandle {
		uint64_t handler = INVALID_HANDLER;
		static constexpr uint64_t INVALID_HANDLER = 0ui64;
		AssetHandle() = default;
		AssetHandle(std::string_view name);
		operator bool() const noexcept;
		bool operator==(const AssetHandle& other) const noexcept;
	};

	
	class Asset {
	public:
		// UUID FOR THE FUTURE
		AssetHandle Handle;
		virtual ~Asset() {};
		virtual AssetType getType() const = 0;
	};

}


namespace std {
	template<> struct hash<RB::AssetHandle> {
		size_t operator() (const RB::AssetHandle& assetHandler) const noexcept {
			return assetHandler.handler;
		}
	};

	//template<> struct hash<RB::AssetType> {
	//	size_t operator()(const RB::AssetType& type) const noexcept {
	//		using UT = std::underlying_type_t<RB::AssetType>;
	//		return std::hash<UT>{}(static_cast<UT>(type));
	//	}
	//};
}

