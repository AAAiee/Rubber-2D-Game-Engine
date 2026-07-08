#include <pch.h>
#include "Utility.h"


namespace RB{

	// initialize static class variables
	std::mt19937 RandomEngineImpl::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> RandomEngineImpl::s_Distribution;


}

