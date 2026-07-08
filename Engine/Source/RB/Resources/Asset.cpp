#include <pch.h>
#include "Asset.h"

namespace RB {

	AssetHandle::AssetHandle(std::string_view name)
	{
		uint64_t hash = HashFunc::fnv1a64(name);
		handler = (hash == 0) ? 1 : hash;
	}

	AssetHandle::operator bool() const noexcept
	{
		return handler != INVALID_HANDLER;
	}

	bool AssetHandle::operator==(const AssetHandle& other) const noexcept
	{
		return handler == other.handler;
	}

}
