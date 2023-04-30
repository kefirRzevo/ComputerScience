#include "../widgets/label.hpp"
#include "../widgets/button.hpp"
#include "../plugin/api.hpp"
#include "../plugin/manager.hpp"
#include "canvas.hpp"


class CanvasTitle;
class Canvas;
class HorScrollBarCanvasCallback;
class VerScrollBarCanvasCallback;

class CanvasTitle: public Widget
{
    protected:

        CanvasWidget* canvasWidget; 

    public:

        CanvasTitle(const std::string& name_, int canvasWidth_, CanvasWidget* canvasWidget_):
        Widget(new Row{0, 0}, Config::defIconTexture), canvasWidget(canvasWidget_)
        {
            int height = Config::defCloseBtnHeight;

            Layout* titleLayout = new Layout{{{canvasWidth_, height}}, 0, 0,
                                            {Config::minHorScrollBarWidth, height},
                                            {Config::maxHorScrollBarWidth, height}};
            TextIcon* titleIcon = new TextIcon{titleLayout, Config::defIconTexture, new Text{name_}};

            Vec2i btnSize = {Config::defCloseBtnWidth, Config::defCloseBtnHeight};
            CloseButton* closeBtn = new CloseButton{new Layout{{btnSize}, 0, 0, btnSize, btnSize},
            canvasWidget, Config::defCloseBtnRelease, Config::defCloseBtnHover, Config::defCloseBtnPress};

            Attach(titleIcon);
            Attach(closeBtn);
        }

        bool
        ProcessListenerEvent(const Event& event_) override
        {
            if(event_.type == mouseMoved)
            {
                canvasWidget->GetLayout()->OnMove(event_.mouse.offset);
            }
            else if(event_.type == mouseReleased)
            {
                system->Unsubscribe(mouseMoved);
                system->Unsubscribe(mouseReleased);
            }
            return true;
        }

        bool
        OnEvent(const Event& event_) override
        {
            if(event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
                return false;

            if(event_.type == mousePressed)
            {
                system->Reset();
                system->Subscribe(this, mouseMoved);
                system->Subscribe(this, mouseReleased);
            }

            return true;
        }
};

class Canvas: public Widget
{
    protected:

        Texture*    fullTexture;
        APITexture  apiFullTexture;

        RectInt visiblePart;
        Vec2i   fullTextureSize;

    public:

        Canvas(Vec2i canvasSize_, Vec2i fullTextureSize_):
        Widget(new Layout{{{canvasSize_.x, canvasSize_.y}}, 0, 0,
        {Config::minHorScrollBarWidth, Config::minVerScrollBarHeight},
        {Config::maxHorScrollBarWidth, Config::maxVerScrollBarHeight}},
        TextureManager::Get()->GetTexture(Config::defCanvasColor)),
        fullTexture(new Texture{fullTextureSize_}), apiFullTexture(fullTexture),
        visiblePart({0, 0, canvasSize_.x, canvasSize_.y}),
        fullTextureSize(fullTextureSize_)
        {
            assert(fullTextureSize_.x > canvasSize_.x &&
                   fullTextureSize_.y > canvasSize_.y);
        }

        ~Canvas()
        {
            delete fullTexture;
        }

        const RectInt&
        GetVisiblePart() const
        {
            return visiblePart;
        }

        Vec2i
        GetFullTextureSize() const
        {
            return fullTextureSize;
        }

        void
        SetVisibilityBeginX(int value)
        {
            int range = fullTextureSize.x - layout->GetRectangle().width;
            if(value >= 0 && value <= range)
                visiblePart.left = value;
        }

        void
        SetVisibilityBeginY(int value)
        {
            int range = fullTextureSize.y - layout->GetRectangle().height;
            if(value >= 0 && value <= range)
                visiblePart.top = value;
        }

        bool
        ProcessListenerEvent(const Event& event_) override
        {
            if(event_.type == mouseMoved)
            {
                Vec2i begin{layout->GetRectangle().left,
                            layout->GetRectangle().top};
                Vec2i visibleBegin{visiblePart.left, visiblePart.top};
                Vec2i pos = event_.mouse.pos - begin + visibleBegin;
                Vec2i delta = event_.mouse.offset;

                ITool* active = Manager<ITool>::Get()->GetActive();
                if(active)
                    active->Action(&apiFullTexture, pos.x, pos.y, delta.x, delta.y);
            }
            else if(event_.type == mouseReleased)
            {
                Vec2i begin{layout->GetRectangle().left,
                            layout->GetRectangle().top};
                Vec2i visibleBegin{visiblePart.left, visiblePart.top};
                Vec2i pos = event_.mouse.pos - begin + visibleBegin;

                ITool* active = Manager<ITool>::Get()->GetActive();
                if(active)
                    active->ActionEnd(&apiFullTexture, pos.x, pos.y);
                
                system->Unsubscribe(mouseMoved);
                system->Unsubscribe(mouseReleased);
            }
            return true;
        }

        bool
        ProcessEvent(const Event& event_) override
        {
            if(event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
                return false;

            switch(event_.type)
            {
                case mousePressed:
                {
                    Vec2i begin{layout->GetRectangle().left,
                                layout->GetRectangle().top};
                    Vec2i visibleBegin{visiblePart.left, visiblePart.top};
                    Vec2i pos = event_.mouse.pos - begin + visibleBegin;

                    ITool* active = Manager<ITool>::Get()->GetActive();
                    if(active)
                        active->ActionBegin(&apiFullTexture, pos.x, pos.y);

                    system->Subscribe(this, mouseMoved);
                    system->Subscribe(this, mouseReleased);
                    break;
                }
                case rightButtonPress:
                {
                    IFilter* active = Manager<IFilter>::Get()->GetActive();
                    if(active)
                        active->Apply(&apiFullTexture);

                    break;
                }
                default:
                {
                    break;
                }
            }
            return true;
        }

        void
        Render() const override
        {
            Renderer::Get()->DrawTexture(texture, fullTexture, visiblePart);
            Widget::Render();
        }
};

class HorScrollBarCanvasCallback: public ScrollBarResponseTest
{
    public:

        HorScrollBarCanvasCallback(Canvas* canvas_):
        canvas(canvas_)
        {}

        void
        OnResponse(float value) override
        {
            int fullWidth    = canvas->GetFullTextureSize().x;
            int visibleWidth = canvas->GetVisiblePart().width;
            int newX = static_cast<int>(value * (fullWidth - visibleWidth));
            canvas->SetVisibilityBeginX(newX);
        }

    private:

        Canvas* canvas;
};

class VerScrollBarCanvasCallback: public ScrollBarResponseTest
{
    public:

        VerScrollBarCanvasCallback(Canvas* canvas_):
        canvas(canvas_)
        {}

        void
        OnResponse(float value) override
        {
            int fullHeight    = canvas->GetFullTextureSize().y;
            int visibleHeight = canvas->GetVisiblePart().height;
            int newY = static_cast<int>(value * (fullHeight - visibleHeight));
            canvas->SetVisibilityBeginX(newY);
        }

    private:

        Canvas* canvas;
};

CanvasWidget::CanvasWidget(Vec2i canvasSize_, Vec2i fullTextureSize_):
Widget(new Column{0, 4}, TextureManager::Get()->GetTexture(Config::defColor)),
canvasSize(canvasSize_), fullTextureSize(fullTextureSize_)
{
    Texture* defTexture = TextureManager::Get()->GetTexture(Config::defColor);

    Attach(new CanvasTitle{"Canvas", canvasSize.x, this});

    canvas = new Canvas{canvasSize, fullTextureSize};

    Layout* verLayout = new Layout{{{Config::defVerScrollBarWidth, canvasSize.y}}, 0, 0,
                                    {Config::defVerScrollBarWidth, Config::minVerScrollBarHeight},
                                    {Config::defVerScrollBarWidth, Config::maxVerScrollBarHeight}};
    Layout* verBoxLayout = new Layout{{{Config::defVerScrollBoxWidth, Config::defVerScrollBoxWidth}}, 0, 0,
                                    {Config::defVerScrollBoxWidth, 0},
                                    {Config::defVerScrollBoxWidth, Config::defVerScrollBarHeight}};
    ScrollBox* verBox = new ScrollBox{verBoxLayout, Config::defScrollBoxRelease,
                                    Config::defScrollBoxHover, Config::defScrollBoxPress};
    verScrollBar = new VerScrollBar{verLayout, Config::defVerScrollBarTexture, verBox,
                                                    new VerScrollBarCanvasCallback{canvas}};
    Widget* linker1 = new Widget{new Row{0, 0}, defTexture};
    linker1->Attach(canvas);
    linker1->Attach(verScrollBar);

    Layout* horLayout = new Layout{{{canvasSize.x, Config::defHorScrollBarHeight}}, 0, 0,
                                    {Config::minHorScrollBarWidth, Config::defHorScrollBarHeight},
                                    {Config::maxHorScrollBarWidth, Config::defHorScrollBarHeight}};
    Layout* horBoxLayout = new Layout{{{Config::defHorScrollBoxWidth, Config::defHorScrollBoxHeight}}, 0, 0,
                                    {0, Config::defHorScrollBoxHeight},
                                    {Config::defHorScrollBoxWidth, Config::defHorScrollBarHeight}};
    ScrollBox* horBox = new ScrollBox{horBoxLayout, Config::defScrollBoxRelease,
                                    Config::defScrollBoxHover, Config::defScrollBoxPress};
    horScrollBar = new HorScrollBar{horLayout, Config::defHorScrollBarTexture, horBox,
                                        new HorScrollBarCanvasCallback{canvas}};
    Layout* iconLayout = new Layout{{{Config::defVerScrollBarWidth, Config::defHorScrollBarHeight}}, 0, 0,
                                    {Config::defVerScrollBarWidth, Config::defHorScrollBarHeight},
                                    {Config::defVerScrollBarWidth, Config::defHorScrollBarHeight}};
    Icon* icon = new Icon{iconLayout, defTexture};
    Widget* linker2 = new Widget{new Row{0, 0}, defTexture};
    linker2->Attach(horScrollBar);
    linker2->Attach(icon);

    Attach(linker1);
    Attach(linker2);
    OnLayoutResize();
}

bool
CanvasWidget::ProcessListenerEvent(const Event& event_)
{
    layout->OnEvent(event_);
    if(event_.type == mouseReleased)
    {
        system->Unsubscribe(mouseMoved);
        system->Unsubscribe(mouseReleased);
    }
    return true;
}

bool
CanvasWidget::OnEvent(const Event& event_)
{
    if(event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
        return false;

    layout->OnEvent(event_);
    switch(event_.type)
    {
        case mousePressed:
        {
            system->Reset();
            system->Subscribe(this, mouseMoved);
            system->Subscribe(this, mouseReleased);
            break;
        }
        default:
        {
            break;
        }
    }
    return true;
}

void
CanvasWidget::OnLayoutResize()
{
    const RectInt& canvasRect = canvas->GetLayout()->GetRectangle();
    canvasSize = {canvasRect.width, canvasRect.height};

    int horBarLen = horScrollBar->GetLayout()->GetRectangle().width;
    int verBarLen = verScrollBar->GetLayout()->GetRectangle().height;
    int horBoxLen = canvasSize.x * canvasSize.x / fullTextureSize.x;
    int verBoxLen = canvasSize.y * canvasSize.y / fullTextureSize.y;

    if(horBoxLen < horBarLen)
        horScrollBar->SetBoxLength(horBoxLen);
    else
        horScrollBar->SetBoxLength(horBarLen);

    if(verBoxLen < verBarLen)
        verScrollBar->SetBoxLength(verBoxLen);
    else
        verScrollBar->SetBoxLength(verBarLen);
}
