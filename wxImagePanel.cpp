#include "wxImagePanel.h"
#include <wx/dcbuffer.h>
using namespace std;

// This class has as object a panel with an image in it. Paint event handler and an image setter are provided.

wxImagePanel::wxImagePanel(wxFrame *parent, wxWindowID theID, int theAlbum, wxBitmap theBitmap) :
wxPanel(parent, theID, wxDefaultPosition, theBitmap.GetSize())
{
    image = theBitmap;
    album = theAlbum;
    turned = false;

    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Connect(wxEVT_PAINT, wxPaintEventHandler(wxImagePanel::paintEvent));

}


void wxImagePanel::paintEvent(wxPaintEvent &evt)
{
    wxAutoBufferedPaintDC dc(this);
    dc.DrawBitmap(image, 0, 0, false);
}



void wxImagePanel::setImage(wxBitmap theBitmap)
{
        image = theBitmap;
        Show();
        Refresh();
        Update();
}



