#include "../plugin/plugin.hpp"

namespace plugin
{

extern "C" IPlugin* Create(IAPI* api)
{
    printf("IT's Created, baby\n");
}

extern "C" void Destroy(IPlugin* plugin)
{
    printf("IT's Destroyed, baby\n");
}

}
