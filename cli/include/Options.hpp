#include <cstdlib>
#include <string>
#include <unordered_map>

struct Options
{
    bool showUsage = false;
    bool showVersion = false;
    bool listEngine = false;
    int engineIndex = 0;
    int rendererIndex = 0;
    std::string romPath{};

    enum class ArgType
    {
        ShowUsage, ShowVersion, ListEngine, EngineIndex, RendererIndex
    };

    struct Arg
    {
        ArgType type;
        std::string desc;
    };

    const std::unordered_map<std::string, Arg> argMap =
    {
        {"--help", {ArgType::ShowUsage, "Show help information"}},
        {"--version", {ArgType::ShowVersion, "Show version information"}},
        {"--list_engine", {ArgType::ListEngine, "List engines and renderers"}},
        {"--engine_index", {ArgType::EngineIndex, "Set engine by index"}},
        {"--renderer_index", {ArgType::RendererIndex, "Set renderer by index"}},
    };

    std::string usage()
    {
        std::string info = "usage: tfcpp <options> filename\n\n";
        for (auto &&arg : argMap) info.append(arg.first + "\n  " + arg.second.desc + '\n');
        return info;
    }

    void parse(const int argc, const char* const argv[])
    {
        for (int i = 0; i < argc; i++)
        {
            if (argv[i][0] != '-')
            {
                romPath = argv[i];
                continue;
            }

            auto arg = argMap.find(argv[i]);
            if (arg == argMap.end()) continue;

            switch (arg->second.type)
            {
            case ArgType::ShowUsage:
                showUsage = true;
                break;
            case ArgType::ShowVersion:
                showVersion = true;
                break;
            case ArgType::ListEngine:
                listEngine = true;
                break;
            case ArgType::EngineIndex:
                if (i + 1 < argc) engineIndex = std::atoi(argv[++i]);
                break;
            case ArgType::RendererIndex:
                if (i + 1 < argc) rendererIndex = std::atoi(argv[++i]);
                break;
            }
        }
    }
};
