#pragma  once
#include "Rubber/ECS/Components/ComponentTemplate.h"



namespace Rubber{

	struct testComponentTypes{
		using Types = std::tuple<
			glm::vec3,
			float,
			glm::vec3,
			glm::quat>;

		enum class Order : std::size_t{
			POS = 0,
			MASS = 1,
			SCALE = 2,
			ORIENTATION = 3,
		};
	};

	class testComponent: public ComponentManager<testComponentTypes>{

	public:


	private:

	};



}