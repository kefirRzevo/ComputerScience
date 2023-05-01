#pragma once

#include "../widgets/widget.hpp"
#include "../widgets/button.hpp"
#include "../widgets/dropdownlist.hpp"

class Root;

class MainTitleBar: public Widget
{
    protected:

        ListOpenerButton* openedListButton;

    public:

        MainTitleBar(Root* root_);

        Button*
        GetMenuToolButton();

        Button*
        GetMenuFilterButton();

        void
        AddTools(DropDownList* list);

        void
        AddFilters(DropDownList* list);

        bool
        ProcessEvent(const Event& event_) override;
};

class Root: public Widget
{
    public:

        Root(Texture* texture_);
};

class CanvasResponse: public ButtonResponse
{
    private:

        Root* root;
        static int nCanvases;

    public:

        CanvasResponse(Root* root_);

        void
        OnResponse() override;
};
