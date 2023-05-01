#pragma once

#include "../graphlib/texture.hpp"
#include "../graphlib/renderer.hpp"
#include "plugin.hpp"

using namespace plugin;

class APITextureFactory: public ITextureFactory
{
    public:

        APITextureFactory() = default;

        ITexture*
        CreateTexture(const char* path) override;
        ITexture*
        CreateTexture(int width, int height) override;
};

class APITexture: public ITexture
{
    public:

        APITexture(Texture* texture_);
        
        int
        GetWidth() override;
        int
        GetHeight() override;

        Buffer
        ReadBuffer() override;
        void
        LoadBuffer(Buffer buffer) override;

        void
        Clear(Color color) override;

        void
        DrawLine(int x0, int y0, int x1, int y1, Color color) override;
        void
        DrawThickLine(int x0, int y0, int x1, int y1, int thickness, Color color) override;
        void
        DrawCircle(int x, int y, int radius, Color color) override;
        void
        DrawRect(int x, int y, int width, int height, Color color) override;

        void
        CopyTexture(ITexture* texture, int x, int y, int width, int height) override;

    private:

        Texture* texture;
};
