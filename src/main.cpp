#include "Application.h"

extern GameEngine::Application GameEngine::app;

namespace GameEngine {
    ChunkDataTypePtr GameEngine::m_GetChunkDataForMeshing(int cx, int cz)
    {
        return ChunkDataTypePtr();
    }
}

using namespace GameEngine;

int main()
{
    GameEngine::app.RunLoop();

    return 0;
}