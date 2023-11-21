#include "button.hpp"
#include "label.hpp"

//----------------------------------------//

Button::Button(Layout *layout_, ButtonResponse *response_, Texture *onRelease_,
               Texture *onHover_, Texture *onPress_)
    : Widget(layout_, onRelease_), response(response_), pressed(false),
      onRelease(onRelease_), onHover(onHover_), onPress(onPress_) {
  if (!onHover)
    onHover = onRelease;
  if (!onPress)
    onPress = onRelease;
}

Button *Button::GetDefault(const std::string &string_,
                           ButtonResponse *response_) {
  Texture *rel = Config::defUpReleaseTexture;
  Texture *hov = Config::defUpHoverTexture;
  Texture *press = Config::defUpPressTexture;

  Button *btn = new Button{new Row{}, response_, rel, hov, press};
  btn->Attach(TextIcon::GetDefault(string_));
  return btn;
}

bool Button::GetPressed() const { return pressed; }

void Button::SetButtonResponse(ButtonResponse *response_) {
  if (response.unique())
    delete response.get();

  response.reset(response_);
}

bool Button::ProcessListenerEvent(const Event &event_) {
  if (event_.type == mouseReleased) {
    if (layout->IsInside(event_.mouse.pos))
      if (pressed)
        response->OnResponse();

    pressed = false;
    texture = onRelease;
    system->Unsubscribe(mouseReleased);

    return true;
  } else if (event_.type == mouseHovered) {
    system->Unsubscribe(mouseHovered);
    if (!pressed)
      texture = onRelease;
  }
  return false;
}

bool Button::OnEvent(const Event &event_) {
  if (event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
    return false;

  switch (event_.type) {
  case mousePressed: {
    system->Reset();
    system->Subscribe(this, mouseHovered);
    system->Subscribe(this, mouseReleased);
    pressed = true;
    texture = onPress;
    break;
  }
  case mouseHovered: {
    system->Subscribe(this, mouseHovered);
    if (!pressed)
      texture = onHover;
    break;
  }
  default: {
    break;
  }
  }
  return true;
}

//----------------------------------------//

OptionButton::OptionButton(Layout *layout_, ButtonResponse *response_,
                           Texture *onRelease_, Texture *onHover_,
                           Texture *onPress_)
    : Button(layout_, response_, onRelease_, onHover_, onPress_),
      active(false) {}

OptionButton *OptionButton::GetDefault(const std::string &string_,
                                       ButtonResponse *response_) {
  Texture *rel = Config::defUpReleaseTexture;
  Texture *hov = Config::defUpHoverTexture;
  Texture *press = Config::defUpPressTexture;

  OptionButton *btn = new OptionButton{new Row{}, response_, rel, hov, press};
  btn->Attach(TextIcon::GetDefault(string_));
  return btn;
}

bool OptionButton::GetActive() const { return active; }

void OptionButton::Diactivate() { active = false; }

bool OptionButton::ProcessListenerEvent(const Event &event_) {
  if (event_.type == mouseReleased) {
    if (layout->IsInside(event_.mouse.pos)) {
      if (pressed) {
        response->OnResponse();
        active = true;
      }
    }

    pressed = false;
    if (!active)
      texture = onRelease;
    system->Unsubscribe(mouseReleased);
    return true;
  } else if (event_.type == mouseHovered) {
    system->Unsubscribe(mouseHovered);
    if (!pressed && !active)
      texture = onRelease;
  }
  return false;
}

bool OptionButton::OnEvent(const Event &event_) {
  if (event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
    return false;

  switch (event_.type) {
  case mousePressed: {
    system->Reset();
    system->Subscribe(this, mouseHovered);
    system->Subscribe(this, mouseReleased);
    pressed = true;
    texture = onPress;
    break;
  }
  case mouseHovered: {
    system->Subscribe(this, mouseHovered);
    if (!pressed && !active)
      texture = onHover;
    break;
  }
  default: {
    break;
  }
  }
  return true;
}

//----------------------------------------//

CloseCommand::CloseCommand(Widget *widget_) : widget(widget_) {}

void CloseCommand::OnResponse() {
  assert(widget);
  EventManager::Get()->PushEvent({widgetClosed, {widget}});
}

//----------------------------------------//

CloseButton::CloseButton(Layout *layout_, Widget *toClose_, Texture *onRelease_,
                         Texture *onHover_, Texture *onPres_)
    : Button(layout_, new CloseCommand{toClose_}, onRelease_, onHover_,
             onPres_) {}

CloseButton *CloseButton::GetDefault(Widget *toClose_) {
  Layout *layout =
      new Layout{{Config::defCloseButtonWidth, Config::defCloseButtonHeight}};

  return new CloseButton{layout, toClose_, Config::defCloseBtnRelease,
                         Config::defCloseBtnHover, Config::defCloseBtnPress};
}

//----------------------------------------//
