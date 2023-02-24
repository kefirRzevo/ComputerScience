#include "../include/view.hpp"
#include "../include/textview.hpp"
#include "../include/guiview.hpp"


View* View::Get(const std::string& mode)
{
        static View* view;

        if(view)
                return view;

        if(!mode.compare("gui"))
                view = new GuiView();
	else if(!mode.compare("text"))
                view = new TextView();
        else
                view = nullptr;

        return view;	    
}
