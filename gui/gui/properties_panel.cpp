#include "properties_panel.hpp"
#include "../widgets/label.hpp"

//----------------------------------------//

HideCommand::HideCommand(PropertiesPanel *panel_) : panel(panel_) {}

void HideCommand::OnResponse() { panel->Hide(); }

//----------------------------------------//

HideButton::HideButton(Layout *layout_, PropertiesPanel *panel_,
                       Texture *onRelease_, Texture *onHover_,
                       Texture *onPress_)
    : Button(layout_, new HideCommand{panel_}, onRelease_, onHover_, onPress_) {
}

HideButton *HideButton::GetDefault(PropertiesPanel *panel_) {
  Layout *layout =
      new Layout{{Config::defCloseButtonWidth, Config::defCloseButtonHeight}};

  return new HideButton{layout, panel_, Config::defCloseBtnRelease,
                        Config::defCloseBtnHover, Config::defCloseBtnPress};
}

//----------------------------------------//

PropertiesPanel::PropertiesPanel(Column *layout_, Texture *texture_)
    : Widget(layout_, texture_), visible(false) {}

PropertiesPanel *PropertiesPanel::GetDefault(Texture *texture_) {
  Column *column = new Column{Config::defMargin, Config::defBorder};
  PropertiesPanel *panel = new PropertiesPanel{column, texture_};

  Widget *linker1 = new Widget{new Row{Config::defMargin, 0}, texture_};

  Vec2i def = {Config::defTextIconWidth, Config::defTextIconHeight};
  Vec2i min = {Config::defMinTextIconWidth, Config::defTextIconHeight};
  Vec2i max = {Config::defMaxTextIconWidth, Config::defTextIconHeight};
  Layout *iconLayout = new Layout{{def}, 0, 0, min, max};

  Icon *icon = new Icon{iconLayout, texture_};
  HideButton *hideButton = HideButton::GetDefault(panel);

  linker1->Attach(icon);
  linker1->Attach(hideButton);
  panel->Attach(linker1);

  return panel;
}

void PropertiesPanel::Hide() { visible = false; }

void PropertiesPanel::Show() { visible = true; }

bool PropertiesPanel::ProcessEvent(const Event &event_) {
  if (visible)
    return Widget::ProcessEvent(event_);
  return false;
}

bool PropertiesPanel::ProcessListenerEvent(const Event &event_) {
  if (visible) {
    layout->OnEvent(event_);
    if (event_.type == mouseReleased) {
      system->Unsubscribe(mouseMoved);
      system->Unsubscribe(mouseReleased);
    }
    return true;
  }
  return false;
}

bool PropertiesPanel::OnEvent(const Event &event_) {
  if (visible) {
    if (event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
      return false;

    layout->OnEvent(event_);
    if (event_.type == mousePressed) {
      system->Reset();
      system->Subscribe(this, mouseMoved);
      system->Subscribe(this, mouseReleased);
    }
    return true;
  }
  return false;
}

void PropertiesPanel::Render() const {
  if (visible)
    Widget::Render();
}

void PropertiesPanel::RenderBorder() const {
  if (visible)
    Widget::RenderBorder();
}

//----------------------------------------//
