#include "apiwidgets.hpp"


//----------------------------------------//

IButton*
APIWidgetFactory::CreateDefaultButtonWithIcon(const char* iconPath_)
{
    return CreateButtonWithIcon(Config::defAPI_IconButtonWidth,
                                Config::defAPI_IconButtonHeight, iconPath_);
}

IButton*
APIWidgetFactory::CreateDefaultButtonWithText(const char* text_)
{
    return CreateButtonWithText(Config::defAPI_TextButtonWidth,
                                Config::defAPI_TextButtonHeight, text_,
                                Config::defTextMaxCapacity);
}

IButton*
APIWidgetFactory::CreateButtonWithIcon
(int width_, int height_, const char* iconPath_)
{
    APIClickCallback* callback = new APIClickCallback{};

    Texture* release = Config::defAPIReleaseTexture;
    Texture* hover   = Config::defAPIHoverTexture;
    Texture* press   = Config::defAPIPressTexture;

    Button* button = new Button{new Row{}, callback, release, hover, press};

    Vec2i defIconSize = {width_,         height_        };
    Vec2i minIconSize = {width_ * 3 / 4, height_ * 3 / 4};
    Vec2i maxIconSize = {width_ * 5 / 4, height_ * 5 / 4};
    
    Layout* iconLayout = new Layout{{defIconSize}, Config::defMargin,
                        Config::defBorder, {minIconSize}, {maxIconSize}};
    Texture* iconTexture = TextureManager::Get()->GetTexture(iconPath_);
    button->Attach(new Icon{iconLayout, iconTexture});

    return new APIButton{button, callback};
}

IButton*
APIWidgetFactory::CreateButtonWithText
(int width_, int height_, const char* text_, int char_size_)
{
    APIClickCallback* callback = new APIClickCallback{};
    
    Texture* release = Config::defAPIReleaseTexture;
    Texture* hover   = Config::defAPIHoverTexture;
    Texture* press   = Config::defAPIPressTexture;

    Button* button = new Button{new Row{}, callback, release, hover, press};

    Vec2i defIconSize = {width_,         height_        };
    Vec2i minIconSize = {width_ * 3 / 4, height_ * 3 / 4};
    Vec2i maxIconSize = {width_ * 5 / 4, height_ * 5 / 4};
    
    Layout* layout = new Layout{{defIconSize}, Config::defMargin,
                        Config::defBorder, {minIconSize}, {maxIconSize}};
    Text* iconText = new Text{text_};
    button->Attach(new TextIcon{layout, iconText, nullptr, char_size_});

    return new APIButton{button, callback};
}

ISlider*
APIWidgetFactory::CreateDefaultSlider(float min_, float max_)
{
    return CreateSlider(Config::defAPI_SliderWidth,
                        Config::defAPI_SliderHeight,
                        static_cast<float>(Config::defAPI_SliderBoxWidth),
                        static_cast<float>(Config::defAPI_SliderBoxHeight),
                        min_, max_);
}

ISlider*
APIWidgetFactory::CreateSlider(int width_, int height_, float min_, float max_)
{
    return CreateSlider(width_, height_,
                        static_cast<float>(Config::defAPI_SliderBoxWidth),
                        static_cast<float>(Config::defAPI_SliderBoxHeight),
                        min_, max_);
}

ISlider*
APIWidgetFactory::CreateSlider
(int width_, int height_, float boxWidth_, float boxHeight_, float min_, float max_)
{
    Texture* boxTexture = Config::defPluginScrollBox;
    Texture* barTexture = Config::defPluginScrollBar;

    Vec2i boxSize = {static_cast<int>(boxWidth_), static_cast<int>(boxHeight_)};
    Vec2i def = {width_,         height_};
    Vec2i min = {width_ * 3 / 4, height_};
    Vec2i max = {width_ * 5 / 4, height_};

    Layout* barLayout = new Layout{{def}, Config::defMargin, 0, min, max};
    Layout* boxLayout = new Layout{boxSize, Config::defMargin, 0};

    APISliderCallback* callback = new APISliderCallback{};

    ScrollBox*    box = new ScrollBox{boxLayout, boxTexture};
    HorScrollBar* bar = new HorScrollBar{barLayout, barTexture, box, callback};
    bar->SetMinValue(min_);
    bar->SetMaxValue(max_);

    APISlider* slider = new APISlider{bar, callback};
    return slider;
}

ILabel*
APIWidgetFactory::CreateDefaultLabel(const char* text_)
{
    return CreateLabel(Config::defAPI_LabelWidth, Config::defAPI_LabelHeight,
                                           text_, Config::defTextMaxCapacity);
}

ILabel*
APIWidgetFactory::CreateLabel
(int width_, int height_, const char* text_, int char_size_)
{
    Texture* texture = Config::defAPILabelTexture;

    Vec2i def = {width_,         height_};
    Vec2i min = {width_ * 3 / 4, height_};
    Vec2i max = {width_ * 5 / 4, height_};
    
    Layout* layout = new Layout{{def}, Config::defMargin, Config::defBorder, min, max};

    Text* iconText = new Text{text_, char_size_};

    TextIcon* textIcon = new TextIcon{layout, iconText, texture};
    APILabel* label = new APILabel{textIcon};
    return label;
}

IPreferencesPanel*
APIWidgetFactory::CreateDefaultPreferencesPanel()
{
    Texture* texture = Config::defDownReleaseTexture;
    return new APIPreferencesPanel{PropertiesPanel::GetDefault(texture)};
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
    //assert(x >=-1 && y >= 0);
    panel->Attach(dynamic_cast<APIWidget*>(widget)->GetBasicWidget());
}

//----------------------------------------//
