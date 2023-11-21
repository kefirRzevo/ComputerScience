#pragma once

#include "../widgets/button.hpp"
#include "../widgets/dropdownlist.hpp"
#include "../widgets/widget.hpp"
#include "properties_panel.hpp"

class Root;

class MainTitleBar : public Widget {
protected:
  ListOpenerButton *openedListButton;

public:
  MainTitleBar(Root *root_);

  Button *GetMenuToolButton();
  Button *GetMenuFilterButton();

  void AttachPanel(PropertiesPanel *child_);
  void DetachPanel(PropertiesPanel *child_);

  void AddTools(DropDownList *list);
  void AddFilters(DropDownList *list);

  bool ProcessEvent(const Event &event_) override;
};

class Root : public Widget {
public:
  Root(Texture *texture_);
};

class CanvasResponse : public ButtonResponse {
private:
  Root *root;

public:
  CanvasResponse(Root *root_);

  void OnResponse() override;
};
