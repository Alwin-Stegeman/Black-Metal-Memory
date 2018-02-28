#include <wx/wx.h>
#include <wx/menu.h>
#include "wxImagePanel.h"
using namespace std;


class MemBoard : public wxFrame
{
    public:
        MemBoard();

    private:
        void NewGame(wxCommandEvent &event);
        void OnQuit(wxCommandEvent &event);
        void OnAbout(wxCommandEvent &event);
        void OnRules(wxCommandEvent &event);
        void OnBoardSize(wxCommandEvent &event);
        void OnStanLevel(wxCommandEvent &event);
        void OnTurns(wxCommandEvent &event);

        void GameStart();
        void templateBoardRight(int bsize, vector<int> &boardtemp);
        void mouseLeftDown(wxMouseEvent &evt);
        void Evaluate(wxString player);
        void TripletFound();
        void ShuffleRightBoard();
        void SwapPairLeftBoard();
        void GameOver();
        void compMemPairUpdate();
        void compMemTripUpdate();
        void compChoice();
        void compChoicePair();
        void compChoice2();
        void compChoiceEndGame();
        void compChoiceLeft();
        void compChoiceLeftLastCard();
        void compChoiceShow();
        void setText(wxStaticText *someST, wxString text);
        void setImage(int numPanel, int numImg);
        vector<int> sort_indexes(vector<int> &vec);

        static int boardsize;
        static int boardsizeNew;
        static int timeShow;
        static int compMemPerc;
        static int compMemPercNew;
        static bool takeTurns;
        static bool takeTurnsNew;
        static vector<wxImagePanel*> Board;
        static vector<int> compMem;
        static int trips_left;
        static wxString gamePhase;
        static int panelsTurned[];
        static int numTurnedLeft;
        static int numTurnedRight;
        static int userScore;
        static int compScore;
        static vector< vector<int> > compMemPairs;
        static vector< vector<int> > compMemTrips;
        static wxStaticText *scoreST;
        static wxStaticText *msgST;
        static vector<wxBitmap> imageVec;


};
