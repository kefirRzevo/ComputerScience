#include "root.hpp"
#include "../plugin/pluginmanager.hpp"
#include "../plugin/apiwidgets.hpp"
#include "../plugin/manager.hpp"

using namespace plugin;

//Manager<ITool*>* toolManager = Manager<ITool*>::Get();


//class MainTitleBar: public Widget
//{
  //  public:

        //MainTitleBar();
//}

Root::Root(Texture* texture_):
Widget(new Layout{{{static_cast<int>(Config::windowWidth),
                    static_cast<int>(Config::windowHeight)}}}, texture_)
{}

//----------------------------------------//
