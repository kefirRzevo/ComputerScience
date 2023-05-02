#pragma once

#include "../widgets/widget.hpp"
#include "../widgets/button.hpp"


class HideCommand;
class HideButton;

class PropertiesPanel;

//----------------------------------------//

class HideCommand: public ButtonResponse
{
    private:

        PropertiesPanel* panel;

    public:

        HideCommand(PropertiesPanel* panel_);

        void
        OnResponse() override;
};

class HideButton: public Button
{
    public:

        HideButton(Layout* layout_, PropertiesPanel* panel_,
        Texture* onRelease_, Texture* onHover_ = nullptr,
        Texture* onPress_ = nullptr);

        static HideButton*
        GetDefault(PropertiesPanel* panel_);
};

//----------------------------------------//

class PropertiesPanel: public Widget
{
    protected:

        bool visible;

    public:

        PropertiesPanel(Column* layout_, Texture* texture_);

        static PropertiesPanel*
        GetDefault(Texture* texture_);

        void
        Hide();
        void
        Show();

        bool
        ProcessEvent(const Event& event_) override;
        bool
        ProcessListenerEvent(const Event& event_) override;
        bool
        OnEvent(const Event& event_) override;

        void
        Render() const override;
        void
        RenderBorder() const override;
};

//----------------------------------------//
