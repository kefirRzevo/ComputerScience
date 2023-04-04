#include "../gui/widget.hpp"
#include "../gui/button.hpp"
#include "../gui/label.hpp"
#include "../graphlib/graphlib.hpp"
#include "application.hpp"

void
App::Run()
{
    Window window{{1024, 1024}, "hi"};
    Renderer* rend = Renderer::Get(&window);
    bool active = true;

    Texture* text1 = new Texture{"res/button.png"};
    Texture* text2 = new Texture{"res/bunny.png"};
    Texture* btn1t = new Texture{"res/btns.jpg", {40, 0, 120, 120}};
    Texture* btn2t = new Texture{"res/btns.jpg", {40, 120, 120, 120}};
    Texture* btn3t = new Texture{"res/btns.jpg", {40, 240, 120, 120}};
    const Font& font1 = FontManager::Get()->GetFont("res/TimesNewRoman.ttf");

    Widget* widget1 = new Widget{{20, 40, 60, 70}, text1};
    Widget* widget2 = new Widget{{70, 80}, text1};
    Widget* widget3 = new Widget{{70, 90}, text1};
    Button* btn1 = new Button{{50, 50}, new CloseCommand{widget2}, btn1t, btn2t, btn3t};
    Label* lab1 = new Label{{80, 60}, Blue, font1, "AAA"};
    widget1->Attach(widget2);
    widget1->Attach(widget3);
    widget1->Attach(btn1);
    widget1->Attach(lab1);
    //widget1->Attach(btn1);
    WidgetSystem* syst1 = new WidgetSystem{widget1};

    while(active)
    {
        Event event;
        if(EventManager::Get()->PollEvent(event))
        {
            //printf("|%d|\n", event.type);
            syst1->ProcessEvent(event);
            if(event.type == quit)
                break;
        }
        rend->SetColor(White);
        rend->Clear();
        syst1->Render();
        rend->Display();
    }
    delete text1;
}
