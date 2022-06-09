#pragma once
#include <rapidjson/document.h>     // rapidjson's DOM-style API
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h> // for stringify JSON
#include "rapidjson/filewritestream.h"
#include <Scene/Scene.h>

namespace GameEngine {
	class Json
	{
	public:
		rapidjson::Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.
		Json() = default;

		int testscene();

		void SerializeScene(Scene* scene);
		Scene* DeserializeScene(std::string filename);
	};
}

