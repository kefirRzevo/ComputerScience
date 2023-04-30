#include "apiwidgets.hpp"


//----------------------------------------//

IButton*
APIWidgetFactory::CreateDefaultButtonWithIcon(const char* iconPath)
{
    return CreateButtonWithIcon(Config::defWidth, Config::defHeight, iconPath);
}

IButton*
APIWidgetFactory::CreateDefaultButtonWithText(const char* text)
{
    return CreateButtonWithText(Config::defWidth, Config::defHeight,
                                            text, Config::defTextMaxCapacity);
}

IButton*
APIWidgetFactory::CreateButtonWithIcon
(int width, int height, const char* iconPath)
{
    APIClickCallback* callback = new APIClickCallback{};
    
    Texture* buttonTexture = new Texture{Config::defColor};
    Texture* iconTexture   = TextureManager::Get()->GetTexture(iconPath);

    Vec2i defIconSize = {width, height};
    Vec2i minIconSize = {Config::minIconWidth, Config::minIconHeight};
    Vec2i maxIconSize = {Config::maxIconWidth, Config::maxIconHeight};
    
    Layout* iconLayout = new Layout{{defIconSize}, Config::defMargin,
            Config::defBorder, {minIconSize}, {maxIconSize}};
    
    Button* button = new Button{new Row{}, callback, buttonTexture};
    button->Attach(new Icon{iconLayout, iconTexture});

    return new APIButton{button, callback};
}

IButton*
APIWidgetFactory::CreateButtonWithText
(int width, int height, const char* text, int char_size)
{
    APIClickCallback* callback = new APIClickCallback{};
    
    Texture* buttonTexture = new Texture{Config::defColor};
    Texture* texture       = new Texture{Config::defIconColor};

    Vec2i defIconSize = {width, height};
    Vec2i minIconSize = {Config::minIconWidth, Config::minIconHeight};
    Vec2i maxIconSize = {Config::maxIconWidth, Config::maxIconHeight};
    
    Layout* layout = new Layout{{defIconSize}, Config::defMargin,
            Config::defBorder, {minIconSize}, {maxIconSize}};
    
    Button* button = new Button{new Row{}, callback, buttonTexture};
    Text* iconText = new Text{text};
    button->Attach(new TextIcon{layout, texture, iconText, char_size});

    return new APIButton{button, callback};
}

ISlider*
APIWidgetFactory::CreateDefaultSlider(float min, float max)
{
    return CreateSlider(Config::defHorScrollBarWidth,
                        Config::defHorScrollBarHeight,
                        static_cast<float>(Config::defHorScrollBoxWidth),
                        static_cast<float>(Config::defHorScrollBoxHeight),
                        min, max);
}

ISlider*
APIWidgetFactory::CreateSlider(int width, int height, float min, float max)
{
    return CreateSlider(width, height,
                        static_cast<float>(Config::defHorScrollBoxWidth),
                        static_cast<float>(Config::defHorScrollBoxHeight),
                        min, max);
}

ISlider*
APIWidgetFactory::CreateSlider
(int width, int height, float boxWidth, float boxHeight, float min, float max)
{
    assert(min > 0 && max > 0);

    Texture* boxTexture = Config::pluginScrollBox;
    Texture* barTexture = Config::pluginScrollBar;

    Vec2i boxSize = {static_cast<int>(boxWidth), static_cast<int>(boxHeight)};
    Vec2i defBarSize = {width, height};
    Vec2i minBarSize = {Config::minHorScrollBarWidth,
                        Config::defHorScrollBarHeight};
    Vec2i maxBarSize = {Config::maxHorScrollBarWidth,
                        Config::defHorScrollBarHeight};

    Layout* barLayout = new Layout{{defBarSize}, Config::defMargin,
            Config::defBorder, minBarSize, maxBarSize};
    Layout* boxLayout = new Layout{{boxSize}, Config::defMargin,
            Config::defBorder, boxSize, boxSize};

    APISliderCallback* callback = new APISliderCallback{};

    ScrollBox*    box = new ScrollBox{boxLayout, boxTexture};
    HorScrollBar* bar = new HorScrollBar{barLayout, barTexture, box, callback};
    APISlider* slider = new APISlider{bar, callback};
    return slider;
}

ILabel*
APIWidgetFactory::CreateDefaultLabel(const char* text)
{
    return CreateLabel(Config::defIconWidth,
                       Config::defIconHeight, text,
                       Config::defTextMaxCapacity);
}

ILabel*
APIWidgetFactory::CreateLabel
(int width, int height, const char* text, int char_size)
{
    Texture* texture = new Texture{Config::defIconColor};

    Vec2i defIconSize = {width, height};
    Vec2i minIconSize = {Config::minIconWidth, Config::minIconHeight};
    Vec2i maxIconSize = {Config::maxIconWidth, Config::maxIconHeight};
    
    Layout* layout = new Layout{{defIconSize}, Config::defMargin,
            Config::defBorder, {minIconSize}, {maxIconSize}};

    Text* iconText = new Text{text};

    TextIcon* textIcon = new TextIcon{layout, texture, iconText, char_size};
    APILabel* label = new APILabel{textIcon};
    return label;
}

IPreferencesPanel*
APIWidgetFactory::CreateDefaultPreferencesPanel()
{
    Column*  column  = new Column{};
    Texture* texture = new Texture{Config::defPanelColor};
    return new APIPreferencesPanel{new PropertiesPanel{column, texture}};
}

//----------------------------------------//

APIClickCallback::APIClickCallback(IClickCallback* callback_):
callback(callback_)
{}

void
APIClickCallback::SetCallback(IClickCallback* callback_)
{
    callback = callback_;
}

void
APIClickCallback::OnResponse()
{
    callback->RespondOnClick();
}

APISliderCallback::APISliderCallback(ISliderCallback* callback_):
callback(callback_)
{}

void
APISliderCallback::SetCallback(ISliderCallback* callback_)
{
    callback = callback_;
}

void
APISliderCallback::OnResponse(float value_)
{
    callback->RespondOnSlide(0, value_);
}

//----------------------------------------//

APIButton::APIButton(Button* button_, APIClickCallback* callback_):
button(button_), callback(callback_)
{}

Widget*
APIButton::GetBasicWidget()
{
    return button;
}

int
APIButton::GetWidth()
{
    return button->GetLayout()->GetRectangle().width;
}

int
APIButton::GetHeight()
{
    return button->GetLayout()->GetRectangle().height;
}

void
APIButton::SetClickCallback(IClickCallback* callback_)
{
    callback->SetCallback(callback_);
}

//----------------------------------------//

APISlider::APISlider(HorScrollBar* scrollBar_, APISliderCallback* callback_):
scrollBar(scrollBar_), callback(callback_)
{}

Widget*
APISlider::GetBasicWidget()
{
    return scrollBar;
}

int
APISlider::GetWidth()
{
    return scrollBar->GetLayout()->GetRectangle().width;
}
        
int
APISlider::GetHeight()
{
    return scrollBar->GetLayout()->GetRectangle().height;
}
        
float
APISlider::GetValue()
{
    return scrollBar->GetValue();
}

void
APISlider::SetValue(float value)
{
    scrollBar->SetValue(value);
}

void
APISlider::SetSliderCallback(ISliderCallback* callback_)
{
    callback->SetCallback(callback_);
}

//----------------------------------------//

APILabel::APILabel(TextIcon* textIcon_):
textIcon(textIcon_)
{}

void
APILabel::SetText(const char* text_)
{
    textIcon->SetString(text_);
}

Widget*
APILabel::GetBasicWidget()
{
    return textIcon;
}

int
APILabel::GetWidth()
{
    return textIcon->GetLayout()->GetRectangle().width;
}

int
APILabel::GetHeight()
{
    return textIcon->GetLayout()->GetRectangle().height;
}

//----------------------------------------//

APIPreferencesPanel::APIPreferencesPanel(PropertiesPanel* panel_):
panel(panel_)
{}

Widget*
APIPreferencesPanel::GetBasicWidget()
{
    return panel;
}

int
APIPreferencesPanel::GetWidth()
{
    return panel->GetLayout()->GetRectangle().width;
}

int
APIPreferencesPanel::GetHeight()
{
    return panel->GetLayout()->GetRectangle().height;
}

void
APIPreferencesPanel::Attach(IWidget* widget, int x, int y)
{
    assert(x >=0 && y >= 0);
    panel->Attach(static_cast<APIWidget*>(widget)->GetBasicWidget());
}

//----------------------------------------//
