  #include "Utils/Json.h"

namespace GameEngine {

    int Json::testscene()
    {
        // 1. Parse a JSON text string to a document.
        const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
        printf("Original JSON:\n %s\n", json);


        if (document.Parse(json).HasParseError())
            return 1;

        FILE* fp = fopen("output.json", "wb"); // non-Windows use "w"

        char writeBuffer[655];
        rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

        rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
        document.Accept(writer);

        fclose(fp);

        return 0;
    }

    void Json::SerializeScene(Scene* scene)
    {
        std::string filename = scene->getName() + ".json";
        FILE* fp = fopen(filename.c_str(), "wb"); // non-Windows use "w"
        std::string file = "{";
        file += "\"name\" : \"" + scene->getName() + "\"";
        file += "}";
        if (document.Parse(file.c_str()).HasParseError())
            printf("s");

        char writeBuffer[655];
        rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

        rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
        document.Accept(writer);

        fclose(fp);
    }

    Scene* Json::DeserializeScene(std::string filename)
    {
        return nullptr;
    }

}