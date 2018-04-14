#include "MemBoard.h"
using namespace std;

// This class has as object the memory game board with a menu, and contains all variables of the game
// and all functions governing the game events, including mouse event handlers.

int MemBoard::boardsize = 0;
int MemBoard::boardsizeNew = 0;
int MemBoard::compMemPerc = 0;
int MemBoard::compMemPercNew = 0;
bool MemBoard::takeTurns;
bool MemBoard::takeTurnsNew;
int MemBoard::trips_left = 0;
vector<wxImagePanel*> MemBoard::Board;
vector<int> MemBoard::compMem;
wxString MemBoard::gamePhase = wxT("userMove");
int MemBoard::numTurnedLeft = 0;
int MemBoard::numTurnedRight = 0;
int MemBoard::panelsTurned[3] = {0, 0, 0};
int MemBoard::userScore = 0;
int MemBoard::compScore = 0;
int MemBoard::timeShow = 0;
vector< vector<int> > MemBoard::compMemPairs;
vector< vector<int> > MemBoard::compMemTrips;
wxStaticText *MemBoard::msgST;
wxStaticText *MemBoard::scoreST;
vector<wxBitmap> MemBoard::imageVec;


MemBoard::MemBoard() :
    wxFrame(NULL, wxID_ANY, wxT("\\\\ BLACK METAL MEMORY //"), wxPoint(50,50), wxSize(700,700))
{
// Constructor of the game board.
// default settings of main game parameters
    boardsizeNew = 3;
    compMemPercNew = 60;
    takeTurnsNew = true;

    wxInitAllImageHandlers();
// add start image of baphomet
    wxBoxSizer *beginbox = new wxBoxSizer(wxHORIZONTAL);
    wxImage image;
    image.LoadFile("images\\img000.jpg", wxBITMAP_TYPE_JPEG);
    beginbox->Add(new wxImagePanel(this, wxID_ANY, 0, wxBitmap(image)));
    SetSizer(beginbox);

// add menubar and menu items
    int wxID_NEWGAME = 1;
    int wxID_ABOUT = 2;
    int wxID_ABOUT2 = 3;
    int wxID_RULES = 4;
    int wxID_TURNS = 7;
    int wxID_NOTURNS = 8;
    int wxID_BOARD3 = 13;
    int wxID_BOARD4 = 14;
    int wxID_BOARD5 = 15;
    int wxID_BOARD6 = 16;
    int wxID_BOARD7 = 17;
    int wxID_STAN1 = 60;
    int wxID_STAN2 = 70;
    int wxID_STAN3 = 80;
    int wxID_STAN4 = 90;
    int wxID_STAN5 = 100;

    wxMenuBar *menubar = new wxMenuBar;
    wxMenu *options = new wxMenu;

    wxMenu *turns = new wxMenu;
    turns->AppendRadioItem(wxID_TURNS, wxT("Take Turns"));
    turns->AppendRadioItem(wxID_NOTURNS, wxT("Triplet Finder Continues Turn"));

    wxMenu *board = new wxMenu;
    board->AppendRadioItem(wxID_BOARD3, wxT("3x3"));
    board->AppendRadioItem(wxID_BOARD4, wxT("4x4"));
    board->AppendRadioItem(wxID_BOARD5, wxT("5x5"));
    board->AppendRadioItem(wxID_BOARD6, wxT("6x6"));
    board->AppendRadioItem(wxID_BOARD7, wxT("7x7"));

    wxMenu *stanmem = new wxMenu;
    stanmem->AppendRadioItem(wxID_STAN1, wxT("Weak"));
    stanmem->AppendRadioItem(wxID_STAN2, wxT("Mediocre"));
    stanmem->AppendRadioItem(wxID_STAN3, wxT("Good"));
    stanmem->AppendRadioItem(wxID_STAN4, wxT("Excellent"));
    stanmem->AppendRadioItem(wxID_STAN5, wxT("Perfect"));

    options->Append(wxID_NEWGAME, wxT("Start New Game"));
    options->AppendSeparator();
    options->AppendSubMenu(board, wxT("Set Board Size"));
    options->AppendSubMenu(stanmem, wxT("Set STAN's Memory"));
    options->AppendSubMenu(turns, wxT("Set Turn Order"));
    options->AppendSeparator();
    options->Append(wxID_EXIT, wxT("Quit"));

    menubar->Append(options, wxT("Exercise Your Free Will"));

    wxMenu *info = new wxMenu;
    info->Append(wxID_ABOUT, wxT("About"));
    info->Append(wxID_RULES, wxT("Game Rules"));

    menubar->Append(info, wxT("Unholy Information"));
    SetMenuBar(menubar);

    Connect(wxID_NEWGAME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MemBoard::NewGame));
    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MemBoard::OnQuit));
    Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MemBoard::OnAbout));
    Connect(wxID_RULES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MemBoard::OnRules));
    for (int count = wxID_TURNS; count <=wxID_NOTURNS; count++) {
        Connect(count, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MemBoard::OnTurns));
    }
    for (int count = wxID_BOARD3; count <=wxID_BOARD7; count++) {
        Connect(count, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MemBoard::OnBoardSize));
    }
    for (int count = wxID_STAN1; count <=wxID_STAN5; count++) {
        Connect(count, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MemBoard::OnStanLevel));
    }


    Fit();
    SetMinSize(GetSize());
    SetMaxSize(GetSize());
    Centre();
    Show();

}


// Next follow the event handlers for the menu options.
void MemBoard::NewGame(wxCommandEvent &event)
{
    boardsize = boardsizeNew;
    timeShow = 2000;
    takeTurns = takeTurnsNew;
    compMemPerc = compMemPercNew;

// adjust computer memory for board size (weak=30 for board size=7, weak=60 for board size=3)
    if (compMemPerc<100) {
        int diff = boardsizeNew - 3;
        int weak = compMemPerc - int((30/4)*diff);
        int step = int((100 - weak)/4);
        int level = int((compMemPerc - 60)/10);
        compMemPerc = weak + level*step;
    }

    GameStart();
}


void MemBoard::OnQuit(wxCommandEvent &event)
{
    int answer = wxMessageBox("Are you sure you want to quit ?", "Confirm to quit", wxYES_NO, this);
    if (answer == wxYES) {
        Destroy();
    }
}


void MemBoard::OnBoardSize(wxCommandEvent &event)
{
//    int wxID_BOARD3 = 13;
//    int wxID_BOARD4 = 14;
//    int wxID_BOARD5 = 15;
//    int wxID_BOARD6 = 16;
//    int wxID_BOARD7 = 17;

    boardsizeNew = event.GetId()-10;
}


void MemBoard::OnStanLevel(wxCommandEvent &event)
{
//    int wxID_STAN1 = 60;
//    int wxID_STAN2 = 70;
//    int wxID_STAN3 = 80;
//    int wxID_STAN4 = 90;
//    int wxID_STAN5 = 100;

    compMemPercNew = event.GetId();
}


void MemBoard::OnTurns(wxCommandEvent &event)
{
    int wxID_TURNS = 7;
    int wxID_NOTURNS = 8;

    if (event.GetId()==wxID_TURNS) {
        takeTurnsNew = true;
    }
    if (event.GetId()==wxID_NOTURNS) {
        takeTurnsNew = false;
    }
}




void MemBoard::OnAbout(wxCommandEvent &event)
{
    wxString about = wxString( _("Programmed by Alwin in January 2018, ") +
            _("using C++ and wxWidgets. "));
    wxMessageBox(about, wxT("\\\\ BLACK METAL MEMORY //"), wxOK, this);
}



void MemBoard::OnRules(wxCommandEvent &event)
{
    wxString rules = wxString( _("Black Metal Memory consists of two game boards with upside down cards. The goal is ") +
                              _("to obtain three identical cards (a triplet) by picking two cards from the left board and one card from the right ") +
                              _("board. The user plays against STAN and the players either take turns or may continue after finding ") +
                              _("a triplet (see \"Exercise Your Free Will\"). \n\n") +
                              _("Some cards in the left board are special: after picking a Megadeth card in the left board, the two ") +
                              _("chosen cards in that board are swapped. In the right board some cards are special too: a Metallica card ") +
                              _("reshuffles the complete right board, while Bathory and Venom cards can be used to complete any ") +
                              _("identical pair in the left board. Metallica, Bathory, and Venom cards only appear in the right board. \n\n") +
                              _("Under \"Exercise Your Free Will\" the board size, STAN's memory strength, and the turn order can be set. \n\n") +
                                _("Have fun playing Black Metal Memory !"));
    wxMessageBox(rules, wxT("\\\\ BLACK METAL MEMORY RULES //"), wxOK, this);
}



void MemBoard::GameStart() {
// Function to start the game and set up the game board with shuffled cards.
        Hide();
        GetSizer()->Clear(true);
        DestroyChildren();
        SetSizer(NULL);
        SetMaxSize(wxSize(2000,1000));

        int bsize = boardsize;
        numTurnedLeft = 0;
        numTurnedRight = 0;
        userScore = 0;
        compScore = 0;
        Board.clear();
        compMem.clear();
        gamePhase = wxT("userMove");
        int numtripels = int(bsize*bsize/2);
        int numjoker[6] = { 0, 1, 2, 3, 3, 4 };
        int numrandom[6] = { 1, 1, 1, 1, 1, 2 };
        int numswitch[6] = { 1, 1, 2, 3, 4, 5 };
        vector<int>::iterator it;
        trips_left = numtripels;
        imageVec.clear();
        compMemPairs.clear();
        compMemTrips.clear();

// make wxBitmap vector imageVec
        wxInitAllImageHandlers();
        vector<int> imgNumber(33);
        iota(imgNumber.begin(), imgNumber.end(), 0);
        for (int count = 101; count <= 105; count++) {
            imgNumber.push_back(count);
        }
        for (int count = 666; count <= 669; count++) {
            imgNumber.push_back(count);
        }
        imgNumber.push_back(888);
        imgNumber.push_back(889);
        imgNumber.push_back(999);

        wxImage image;
        wxString file;
        for (int count = 0; count < imgNumber.size(); count++) {
            file = "images\\img" + to_string(imgNumber[count]) + ".jpg";
            image.LoadFile(file, wxBITMAP_TYPE_JPEG);
            image.Rescale(int(575/bsize), int(575/bsize));
            imageVec.push_back(wxBitmap(image));
        }

// select album cards randomly
        vector<int> allAlbums(32);
        iota(allAlbums.begin(), allAlbums.end(), 1);
        vector<int> allSwitch = {101, 102, 103, 104, 105};
        vector<int> allJokers = {666, 667, 668, 669};
        vector<int> allRandoms = {888, 889};
        shuffle(allAlbums.begin(), allAlbums.end(), default_random_engine(time(0)));
        shuffle(allSwitch.begin(), allSwitch.end(), default_random_engine(time(0)));
        shuffle(allJokers.begin(), allJokers.end(), default_random_engine(time(0)));
        shuffle(allRandoms.begin(), allRandoms.end(), default_random_engine(time(0)));
        vector<int> albumsLeft(allAlbums.begin(), allAlbums.begin()+numtripels-numswitch[bsize-3]);
        for (int count = 0; count < numswitch[bsize-3]; count++) {
            albumsLeft.push_back(allSwitch[count]);
        }
        vector<int> albumsRight = albumsLeft;
        for (int count = 0; count < numjoker[bsize-3]; count++) {
            albumsRight.push_back(allJokers[count]);
        }
        for (int count = 0; count < numrandom[bsize-3]; count++) {
            albumsRight.push_back(allRandoms[count]);
        }

// begin board construction
        wxFlexGridSizer *fbox = new wxFlexGridSizer(2, 2, 3, 3);
        wxStaticBoxSizer *h1box = new wxStaticBoxSizer(wxHORIZONTAL, this);
        wxStaticBoxSizer *h2box = new wxStaticBoxSizer(wxHORIZONTAL, this);
        wxStaticBoxSizer *t1box = new wxStaticBoxSizer(wxHORIZONTAL, this);
        wxStaticBoxSizer *t2box = new wxStaticBoxSizer(wxHORIZONTAL, this);

// construct left board
        wxGridSizer *gsizerLeft = new wxGridSizer(bsize, bsize, 0, 0);

        albumsLeft.insert(albumsLeft.end(), albumsLeft.begin(), albumsLeft.end());
        shuffle(albumsLeft.begin(), albumsLeft.end(), default_random_engine(time(0)));

        if (bsize%2==1) {
            it = albumsLeft.begin();
            advance(it, numtripels);
            it = albumsLeft.insert(it, 0);
        }

        for (int count = 0; count < bsize*bsize; count++) {
                Board.push_back(new wxImagePanel(this, count, albumsLeft[count], imageVec[0]));
                Board[count]->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MemBoard::mouseLeftDown));
                gsizerLeft->Add(Board[count], 0, wxALL, 3);
                compMem.push_back(0);
        }

        if (bsize%2==1) {
            compMem[numtripels] = -1;
        }

// construct right board
        wxGridSizer *gsizerRight = new wxGridSizer(bsize, bsize, 0, 0);

        vector<int> boardShapeRight (bsize*bsize);
        templateBoardRight(bsize, boardShapeRight);

        shuffle(albumsRight.begin(), albumsRight.end(), default_random_engine(time(0)));

        int countRight = 0;
        for (int count = 0; count < bsize*bsize; count++) {
                if (boardShapeRight[count]==1) {
                    Board.push_back(new wxImagePanel(this, bsize*bsize+count, albumsRight[countRight], imageVec[0]));
                    Board[count+bsize*bsize]->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MemBoard::mouseLeftDown));
                    countRight++;
                    compMem.push_back(0);
                }
                else {
                    Board.push_back(new wxImagePanel(this, bsize*bsize+count, -1, imageVec[0]));
                    Board[count+bsize*bsize]->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MemBoard::mouseLeftDown));
                    compMem.push_back(-1);
                }
                gsizerRight->Add(Board[count+bsize*bsize], 0, wxALL, 3);
        }


// construct text panels
        wxString scoreText = wxT("Score:   You  ") + to_string(userScore) + wxT(" - ") + to_string(compScore) + wxT("  STAN");
        scoreST = new wxStaticText(this, wxID_ANY, scoreText, wxDefaultPosition, wxSize(600,28), wxTE_CENTRE);
        msgST = new wxStaticText(this, wxID_ANY, wxT("Are you ready? Pick your cards."), wxDefaultPosition, wxSize(600,28), wxTE_CENTRE);
        wxFont myFont(19, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        scoreST->SetFont(myFont);
        msgST->SetFont(myFont);

// put left and right boards in frame
        t1box->Add(scoreST, 0, wxEXPAND);
        t2box->Add(msgST, 0, wxEXPAND);
        h1box->Add(gsizerLeft);
        h2box->Add(gsizerRight);
        fbox->Add(t1box, 0, wxEXPAND);
        fbox->Add(t2box, 0, wxEXPAND | wxLEFT, 10);
        fbox->Add(h1box);
        fbox->Add(h2box, 0, wxLEFT, 10);

        SetSizer(fbox);
        Fit();

        SetMinSize(GetSize());
        SetMaxSize(GetSize());

        if (bsize%2==1) {
            Board[numtripels]->Hide();
        }
        for (int count = 0; count < bsize*bsize; count++) {
                if (boardShapeRight[count]==0) {
                    Board[count+bsize*bsize]->Hide();
                }
        }

        Layout();
        Centre();
        Show();
}



void MemBoard::templateBoardRight(int bsize, vector<int> &boardtemp) {
// Function that specifies the shape of the right board depending on the board size.
// NB: vector goaltemp is declared inside each if-statement and is deleted from the stack when exiting it !
// This is why assigning boardtemp=goaltemp  should be inside each if-statement.
    if (bsize==3) {
        vector<int>  goaltemp = { 0, 1, 0, 1, 1, 1, 0, 1, 0 };
        boardtemp = goaltemp;
    }
    if (bsize==4) {
        vector<int> goaltemp = { 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1 };
        boardtemp = goaltemp;
    }
    if (bsize==5) {
        vector<int> goaltemp = { 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1 };
        boardtemp = goaltemp;
    }
    if (bsize==6) {
        vector<int> goaltemp = { 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0,
                                                                                                        0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1 };
        boardtemp = goaltemp;
    }
    if (bsize==7) {
        vector<int> goaltemp = { 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                                                                        1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1 };
        boardtemp = goaltemp;
    }
    if (bsize==8) {
        vector<int> goaltemp = { 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0,
                                              0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1 };
        boardtemp = goaltemp;
    }
}


// Next follows the mouse event handler.
void MemBoard::mouseLeftDown(wxMouseEvent &evt)
{
    if (gamePhase=="userMove" && evt.GetId()<boardsize*boardsize && Board[evt.GetId()]->turned==false
                                                                                                                    && numTurnedLeft<2) {
        setText(msgST, wxT(""));
        setImage(evt.GetId(), Board[evt.GetId()]->album);
        Board[evt.GetId()]->turned = true;
        panelsTurned[numTurnedLeft+numTurnedRight] = evt.GetId();
        numTurnedLeft++;
        if (numTurnedLeft+numTurnedRight==3) {
            Evaluate(wxT("user"));
        }
    }
    if (gamePhase=="userMove" && evt.GetId()>=boardsize*boardsize && Board[evt.GetId()]->turned==false
                                                                                                                    && numTurnedRight<1) {
        setText(msgST, wxT(""));
        setImage(evt.GetId(), Board[evt.GetId()]->album);
        Board[evt.GetId()]->turned = true;
        panelsTurned[numTurnedLeft+numTurnedRight] = evt.GetId();
        numTurnedRight++;
        if (numTurnedLeft+numTurnedRight==3) {
            Evaluate(wxT("user"));
        }
    }
}



void MemBoard::Evaluate(wxString player) {
// Function to evaluate the three cards picked by the user or the computer player.
// evaluate three cards turned by user
                gamePhase = wxT("Evaluate");
                srand(time(0));
                bool compInMemory;
                bool randRight;
                bool swapLeft;
                bool pairLeft;
                int albumsChosen[3] = {0, 0, 0};
                for (int count = 0; count < 3; count++) {
                    albumsChosen[count] = Board[panelsTurned[count]]->album;
                }
                bool threeEquals = false;
// triplet found without bonus cards
                if (albumsChosen[0]==albumsChosen[1] && albumsChosen[1]==albumsChosen[2]) {
                    threeEquals = true;
                    if (player=="user") {
                        setText(msgST, wxT("You found a triplet !"));
                    }
                    if (player=="comp") {
                        setText(msgST, wxT("STAN found a triplet !"));
                    }
                }
// triplet found with Venom bonus card
                if ( (albumsChosen[0]==albumsChosen[1] && int(albumsChosen[2]/100)==6 && albumsChosen[2]%2==0)
                    || (albumsChosen[0]==albumsChosen[2] && int(albumsChosen[1]/100)==6 && albumsChosen[1]%2==0)
                    || (albumsChosen[1]==albumsChosen[2] && int(albumsChosen[0]/100)==6 && albumsChosen[0]%2==0) ) {
                    threeEquals = true;
                    if (player=="user") {
                        setText(msgST, wxT("VENOM makes triplet for You !"));
                    }
                    if (player=="comp") {
                        setText(msgST, wxT("VENOM makes triplet for STAN !"));
                    }
                }
// triplet found with Bathory bonus card
                if ( (albumsChosen[0]==albumsChosen[1] && int(albumsChosen[2]/100)==6 && albumsChosen[2]%2==1)
                    || (albumsChosen[0]==albumsChosen[2] && int(albumsChosen[1]/100)==6 && albumsChosen[1]%2==1)
                    || (albumsChosen[1]==albumsChosen[2] && int(albumsChosen[0]/100)==6 && albumsChosen[0]%2==1) ) {
                    threeEquals = true;
                    if (player=="user") {
                        setText(msgST, wxT("BATHORY makes triplet for You !"));
                    }
                    if (player=="comp") {
                        setText(msgST, wxT("BATHORY makes triplet for STAN !"));
                    }
                }
// do this when triplet is found
                if  (threeEquals==true) {
                    trips_left--;
                    numTurnedLeft = 0;
                    numTurnedRight = 0;
                    if (player=="user") {
                        userScore++;
                    }
                    if (player=="comp") {
                        compScore++;
                    }
                    TripletFound();
                    wxString scoreText = wxT("Score:   You  ") + to_string(userScore) + wxT(" - ") + to_string(compScore) + wxT("  STAN");
                    setText(scoreST, scoreText);
// update computer memory
                    for (int count = 0; count < 3; count++) {
                        compMem[panelsTurned[count]] = -1;
                    }
// check if game is over
                    if (trips_left==0) {
                        GameOver();
                    }
                }
// do this when no triplet is found
                else {
                    numTurnedLeft = 0;
                    numTurnedRight = 0;
                    pairLeft = ( (albumsChosen[0]==albumsChosen[1] && panelsTurned[0]<boardsize*boardsize &&
                                       panelsTurned[1]<boardsize*boardsize) ||
                                     (albumsChosen[0]==albumsChosen[2] && panelsTurned[0]<boardsize*boardsize &&
                                       panelsTurned[2]<boardsize*boardsize) ||
                                     (albumsChosen[1]==albumsChosen[2] && panelsTurned[1]<boardsize*boardsize &&
                                       panelsTurned[2]<boardsize*boardsize) );
                    swapLeft = ( (int(albumsChosen[0]/100)==1 && panelsTurned[0]<boardsize*boardsize) ||
                                                (int(albumsChosen[1]/100)==1 && panelsTurned[1]<boardsize*boardsize) ||
                                                (int(albumsChosen[2]/100)==1 && panelsTurned[2]<boardsize*boardsize) );
                    randRight = ( (int(albumsChosen[0]/100)==8) || (int(albumsChosen[1]/100)==8) ||
                                                                                                            (int(albumsChosen[2]/100)==8) );
// turn cards over again, no randomizing, no swapping pair
                    if (randRight==false && (pairLeft==true || swapLeft==false)) {
                        wxMilliSleep(timeShow);
                        for (int count = 0; count < 3; count++) {
                            setImage(panelsTurned[count], 0);
                            Board[panelsTurned[count]]->turned = false;
                        }
                    }
// randomizing right board, turn left cards over again
                    if (randRight==true && (pairLeft==true || swapLeft==false)) {
                        wxMilliSleep(timeShow-300);
                        for (int count = 0; count < 3; count++) {
                                Board[panelsTurned[count]]->turned = false;
                        }
                        ShuffleRightBoard();
                        for (int count = 0; count < 3; count++) {
                            if (panelsTurned[count]<boardsize*boardsize) {
                                setImage(panelsTurned[count], 0);
                            }
                        }
                    }
// randomizing right board + swap left pair
                    if (randRight==true && pairLeft==false && swapLeft==true) {
                        wxMilliSleep(timeShow-300);
                        for (int count = 0; count < 3; count++) {
                                Board[panelsTurned[count]]->turned = false;
                        }
                        SwapPairLeftBoard();
                        ShuffleRightBoard();
                    }
// no randomizing, but swap left pair
                    if (randRight==false && pairLeft==false && swapLeft==true) {
                        wxMilliSleep(timeShow-300);
                        for (int count = 0; count < 3; count++) {
                                Board[panelsTurned[count]]->turned = false;
                        }
                        SwapPairLeftBoard();
                        for (int count = 0; count < 3; count++) {
                                if (panelsTurned[count]>=boardsize*boardsize) {
                                    setImage(panelsTurned[count], 0);
                                }
                        }
                    }
// update computer memory with probability compMemPerc/100
                for (int count = 0; count < 3; count++) {
                    compInMemory = (rand() % 100) <= compMemPerc;
                    if (compInMemory==true) {
                        compMem[panelsTurned[count]] = Board[panelsTurned[count]]->album;
                    }
                }
// erase computer memory of right board after reshuffling
                if (randRight==true) {
                    for (int count = 0; count < boardsize*boardsize; count++) {
                        if (Board[count+boardsize*boardsize]->album==0) {
                            compMem[count+boardsize*boardsize] = -1;
                        }
                        if (Board[count+boardsize*boardsize]->album>0) {
                            compMem[count+boardsize*boardsize] = 0;
                        }
                    }
                }
            }
// set game phase
                if (trips_left>0 && ((player=="user" && takeTurns==true)
                        || (player=="user" && threeEquals==false && takeTurns==false)
                        || (player=="comp" && threeEquals==true && takeTurns==false))) {
                    wxYield();
                    gamePhase = wxT("compMove");
                    setText(msgST, wxT("STAN makes his choice."));
                    compChoice();
                    compChoiceShow();
                }
                if (trips_left>0 && ((player=="comp" && takeTurns==true)
                        || (player=="comp" && threeEquals==false && takeTurns==false)
                        || (player=="user" && threeEquals==true && takeTurns==false))) {
                    wxYield();
                    gamePhase = wxT("userMove");
                    setText(msgST, wxT("You should now pick cards."));
                }
    }



void MemBoard::TripletFound() {
// put on a show when triplet has been found
    wxMilliSleep(200);
    for (int count = 0; count < 2; count++) {
        for (int count1 = 0; count1 < 3; count1++) {
                setImage(panelsTurned[count1], 999);
        }
        wxMilliSleep(200);
        for (int count1 = 0; count1 < 3; count1++) {
                setImage(panelsTurned[count1], Board[panelsTurned[count1]]->album);
        }
        wxMilliSleep(200);
    }
    for (int count1 = 0; count1 < 3; count1++) {
                setImage(panelsTurned[count1], 999);
    }
    wxMilliSleep(200);
    for (int count = 0; count < 3; count++) {
            Board[panelsTurned[count]]->album = 0;
            Board[panelsTurned[count]]->turned = false;
            Board[panelsTurned[count]]->Hide();
    }
}



void MemBoard::ShuffleRightBoard() {
// reshuffle cards in right board when Metallica album has been turned
    setText(msgST, wxT("METALLICA shuffles the right board !"));
    vector<int> albumsRight;
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (Board[count+boardsize*boardsize]->album>=0) {
            albumsRight.push_back(Board[count+boardsize*boardsize]->album);
            setImage(count+boardsize*boardsize, 999);
        }
    }
    shuffle(albumsRight.begin(), albumsRight.end(), default_random_engine(time(0)));
    wxMilliSleep(200);
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (Board[count+boardsize*boardsize]->album>=0) {
            setImage(count+boardsize*boardsize, 0);
        }
    }
    wxMilliSleep(200);
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (Board[count+boardsize*boardsize]->album>=0) {
            setImage(count+boardsize*boardsize, 999);
        }
    }
    wxMilliSleep(200);
    int count1 = 0;
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (Board[count+boardsize*boardsize]->album>=0) {
            Board[count+boardsize*boardsize]->album = albumsRight[count1];
            count1++;
        }
    }
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (Board[count+boardsize*boardsize]->album>0) {
            setImage(count+boardsize*boardsize, 0);
        }
        if (Board[count+boardsize*boardsize]->album==0) {
            Board[count+boardsize*boardsize]->Hide();
        }
    }
}



void MemBoard::SwapPairLeftBoard() {
// swap pair on left board when (at least) one of the albums turned is from Megadeth
    setText(msgST, wxT("MEGADETH swaps albums on the left !"));
    int pairPanels[2];
    int pairAlbums[2];
    int count1 = 0;
    for (int count = 0; count < 3; count++) {
        if (panelsTurned[count]<boardsize*boardsize) {
            pairPanels[count1] = panelsTurned[count];
            pairAlbums[count1] = Board[panelsTurned[count]]->album;
            count1++;
        }
    }
    for (int count = 0; count < 2; count++) {
            setImage(pairPanels[count], 999);
    }
    wxMilliSleep(200);
    Board[pairPanels[0]]->album = pairAlbums[1];
    Board[pairPanels[1]]->album = pairAlbums[0];
    for (int count = 0; count < 2; count++) {
            setImage(pairPanels[count], Board[pairPanels[count]]->album);
    }
    wxMilliSleep(500);
    for (int count = 0; count < 2; count++) {
            setImage(pairPanels[count], 999);
    }
    wxMilliSleep(200);
    for (int count = 0; count < 2; count++) {
            setImage(pairPanels[count], Board[pairPanels[count]]->album);
    }
    wxMilliSleep(500);
    for (int count = 0; count < 2; count++) {
            setImage(pairPanels[count], 0);
    }
}



void MemBoard::setText(wxStaticText *someST, wxString text) {
// Function to print text on the game board.
    someST->SetLabel(text);
    someST->Refresh();
    someST->Update();
}



void MemBoard::setImage(int numPanel, int numImg) {
// Function the change the image of a card on the board.
    int theNumber;
    if (numImg<=32) {
        theNumber = numImg;
    }
    else if (numImg>100 && numImg<600) {
        theNumber = 32 + numImg - 100;
    }
    else if (numImg>600 && numImg<800) {
        theNumber = 32 + 5 + numImg - 665;
    }
    else if (numImg>800 && numImg<900) {
        theNumber = 32 + 5 + 4 + numImg - 887;
    }
    else {
        theNumber = 32 + 5 + 4 + 2 + 1;
    }
    Board[numPanel]->setImage(imageVec[theNumber]);
}



void MemBoard::GameOver() {
    gamePhase = wxT("GameOver");
    if (userScore>compScore) {
        setText(msgST, wxT("Game Over: You won !"));
    }
    if (userScore<compScore) {
        setText(msgST, wxT("Game Over: STAN won !"));
    }
    if (userScore==compScore) {
        setText(msgST, wxT("Game Over: equal scores !"));
    }
}



vector<int> MemBoard::sort_indexes(vector<int> &vec) {
// Function to sort a vector and return the order of indexes in the sorted vector.
// initialize original index locations
   vector<int> idx(vec.size());
   iota(idx.begin(), idx.end(), 0);

// sort indexes based on comparing values in vec (lambda function as comparator)
   sort(idx.begin(), idx.end(), [&vec] (int i1, int i2) {return vec[i1] < vec[i2]; } );

   return idx;
}



void MemBoard::compMemPairUpdate() {
// identify pairs in left board in computer memory
    compMemPairs.clear();
    vector<int> memVec2(compMem.begin(), compMem.begin()+boardsize*boardsize);
    vector<int> ms_idx = sort_indexes(memVec2);
    for (int count = 0; count < boardsize*boardsize-1; count++) {
       if ( memVec2[ms_idx[count]]==memVec2[ms_idx[count+1]] && memVec2[ms_idx[count]]>0 ) {
            vector<int> temp = {ms_idx[count], ms_idx[count+1]};
            compMemPairs.push_back(temp);
       }
    }
}



void MemBoard::compMemTripUpdate() {
// identify triplets in computer memory
    compMemTrips.clear();
    for (int count = 0; count < compMemPairs.size(); count++) {
        int albumPair = compMem[compMemPairs[count][0]];
        for (int count1 = 0; count1 < boardsize*boardsize; count1++) {
            if (compMem[count1+boardsize*boardsize]==albumPair) {
                vector<int> temp = {compMemPairs[count][0], compMemPairs[count][1]};
                temp.push_back(count1+boardsize*boardsize);
                compMemTrips.push_back(temp);
            }
        }
    }
}



void MemBoard::compChoicePair() {
// panelsTurned[0,1] have been chosen (left) and form a pair - now choose card on the right
    int albumPair = Board[panelsTurned[0]]->album;
// check whether computer has matching triplet in memory
    for (int count = 0; count < boardsize*boardsize; count++) {
        if ( compMem[count+boardsize*boardsize]==albumPair ) {
            panelsTurned[2]= count+boardsize*boardsize;
            return;
        }
    }
// check whether computer has bonus card in memory
    for (int count = 0; count < boardsize*boardsize; count++) {
        if ( int(compMem[count+boardsize*boardsize]/100)==6 ) {
            panelsTurned[2]= count+boardsize*boardsize;
            return;
        }
    }
    compChoice2();
}



void MemBoard::compChoice2() {
// two cards (left) turned but no pair (or no triplet in memory) - now choose card on the right
// select first card in right board not in memory
    for (int count = 0; count < boardsize*boardsize; count++) {
        if ( compMem[count+boardsize*boardsize]==0 ) {
            panelsTurned[2]= count+boardsize*boardsize;
            return;
        }
    }
// select first card in right board that is present
    for (int count = 0; count < boardsize*boardsize; count++) {
        if ( compMem[count+boardsize*boardsize]>=0 ) {
            panelsTurned[2]= count+boardsize*boardsize;
            return;
        }
    }
}



void MemBoard::compChoice() {
    compMemPairUpdate();
    compMemTripUpdate();
// start with triplet in computer memory if it exists
    if (compMemTrips.size()>0) {
            for (int count = 0; count < 3; count++) {
                panelsTurned[count] = compMemTrips[0][count];
            }
// if so, then turn first card
            if ( Board[panelsTurned[0]]->album==compMem[panelsTurned[0]]) {
// if memory matches first card, then turn second card
                if ( Board[panelsTurned[1]]->album==compMem[panelsTurned[1]]) {
// if memory matches second card, then a pair is found - choose third card from memory
                    return;
                }
// if memory does not match second card, then two unequal cards are turned - choose third card
                else {
                    compChoice2();
                    return;
                }
            }
// if memory does not match first card, try to find a new pair in memory
            else {
                for (int count3 = 0; count3 < boardsize*boardsize-1; count3++) {
                    if (Board[panelsTurned[0]]->album==compMem[count3] && count3!=panelsTurned[0]) {
// if so, then turn second card
                        panelsTurned[1] = count3;
                        if (Board[panelsTurned[1]]->album==compMem[panelsTurned[1]]) {
// if memory matches second card, then a pair is found - choose third card
                            compChoicePair();
                            return;
                        }
// if memory does not match second card, then two unequal cards are turned - choose third card
                        else {
                            compChoice2();
                            return;
                        }
                    }
                }
            }
    }
// if no triplet in memory: check if endgame strategy is needed
// if number of pairs (left) in memory is greater than number of unknown pairs (left), then do endgame strategy
    if (compMemPairs.size()>trips_left-compMemPairs.size() || trips_left-compMemPairs.size()==1) {
        compChoiceEndGame();
        return;
    }
// if no triplet in memory and no endgame: check whether computer has two identical cards in left board memory
    if (compMemPairs.size()>0) {
// if so, then turn first card
            panelsTurned[0] = compMemPairs[0][0];
            if ( Board[panelsTurned[0]]->album==compMem[panelsTurned[0]]) {
// if memory matches first card, then turn second card
                panelsTurned[1] = compMemPairs[0][1];
                if ( Board[panelsTurned[1]]->album==compMem[panelsTurned[1]]) {
// if memory matches second card, then a pair is found - choose third card
                    compChoicePair();
                    return;
                }
// if memory does not match second card, then two unequal cards are turned - choose third card
                else {
                    compChoice2();
                    return;
                }
            }
// if memory does not match first card, try to find a new pair in memory
            else {
                for (int count3 = 0; count3 < boardsize*boardsize-1; count3++) {
                    if (Board[panelsTurned[0]]->album==compMem[count3] && count3!=panelsTurned[0]) {
// if so, then turn second card
                        panelsTurned[1] = count3;
                        if (Board[panelsTurned[1]]->album==compMem[panelsTurned[1]]) {
// if memory matches second card, then a pair is found - choose third card
                            compChoicePair();
                            return;
                        }
// if memory does not match second card, then two unequal cards are turned - choose third card
                        else {
                            compChoice2();
                            return;
                        }
                    }
                }
            }
       }
// no identical cards in memory ==> pick the first two available cards not in memory
    for (int count1 = 0; count1 < boardsize*boardsize; count1++) {
        if (compMem[count1]==0 ) {
            panelsTurned[0] = count1;
// check for pair in memory after first card is turned
            for (int count2 = 0; count2 < boardsize*boardsize; count2++) {
                if ( Board[panelsTurned[0]]->album==compMem[count2] && panelsTurned[0]!=count2 ) {
                        panelsTurned[1] = count2;
                        if ( Board[panelsTurned[1]]->album==compMem[panelsTurned[1]]) {
// if memory matches second card, then a pair is found - choose third card
                            compChoicePair();
                            return;
                        }
// if memory does not match second card, then two unequal cards are turned - choose third card
                        else {
                            compChoice2();
                            return;
                        }
                }
            }
// otherwise pick first available card not in memory
            for (int count2 = count1+1; count2 < boardsize*boardsize; count2++) {
                if (compMem[count2]==0 ) {
                    panelsTurned[1] = count2;
// check if two turned cards form a pair
                    if (Board[panelsTurned[0]]->album==Board[panelsTurned[1]]->album) {
                        compChoicePair();
                        return;
                    }
// otherwise two unequal cards are turned - choose third card
                    else {
                        compChoice2();
                        return;
                    }
                }
            }
        }
    }
// no two cards not in memory ==> pick the first two available cards (at least one of which not in memory)
    for (int count1 = 0; count1 < boardsize*boardsize; count1++) {
        if ( compMem[count1]==0 ) {
            panelsTurned[0] = count1;
// check for pair in memory after first card is turned
            for (int count2 = 0; count2 < boardsize*boardsize; count2++) {
                if ( Board[panelsTurned[0]]->album==compMem[count2] && panelsTurned[0]!=count2 ) {
                    panelsTurned[1] = count2;
                    if ( Board[panelsTurned[1]]->album==compMem[panelsTurned[1]]) {
// if memory matches second card, then a pair is found - choose third card
                            compChoicePair();
                            return;
                        }
// if memory does not match second card, then two unequal cards are turned - choose third card
                        else {
                            compChoice2();
                            return;
                        }
                }
            }
// otherwise pick card that is in memory
            for (int count2 = 0; count2 < boardsize*boardsize; count2++) {
                if ( count2!=panelsTurned[0] && compMem[count2]>=0 ) {
                    panelsTurned[1] = count2;
// check if two turned cards form a pair
                    if (Board[panelsTurned[0]]->album==Board[panelsTurned[1]]->album) {
                        compChoicePair();
                        return;
                    }
// otherwise two unequal cards are turned - choose third card
                    else {
                        compChoice2();
                        return;
                    }
                }
            }
        }
    }
}



void MemBoard::compChoiceEndGame() {
// different strategy for endgame: first pick card in right board (if no triplets are in computer memory)
// first, add pair (left) to memory of only one pair is unknown
    bool unknownPair = false;
    if (trips_left-compMemPairs.size()==1) {
        vector<int> newPair;
        for (int count = 0; count < boardsize*boardsize; count++) {
            bool cardInPair = false;
            if (compMem[count]>=0) {
                for (int count2 = 0; count2 < compMemPairs.size(); count2++) {
                    if (compMemPairs[count2][0]==count || compMemPairs[count2][1]==count) {
                        cardInPair = true;
                    }
                }
                if (cardInPair==false) {
                    newPair.push_back(count);
                }
            }
        }
    compMemPairs.push_back(newPair);
    unknownPair = true;
    }
// pick bonus card in right board if it exists in computer memory
    for (int count1 = 0; count1 < boardsize*boardsize; count1++) {
        if (int(compMem[count1+boardsize*boardsize]/100)==6) {
            panelsTurned[0] = count1+boardsize*boardsize;
            compChoiceLeft();
            return;
        }
    }
// if no bonus card in memory, then pick first card in right board not in memory (since we have no triplet)
    for (int count1 = 0; count1 < boardsize*boardsize; count1++) {
        if (compMem[count1+boardsize*boardsize]==0) {
            panelsTurned[0] = count1+boardsize*boardsize;
            compChoiceLeft();
            return;
        }
    }
}



void MemBoard::compChoiceLeft() {
// in endgame, after picking right card, now pick two cards in left board
    int albumRight = Board[panelsTurned[0]]->album;
// in case of bonus card, take first pair (left)
    if (int(albumRight/100)==6) {
        panelsTurned[1] = compMemPairs[0][0];
        if (compMem[panelsTurned[1]]==Board[panelsTurned[1]]->album) {
            panelsTurned[2] = compMemPairs[0][1];
            return;
        }
        else {
            compChoiceLeftLastCard();
            return;
        }
    }
// if no bonus card, then check for matching pair (left) in memory
    for (int count = 0; count < compMemPairs.size(); count++) {
        if (compMem[compMemPairs[count][0]]==albumRight) {
            panelsTurned[1] = compMemPairs[count][0];
            if (compMem[panelsTurned[1]]==Board[panelsTurned[1]]->album) {
                panelsTurned[2] = compMemPairs[count][1];
                return;
            }
            else {
                compChoiceLeftLastCard();
                return;
            }
        }
    }
// if no matching pair (left), then check for matching card (left) in memory
// NB: if matching card turns out incorrect, then still try to find match to turned card (left)
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (compMem[count]==albumRight) {
            panelsTurned[1] = count;
            compChoiceLeftLastCard();
            return;
        }
    }
// if no matching card (left) in memory exists, then pick a card not in memory
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (compMem[count]==0) {
            panelsTurned[1] = count;
            compChoiceLeftLastCard();
            return;
        }
    }
// if no cards not in memory exist, then pick a card not in a pair
    for (int count = 0; count < boardsize*boardsize; count++) {
        bool cardInPair = false;
        if (compMem[count]>=0) {
            for (int count2 = 0; count2 < compMemPairs.size(); count2++) {
                if (compMemPairs[count2][0]==count || compMemPairs[count2][1]==count) {
                    cardInPair = true;
                }
            }
            if (cardInPair==false) {
                panelsTurned[1] = count;
                compChoiceLeftLastCard();
                return;
            }
        }
    }
// if still no card (left) picked, then pick first card that is present
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (compMem[count]>=0) {
            panelsTurned[1] = count;
            compChoiceLeftLastCard();
            return;
        }
    }
}



void MemBoard::compChoiceLeftLastCard() {
// panelsTurned[0,1] have been chosen (right, left)
// if two turned cards form a pair, the find matching left card to panelsTurned[1]
if (Board[panelsTurned[0]]->album==Board[panelsTurned[1]]->album) {
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (compMem[count]==Board[panelsTurned[1]]->album && count!=panelsTurned[1]) {
            panelsTurned[2] = count;
            return;
        }
    }
// if no match exists in memory, then pick first card (left) not in memory
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (compMem[count]==0 && count!=panelsTurned[1]) {
            panelsTurned[2] = count;
            return;
        }
    }
// if no cards (left) not in memory, then pick first card (left) present
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (compMem[count]>=0 && count!=panelsTurned[1]) {
            panelsTurned[2] = count;
            return;
        }
    }
}
// otherwise pick first card not matching panelsTurned[1] in memory (to not give away information on pair)
else {
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (compMem[count]>0 && compMem[count]!=Board[panelsTurned[1]]->album && count!=panelsTurned[1]) {
            panelsTurned[2] = count;
            return;
        }
    }
// if this does not yield a card, then pick first card (left) not in memory
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (compMem[count]==0 && count!=panelsTurned[1]) {
            panelsTurned[2] = count;
            return;
        }
    }
// as final option, pick first card (left) present
    for (int count = 0; count < boardsize*boardsize; count++) {
        if (compMem[count]>=0 && count!=panelsTurned[1]) {
            panelsTurned[2] = count;
            return;
        }
    }
}
}


void MemBoard::compChoiceShow() {
// Function to turn the three cards selected by the computer player.
    for (int count = 0; count <3; count++) {
        wxMilliSleep(1000);
        setImage(panelsTurned[count], Board[panelsTurned[count]]->album);
    }
    Evaluate(wxT("comp"));
}
