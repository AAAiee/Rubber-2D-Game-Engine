#pragma once
#include <Rubber/ECS/EcsCommonHeaders.h>


namespace Rubber {

	template <class Storage>
	class SimpleView {
	public:
		SimpleView(Storage& s) :m_Storage(s) {}
		auto begin() { return m_Storage.entities(); }
		auto end() { begin() + m_Storage.count(); }

	private:
		Storage& m_Storage;
	};
}
