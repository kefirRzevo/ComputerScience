#pragma once

#include "view.hpp"


class GuiView: public View
{
    public:
        GuiView()
            {};

        ~GuiView()
            {}

        void Run() override
            {
                std::cout << "Not implemented" << std::endl;
            }

        size GetWindowSize() const override
            {
                std::cout << "Not implemented" << std::endl;
                return {};
            }

        bool GetStatus() const override
            {
                std::cout << "Not implemented" << std::endl;
                return false;
            }

    private:
        void UpdateWindowSize() override
            {
                std::cout << "Not implemented" << std::endl;
            }

        void UpdateStatus() override
            {
                std::cout << "Not implemented" << std::endl;
            }
};
