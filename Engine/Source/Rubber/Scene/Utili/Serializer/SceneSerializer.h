#pragma once
#include <Rubber/Utility/Utility.h>

namespace Rubber {

	class Scene;


	class SceneSerializer {


	public:
		SceneSerializer(const Ref<Scene>& scene);
		~SceneSerializer() = default;

		void serialize(const std::filesystem::path& filepath);
		void serializeRunTime(const std::filesystem::path& filepath);
		

		bool deserialize(const std::filesystem::path& filepath)	;
		bool deserializeRunTime(const std::filesystem::path& filepath);


	private:
		Ref<Scene> m_Context;
	};
	
}
