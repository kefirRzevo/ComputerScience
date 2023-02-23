#include <sys/ioctl.h>
#include <csignal>

#include "../include/view.hpp"


//------------View------------//

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

//------------Text-View------------//

void SigHandler(int signum)
{
        View* view = View::Get();
        if(!view)
                return;

        if(signum == SIGWINCH)
        {
                view->UpdateWindowSize();
        }
        else if(signum == SIGINT)
        {
                //std::cout << "Goodbye!" << std::endl;
                view->status = false;
        }
}

TextView::TextView()
{
        UpdateWindowSize();
        status = true;

        signal(SIGWINCH, SigHandler);
        signal(SIGINT, SigHandler);
        CarretOff();
}

void TextView::UpdateWindowSize()
{
        struct ttysize ts;
        ioctl(0, TIOCGSIZE, &ts);
        windowSize.first = ts.ts_cols;
        windowSize.second = ts.ts_lines;
}

static const int CSI_fg_color_code = 30;
static const int CSI_bg_color_code = 40;

void TextView::AddProperties(enum TextView::Color fg, enum TextView::Color bg) const
{
        printf("\e[%d;%dm", fg + CSI_fg_color_code, bg + CSI_bg_color_code);
        fflush(stdout);
}

//------------Gui-View------------//

//not implemented

//--------------------------------//
