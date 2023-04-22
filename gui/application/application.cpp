#include "../gui/widget.hpp"
#include "../gui/button.hpp"
//#include "../gui/label.hpp"
//#include "../gui/scrollbar.hpp"
#include "../graphlib/graphlib.hpp"
#include "application.hpp"


void
App::Run()
{
    Window window{{1024, 1024}, "hi"};
    Renderer::Initialize(&window);
    Renderer* rend = Renderer::Get();
    bool active = true;

    Texture* text1 = new Texture{"res/button.png"};
    Texture* text2 = new Texture{Blue};
    Texture* btn1t = new Texture{"res/btns.jpg", {40, 0, 120, 120}};
    Texture* btn2t = new Texture{"res/btns.jpg", {40, 120, 120, 120}};
    Texture* btn3t = new Texture{"res/btns.jpg", {40, 240, 120, 120}};
    const Font& font1 = FontManager::Get()->GetFont("res/TimesNewRoman.ttf");

    Widget* widget1 = new Widget{new Column{{50, 50, 50, 50}}, text1};
    //Widget* widget2 = new Widget{new Column{{40, 40, 120, 80}, {10, 10}, 3}, text1};
    Widget* widget3 = new Widget{new Layout{{{50, 50}}, 2, 2, {30, 30}, {150, 150}}, text1};
    Widget* widget2 = new Widget{new Layout{{{50, 50}}}, text1};
    Widget* widget4 = new Widget{new Layout{{{50, 50}}}, text1};
    Widget* widget5 = new Widget{new Row{{10, 20, 50, 50}}, text2};
    widget5->Attach(widget2);
    widget5->Attach(widget4);

    //Widget* widget2 = new Widget{{70, 80}, new BorderView{text1}};
    //Widget* widget3 = new Widget{{70, 90}, new BorderView{text1}};
    Button* btn1 = new Button{new Layout{{{50, 50}}}, new CloseCommand{widget3}, btn1t, btn2t, btn3t};
    //TextLabel* lab1 = new TextLabel{{80, 60}, new BorderView{text1}, new TextLabelResponseTest{}, new Text{}};

    //ScrollBarResponse* scrollResp1 = new ScrollBarResponseTest();
    //ScrollBox* box1 = new ScrollBox{{40, 40}, new BorderView{text1}};
    //ScrollBar* bar1 = new HorScrollBar{{200, 40}, new BorderView{text1}, scrollResp1, box1};

    //widget1->Attach(widget2);
    widget1->Attach(widget3);
    //widget1->Attach(btn1);
    widget1->Attach(widget5);
    //widget1->Attach(lab1);
    //widget1->Attach(bar1);
    //widget1->Attach(btn1);
    WidgetSystem* syst1 = new WidgetSystem{widget1};

    while(active)
    {
        Event event;
        if(EventManager::Get()->PollEvent(event))
        {
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
