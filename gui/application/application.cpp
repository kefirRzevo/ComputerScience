#include "../widgets/widget.hpp"
#include "../widgets/button.hpp"
#include "../widgets/label.hpp"
#include "../widgets/scrollbar.hpp"
#include "../widgets/dropdownlist.hpp"

#include "../graphlib/graphlib.hpp"

#include "../gui/canvas.hpp"
#include "../gui/properties_panel.hpp"
#include "../gui/root.hpp"

#include "application.hpp"

#include "../plugin/pluginmanager.hpp"

class ButtonTest2: public ButtonResponse
{
    private:
        ScrollBar* bar;

    public:
        ButtonTest2(ScrollBar* bar_):
            bar(bar_)
    {}

    void
    OnResponse() override
    {
        bar->SetValue(0.5);
    }
};

void
App::Run()
{
    Window window{{static_cast<unsigned int>(Config::defWindowWidth),
                   static_cast<unsigned int>(Config::defWindowHeight)}, "hi"};
    Renderer::Initialize(&window);
    Renderer* rend = Renderer::Get();

    Root* root = new Root{Config::defTexture};
    WidgetSystem* syst1 = new WidgetSystem{root};

    while(1)
    {
        Event event;
        if(EventManager::Get()->PollEvent(event))
        {
            syst1->ProcessEvent(event);
            if(event.type == quit)
                break;
        }
        rend->SetColor(Config::defColor);
        rend->Clear();
        syst1->Render();
        rend->Display();
    }
}
