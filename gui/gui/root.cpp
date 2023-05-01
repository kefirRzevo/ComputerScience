#include "root.hpp"
#include "canvas.hpp"
#include "properties_panel.hpp"
#include "../widgets/dropdownlist.hpp"
#include "../widgets/scrollbar.hpp"

#include "../plugin/pluginmanager.hpp"
#include "../plugin/apiwidgets.hpp"
#include "../plugin/manager.hpp"

int
CanvasResponse::nCanvases = 0;

template<class T>
class Setter: public ButtonResponse
{
    protected:

        MainTitleBar* bar;
        T* object;

    public:

        Setter(T* object_, MainTitleBar* bar_):
        bar(bar_), object(object_)
        {}

        void
        OnResponse() override
        {
            Manager<T>::Get()->SetActive(object);
            APIPreferencesPanel* panel = static_cast<APIPreferencesPanel*>(object->GetPreferencesPanel());
            if(panel)
            {
                bar->Detach( panel->GetBasicWidget());
                bar->Attach(panel->GetBasicWidget());
            }
        }
};

//----------------------------------------//

MainTitleBar::MainTitleBar(Root* root_):
Widget(new Row{}, Config::defUpReleaseTexture), openedListButton(nullptr)
{
    Attach(Button::GetDefault("Canvas", new CanvasResponse{root_}));
    Attach(GetMenuToolButton());
    Attach(GetMenuFilterButton());
}

Button*
MainTitleBar::GetMenuToolButton()
{
    DropDownList* toolList = new DropDownList{Config::defDownReleaseTexture};
    ListOpenerButton* toolsBtn = ListOpenerButton::GetDefault("Tools", toolList);
    AddTools(toolList);
    return toolsBtn;
}

Button*
MainTitleBar::GetMenuFilterButton()
{
    DropDownList* filterList = new DropDownList{Config::defDownReleaseTexture};
    ListOpenerButton* filtersBtn = ListOpenerButton::GetDefault("Filters", filterList);
    AddFilters(filterList);
    return filtersBtn;
}

void
MainTitleBar::AddTools(DropDownList* list)
{
    for(Plugin* plugin: PluginManager::Get()->GetPlugins())
    {
        if(plugin->GetLoadStatus())
            continue;
        plugin->Load();

        IPlugin* iPlugin = plugin->GetIPlugin();
        if(!iPlugin)
            continue;

        for(ITool* tool: iPlugin->GetTools())
        {
            Texture* iconTexture = TextureManager::Get()->GetTexture(tool->GetIconFileName());
            assert(iconTexture);

            Button* toolBtn = Button::GetDefault(tool->GetName(), new Setter<ITool>(tool, this));
            toolBtn->Attach(Icon::GetDefault(iconTexture));
            list->Attach(toolBtn);
        }
    }
}

void
MainTitleBar::AddFilters(DropDownList* list)
{
    for(Plugin* plugin: PluginManager::Get()->GetPlugins())
    {
        if(plugin->GetLoadStatus())
            continue;
        plugin->Load();

        IPlugin* iPlugin = plugin->GetIPlugin();
        if(!iPlugin)
            continue;

        for(IFilter* filter: iPlugin->GetFilters())
        {
            Button* filterBtn = Button::GetDefault(filter->GetName(), new Setter<IFilter>(filter, this));
            list->Attach(filterBtn);
        }
    }
}

bool
MainTitleBar::ProcessEvent(const Event& event_)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        if((*it)->ProcessEvent(event_))
        {
            ListOpenerButton* processedChild = dynamic_cast<ListOpenerButton*>(*it);
            if(processedChild && processedChild->GetListVisibility())
            {
                if(processedChild != openedListButton)
                {
                    if(openedListButton)
                    {
                        openedListButton->HideList();
                    }

                    openedListButton = processedChild;
                }
            }

            if(event_.type == mousePressed)
            {
                Widget* active = *it;
                children.erase(it);
                children.push_front(active);
            }
            return true;
        }
    }
    return OnEvent(event_);
}

//----------------------------------------//

Root::Root(Texture* texture_):
Widget(new Layout{{Config::defWindowWidth, Config::defWindowHeight}}, texture_)
{
    Attach(new MainTitleBar{this});
}

//----------------------------------------//

CanvasResponse::CanvasResponse(Root* root_):
root(root_)
{}

void
CanvasResponse::OnResponse()
{
    std::string title = "Canvas" +  std::to_string(nCanvases);
    root->Attach(new CanvasWidget{title, {Config::defFullTextureWidth, Config::defFullTextureHeight}});
    nCanvases++;
}

//----------------------------------------//
