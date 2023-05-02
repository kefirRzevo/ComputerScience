#include "../widgets/label.hpp"
#include "../widgets/button.hpp"
#include "../plugin/api.hpp"
#include "../plugin/manager.hpp"
#include "canvas.hpp"


class CanvasTitle;
class Canvas;
class HorScrollBarCanvasCallback;
class VerScrollBarCanvasCallback;

//----------------------------------------//

class CanvasTitle: public Widget
{
    protected:

        CanvasWidget* canvasWidget; 

    public:

        CanvasTitle(TextIcon* titleIcon_, CloseButton* closeButton_,
        CanvasWidget* canvasWidget_):
        Widget(new Row{0, 0}, Config::defUpReleaseTexture), canvasWidget(canvasWidget_)
        {
            Attach(titleIcon_);
            Attach(closeButton_);
        }

        static CanvasTitle*
        GetDefault(const std::string& name_, CanvasWidget* canvasWidget_)
        {
            Vec2i def = {Config::defCanvasWidth, Config::defIconHeight};
            Vec2i min = {Config::defMinCanvasWidth, Config::defIconHeight};
            Vec2i max = {Config::defMaxCanvasWidth, Config::defIconHeight};
            Layout* iconLayout = new Layout{{def}, 0, 0, min, max};

            Texture* tex = Config::defUpReleaseTexture;
            TextIcon* icon = new TextIcon{iconLayout, new Text{name_}, tex};

            CloseButton* closeButton = CloseButton::GetDefault(canvasWidget_);

            return new CanvasTitle{icon, closeButton, canvasWidget_};
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

        CanvasWidget* canvasWidget;
        Texture*      fullTexture;
        APITexture    apiFullTexture;

        RectInt visiblePart;
        Vec2i   fullTextureSize;

    public:

        Canvas(Layout* layout, Vec2i fullTextureSize_, CanvasWidget* canvasWidget_):
        Widget(layout, nullptr),
        canvasWidget(canvasWidget_),
        fullTexture(nullptr),
        apiFullTexture(nullptr),
        fullTextureSize(fullTextureSize_)
        {
            const RectInt& rect = layout->GetRectangle();
            visiblePart = {0, 0, rect.width, rect.height};

            Renderer* rend = Renderer::Get();
            
            Texture* defTexture = Config::defCanvasTexture;
            Vec2i defTextureSize = {defTexture->GetWidth(), defTexture->GetHeight()};

            Vec2i textureSize = {Config::defWindowWidth, Config::defWindowHeight};

            texture = new Texture{textureSize};
            fullTexture = new Texture{fullTextureSize};

            rend->DrawTexture(texture, Config::defCanvasTexture);
            rend->DrawTexture(fullTexture, Config::defCanvasTexture);
            apiFullTexture.SetTexture(fullTexture);

            assert(fullTextureSize.x > rect.width &&
                   fullTextureSize.y > rect.height);
        }

        static Canvas*
        GetDefault(Vec2i fullTextureSize_, CanvasWidget* canvasWidget_)
        {
            Vec2i def = {Config::defCanvasWidth, Config::defCanvasHeight};
            Vec2i min = {Config::defMinCanvasWidth, Config::defMinCanvasHeight};
            Vec2i max = {Config::defMaxCanvasWidth, Config::defMaxCanvasHeight};
            Layout* layout = new Layout{{def}, 0, 0, min, max};

            return new Canvas{layout, fullTextureSize_, canvasWidget_};
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
            int range = fullTextureSize.x - visiblePart.width;
            if(value >= 0 && value <= range)
                visiblePart.left = value;
        }

        void
        SetVisibilityBeginY(int value)
        {
            int range = fullTextureSize.y - visiblePart.height;
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

        void
        OnLayoutResize() override
        {
            const RectInt& rect = layout->GetRectangle();

            visiblePart.width  = rect.width;
            visiblePart.height = rect.height;

            if(visiblePart.left + rect.width > fullTextureSize.x)
                visiblePart.left = fullTextureSize.x - rect.width;

            if(visiblePart.top + rect.height > fullTextureSize.y)
                visiblePart.top = fullTextureSize.y - rect.height;

            canvasWidget->OnLayoutResize();
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
            int fullWidth = canvas->GetFullTextureSize().x;
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
            canvas->SetVisibilityBeginY(newY);
        }

    private:

        Canvas* canvas;
};

//----------------------------------------//

CanvasWidget::CanvasWidget(const std::string& name_, Vec2i fullTextureSize_):
Widget(new Column{0, 4}, Config::defUpReleaseTexture),
fullTextureSize(fullTextureSize_)
{
    canvasSize = {Config::defCanvasWidth, Config::defCanvasHeight};
    canvas = Canvas::GetDefault(fullTextureSize_, this);

    VerScrollBarCanvasCallback* ver = new VerScrollBarCanvasCallback{canvas};
    verScrollBar = VerScrollBar::GetDefault(ver);
    HorScrollBarCanvasCallback* hor = new HorScrollBarCanvasCallback{canvas};
    horScrollBar = HorScrollBar::GetDefault(hor);

    Texture* defTexture = Config::defDownReleaseTexture;

    Attach(CanvasTitle::GetDefault(name_, this));

    Widget* linker1 = new Widget{new Row{0, 0}, defTexture};
    linker1->Attach(canvas);
    linker1->Attach(verScrollBar);

    Widget* linker2 = new Widget{new Row{0, 0}, defTexture};
    linker2->Attach(horScrollBar);
    linker2->Attach(Icon::GetDefault(defTexture));

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

//----------------------------------------//
