#include "GUI/Constraints.h"


namespace GameEngine {

	Constraints::Constraints(glm::vec2 newXdist, glm::vec2 newYdist, bool newXlock, bool newYlock)
		: Xrange(newXdist), Yrange(newYdist), Xlock(newXlock), Ylock(newYlock)
	{
		Xdist = Xrange.y - Xrange.x;
		Ydist = Yrange.y - Yrange.x;
	}

}