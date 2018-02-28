#include <wx/wx.h>


class wxImagePanel : public wxPanel
    {
    public:
        wxImagePanel(wxFrame *parent, wxWindowID theID, int theAlbum, wxBitmap theBitmap);

        void paintEvent(wxPaintEvent &evt);

        int album;
        void setImage(wxBitmap theBitmap);
        bool turned;

    private:
        wxBitmap image;



    };

