#include "Application.h"

extern GameEngine::Application GameEngine::app;

namespace GameEngine {
    LayerDataTypePtr GameEngine::m_GetLayerDataForMeshing(int cx, int cz)
    {
        return LayerDataTypePtr();
    }
}

using namespace GameEngine;

int main()
{
    GameEngine::app.RunLoop();

    return 0;
}