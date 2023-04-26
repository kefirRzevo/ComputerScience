#pragma once

#include "../widgets/button.hpp"
#include "../widgets/scrollbar.hpp"
#include "../widgets/label.hpp"
#include "../gui/properties_panel.hpp"
#include "plugin.hpp"

using namespace plugin;


class APIWidgetFactory: public IWidgetFactory
{
    public:

        APIWidgetFactory() = default;

        IButton*
        CreateDefaultButtonWithIcon(const char* iconPath) override;
        IButton*
        CreateDefaultButtonWithText(const char* text) override;
        IButton*
        CreateButtonWithIcon
        (int width, int height, const char* icon_file_name) override;
        IButton*
        CreateButtonWithText
        (int width, int height, const char* text, int char_size) override;

        ISlider*
        CreateDefaultSlider(float range_min, float range_max) override;
        ISlider*
        CreateSlider
        (int width, int height, float range_min, float range_max) override;
        ISlider*
        CreateSlider
        (int width, int height, float boxWidth, float boxHeight,
        float min, float max) override;

        ILabel*
        CreateDefaultLabel(const char* text) override;
        ILabel*
        CreateLabel
        (int width, int height, const char* text, int char_size) override;

        IPreferencesPanel*
        CreateDefaultPreferencesPanel() override;
};

class APIClickCallback: public ButtonResponse
{
    public:

        APIClickCallback(IClickCallback* callback_ = nullptr);

        void
        SetCallback(IClickCallback* callback_);

        void
        OnResponse() override;

    private:

        IClickCallback* callback;
};

class APISliderCallback: public ScrollBarResponse
{
    public:

        APISliderCallback(ISliderCallback* callback_ = nullptr);

        void
        SetCallback(ISliderCallback* callback_);

        void
        OnResponse(float value_) override;

    private:

        ISliderCallback* callback;
};

class APIWidget: public IWidget
{
    public:

        virtual Widget*
        GetBasicWidget() = 0;
};

class APIButton: public IButton, public APIWidget
{
    public:

        APIButton(Button* button_, APIClickCallback* callback_);

        Widget*
        GetBasicWidget() override;
        int
        GetWidth()  override;
        int
        GetHeight() override;

        void
        SetClickCallback(IClickCallback* callback_) override;

    private:

        Button* button;
        APIClickCallback* callback;
};

class APISlider: public ISlider, public APIWidget
{
    public:

        APISlider(HorScrollBar* scrollBar_, APISliderCallback* callback_);

        Widget*
        GetBasicWidget() override;
        int
        GetWidth()  override;
        int
        GetHeight() override;
        float
        GetValue() override;
        
        void
        SetValue(float value) override;
        void
        SetSliderCallback(ISliderCallback* callback_) override;

    private:

        HorScrollBar* scrollBar;
        APISliderCallback* callback;
};

class APILabel: public ILabel, public APIWidget
{
    public:

        APILabel(TextIcon* textIcon_);

        void
        SetText(const char* text_) override;

        Widget*
        GetBasicWidget() override;
        int
        GetWidth() override;
        int
        GetHeight() override;

    private:

        TextIcon* textIcon;
};

class APIPreferencesPanel: public IPreferencesPanel, public APIWidget
{
    public:

        APIPreferencesPanel(PropertiesPanel* panel_);

        Widget*
        GetBasicWidget() override;
        int
        GetWidth() override;
        int
        GetHeight() override;

        void
        Attach(IWidget* widget, int x, int y) override;

    private:

        PropertiesPanel* panel;
};
