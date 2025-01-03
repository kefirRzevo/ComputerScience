#include "apitexture.hpp"

ITexture *APITextureFactory::CreateTexture(const char *path) {
  return new APITexture{TextureManager::Get()->GetTexture(path)};
}

ITexture *APITextureFactory::CreateTexture(int width, int height) {
  return new APITexture{new Texture{{width, height}}};
}

APITexture::APITexture(Texture *texture_) : texture(texture_) {}

int APITexture::GetWidth() { return texture->GetWidth(); }

int APITexture::GetHeight() { return texture->GetHeight(); }

void APITexture::SetTexture(Texture *texture_) { texture = texture_; }

Buffer APITexture::ReadBuffer() {
  return Buffer{texture->ToBuffer(), static_cast<ITexture *>(this)};
}

void APITexture::LoadBuffer(Buffer buffer) {
  texture->FromBuffer(buffer.pixels);
}

void APITexture::Clear(Color color) {
  Renderer::Get()->SetColor(color);
  Renderer::Get()->Clear(texture);
}

void APITexture::DrawLine(int x0, int y0, int x1, int y1, Color color) {
  Renderer::Get()->SetColor(color);
  Renderer::Get()->DrawLine(texture, {x0, y0}, {x1, y1});
}
void APITexture::DrawThickLine(int x0, int y0, int x1, int y1, int thickness,
                               Color color) {
  Renderer::Get()->SetColor(color);
  Renderer::Get()->DrawThickLineSlow(texture, {x0, y0}, {x1, y1}, thickness);
}

void APITexture::DrawCircle(int x, int y, int radius, Color color) {
  Renderer::Get()->SetColor(color);
  Renderer::Get()->DrawCircle(texture, {x, y}, radius);
}

void APITexture::DrawRect(int x, int y, int width, int height, Color color) {
  Renderer::Get()->SetColor(color);
  Renderer::Get()->DrawRect(texture, {x, y, width, height});
}

void APITexture::CopyTexture(ITexture *iTexture, int x, int y, int width,
                             int height) {
  APITexture *dst = static_cast<APITexture *>(iTexture);
  Renderer::Get()->DrawTexture(texture, {x, y, width, height}, dst->texture);
}
