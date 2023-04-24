#include "../widgets/widget.hpp"
#include "../widgets/button.hpp"
#include "../widgets/label.hpp"
#include "../widgets/scrollbar.hpp"
#include "../widgets/dropdownlist.hpp"
#include "../graphlib/graphlib.hpp"
#include "application.hpp"


void
App::Run()
{
    Window window{{1024, 1024}, "hi"};
    Renderer::Initialize(&window);
    Renderer* rend = Renderer::Get();
    bool active = true;

    Texture* text1 = TextureManager::Get()->GetTexture("res/button.png");
    Texture* text2 = new Texture{{10, 10}, Blue};
    Texture* btn1t = TextureManager::Get()->GetTexture("res/btns.jpg", {40, 0, 120, 120});
    Texture* btn2t = TextureManager::Get()->GetTexture("res/btns.jpg", {40, 120, 120, 120});
    Texture* btn3t = TextureManager::Get()->GetTexture("res/btns.jpg", {40, 240, 120, 120});

    DropDownList* list1 = new DropDownList{new Column{{250, 300, 200, 400}}, text1};
    //const Font& font1 = FontManager::Get()->GetFont("res/TimesNewRoman.ttf");

    Widget* widget1 = new Widget{new Layout{{50, 50, 100, 100}}, text1};
    //Widget* widget2 = new Widget{new Column{{40, 40, 120, 80}, {10, 10}, 3}, text1};
    Widget* widget3 = new Widget{new Layout{{{50, 50}}, 2, 4, {30, 30}, {300, 150}}, text1};
    list1->Attach(widget3);
    //Widget* widget2 = new Widget{new Layout{{{50, 50}}}, text1};
    //Widget* widget4 = new Widget{new Layout{{{50, 50}}}, text1};
    //Widget* widget5 = new Widget{new Row{{10, 20, 50, 50}}, text2};
    //widget5->Attach(widget2);
    //widget5->Attach(widget4);

    //Widget* widget2 = new Widget{{70, 80}, new BorderView{text1}};
    //Widget* widget3 = new Widget{{70, 90}, new BorderView{text1}};
    Button* btn1 = new Button{new Layout{{{50, 50}}}, new ListOpenerCommand{list1}, btn1t, btn2t, btn3t};
    //TextLabel* lab1 = new TextLabel{{80, 60}, new BorderView{text1}, new TextLabelResponseTest{}, new Text{}};

    //ScrollBarResponse* scrollResp1 = new ScrollBarResponseTest();
    //ScrollBox* box1 = new ScrollBox{new Layout{{{40, 40}}}, text2};
    //ScrollBar* bar1 = new HorScrollBar{new Layout{{{200, 40}}, 2, 4, {150, 40}, {250, 40}}, text1, box1, scrollResp1};

    //ScrollBarResponse* scrollResp2 = new ScrollBarResponseTest();
    //ScrollBox* box2 = new ScrollBox{new Layout{{{40, 40}}}, text1};
    //ScrollBar* bar2 = new VerScrollBar{new Layout{{{40,200}}, 2, 4, {40, 150}, {40, 250}}, text2, box2, scrollResp2};
 
    Icon* icon1 = new Icon{new Layout{{{40, 40}}}, text2};
    list1->Attach(icon1);
    //TextIcon* txticon1 = new TextIcon{new Layout{{{40, 40}}}, text1, new Text{"hello dear"}};
    //TextLabel* txtlabel1 = new TextLabel{new Layout{{{40, 40}}}, text1, new TextLabelResponseTest{}, new Text{"hello dear"}};
    //widget1->Attach(widget2);
    //widget1->Attach(widget3);
    //widget1->Attach(btn1);
    //widget1->Attach(widget5);
    //widget1->Attach(lab1);
    //widget1->Attach(bar1);
    //widget1->Attach(bar2);
    widget1->Attach(btn1);
    widget1->Attach(list1);
    //widget1->Attach(txticon1);
    //widget1->Attach(txtlabel1);
    fprintf(stderr, "%d %d %d %d\n", widget1->GetLayout()->GetMinSize().x, widget1->GetLayout()->GetMinSize().y, widget1->GetLayout()->GetMaxSize().x, widget1->GetLayout()->GetMaxSize().y);
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
}
