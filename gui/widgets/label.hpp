#pragma once

#include "widget.hpp"

//----------------------------------------//

class Icon : public Widget {
public:
  Icon(Layout *layout_, Texture *texture_);

  static Icon *GetDefault(Texture *texture_);

  bool OnEvent(const Event &event_) override;
};

class TextIcon : public Widget {
protected:
  std::unique_ptr<Text> text;
  std::string fullString;
  std::string curString;
  unsigned int textMaxSize;

  bool CheckSize(Vec2i size_);

public:
  TextIcon(Layout *layout_, Text *text_, Texture *texture_,
           int textMaxSize_ = Config::defTextMaxCapacity);

  static TextIcon *GetDefault(const std::string &string);

  void SetString(const char *string);

  void OnLayoutMove() override;
  void OnLayoutResize() override;

  bool OnEvent(const Event &event_) override;

  void Render() const override;
};

//----------------------------------------//

class TextLabelResponse {
public:
  virtual ~TextLabelResponse();

  virtual void OnResponse(const std::string &string) = 0;
};

class TextLabelResponseTest : public TextLabelResponse {
public:
  TextLabelResponseTest();

  void OnResponse(const std::string &string) override;
};

class TextLabel : public TextIcon {
protected:
  std::unique_ptr<TextLabelResponse> responce;

public:
  TextLabel(Layout *layout_, Texture *texture_, TextLabelResponse *response_,
            Text *textStyle_, int textMaxSize_ = Config::defTextMaxCapacity);

  bool ProcessListenerEvent(const Event &event_) override;
  bool OnEvent(const Event &event_) override;
};

//----------------------------------------//
