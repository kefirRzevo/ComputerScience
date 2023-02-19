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
	else
                view = new TextView();

        return view;	    
}

//------------Text-View------------//

void SigHandler(int signum)
{
        if(signum == SIGWINCH)
        {
                View* view = View::Get();
                if(view)
                        view->UpdateWinSize();
        }
}

TextView::TextView()
{
        signal(SIGWINCH, SigHandler);
        UpdateWinSize();
        //CarretOff();
}

void TextView::UpdateWinSize()
{
        struct ttysize ts;
        ioctl(0, TIOCGSIZE, &ts);
        width = ts.ts_cols;
        height = ts.ts_lines;

        //std::cout << width << " " << height << std::endl;
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
