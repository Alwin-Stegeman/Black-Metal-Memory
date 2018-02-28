#include "Memory_Main.h"

// Black Metal Memory is a memory game with some funny twists.
// The cards are black metal album covers and the user should pick three identical cards to form a triplet and score a point.
// Your computer opponent is STAN whose memory strength can be set by the user.
// Also the game board size and turn order can be set by the user.

IMPLEMENT_APP(BMMemory)

bool BMMemory::OnInit()
{

        MemBoard *MB = new MemBoard();

        return true;
}







