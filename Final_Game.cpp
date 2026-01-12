#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

    // Textures
    sf::Texture king1Texture, king2Texture;
    sf::Texture queen1Texture, queen2Texture;
    sf::Texture rook1Texture, rook2Texture;
    sf::Texture bishop1Texture, bishop2Texture;
    sf::Texture knight1Texture, knight2Texture;
    sf::Texture pawn1Texture, pawn2Texture;

    // Sprites
    sf::Sprite king1, king2;
    sf::Sprite queen1, queen2;
    sf::Sprite rook1[2], rook2[2];
    sf::Sprite bishop1[2], bishop2[2];
    sf::Sprite knight1[2], knight2[2];
    sf::Sprite pawn1[8], pawn2[8];

    // Board representation
    string board[8][8];

    // Track which pawn index is at each position
    int pawnIndexAt[8][8];

    // Track which rook index is at each position
    int rookIndexAt[8][8];

    // Track which bishop index is at each position
    int bishopIndexAt[8][8];

    // Track which queen index is at each position
    int queenIndexAt[8][8];

    // Track which knight index is at each position
    int knightIndexAt[8][8];

    // Track if pieces are captured
    bool pawn1Captured[8] = { false };
    bool pawn2Captured[8] = { false };
    bool rook1Captured[2] = { false };
    bool rook2Captured[2] = { false };
    bool bishop1Captured[2] = { false };
    bool bishop2Captured[2] = { false };

    // Track if queens are captured
    bool queen1Captured = false;
    bool queen2Captured = false;

    // Track if knights are captured
    bool knight1Captured[2] = { false };
    bool knight2Captured[2] = { false };

    // Track if kings are captured
    bool king1Captured = false;
    bool king2Captured = false;

    // Movement tracking
    int startRow = -1, startCol = -1;

    //Highlighting the box yellow if selected
    sf::RectangleShape selectedHighlight;

    //Highlighting the box red if check is meet
    sf::RectangleShape checkHighlight;

    // Turn tracking
    bool isWhiteTurn = true;

    // Promoted queens (max 8 per side since max 8 pawns can promote)
    sf::Sprite promotedQueen1[8];  // Black promoted queens
    sf::Sprite promotedQueen2[8];  // White promoted queens
    bool promotedQueen1Captured[8] = { false };
    bool promotedQueen2Captured[8] = { false };
    int promotedQueen1Count = 0;  // How many black queens have been promoted
    int promotedQueen2Count = 0;  // How many white queens have been promoted

    bool gameOver = false;  //To end the game when a checkmate or stalemate is met

    // Player scores
    int whiteScore = 0;
    int blackScore = 0;

    void chess_pieces()
    {
        // Load textures
        king1Texture.loadFromFile("images/king1.png");
        king2Texture.loadFromFile("images/king2.png");
        queen1Texture.loadFromFile("images/queen1.png");
        queen2Texture.loadFromFile("images/queen2.png");
        rook1Texture.loadFromFile("images/rook1.jpg");
        rook2Texture.loadFromFile("images/rook2.jpg");
        bishop1Texture.loadFromFile("images/bishop1.png");
        bishop2Texture.loadFromFile("images/bishop2.png");
        knight1Texture.loadFromFile("images/knight1.png");
        knight2Texture.loadFromFile("images/knight2.png");
        pawn1Texture.loadFromFile("images/pawn1.png");
        pawn2Texture.loadFromFile("images/pawn2.png");

        // Set textures
        king1.setTexture(king1Texture);
        king2.setTexture(king2Texture);
        queen1.setTexture(queen1Texture);
        queen2.setTexture(queen2Texture);

        for (int i = 0; i < 2; i++)
        {
            rook1[i].setTexture(rook1Texture);
            rook2[i].setTexture(rook2Texture);
            bishop1[i].setTexture(bishop1Texture);
            bishop2[i].setTexture(bishop2Texture);
            knight1[i].setTexture(knight1Texture);
            knight2[i].setTexture(knight2Texture);
        }

        // Pawn positions
        for (int i = 0; i < 8; i++)
        {
            pawn1[i].setTexture(pawn1Texture);
            pawn1[i].setPosition(i * 75, 1 * 75);
        }

        for (int i = 0; i < 8; i++)
        {
            pawn2[i].setTexture(pawn2Texture);
            pawn2[i].setPosition(i * 75, 6 * 75);
        }

        // Major pieces (black)
        king1.setPosition(4 * 75, 0 * 75);
        queen1.setPosition(3 * 75, 0 * 75);
        rook1[0].setPosition(0 * 75, 0 * 75);
        rook1[1].setPosition(7 * 75, 0 * 75);
        knight1[0].setPosition(1 * 75, 0 * 75);
        knight1[1].setPosition(6 * 75, 0 * 75);
        bishop1[0].setPosition(2 * 75, 0 * 75);
        bishop1[1].setPosition(5 * 75, 0 * 75);

        // Major pieces (white)
        king2.setPosition(4 * 75, 7 * 75);
        queen2.setPosition(3 * 75, 7 * 75);
        rook2[0].setPosition(0 * 75, 7 * 75);
        rook2[1].setPosition(7 * 75, 7 * 75);
        knight2[0].setPosition(1 * 75, 7 * 75);
        knight2[1].setPosition(6 * 75, 7 * 75);
        bishop2[0].setPosition(2 * 75, 7 * 75);
        bishop2[1].setPosition(5 * 75, 7 * 75);
    }


void chess_pieces_draw(sf::RenderWindow& window)
{
    // Draw pawns (only if not captured)
    for (int i = 0; i < 8; i++)
        if (!pawn1Captured[i])
            window.draw(pawn1[i]);
    for (int i = 0; i < 8; i++)
        if (!pawn2Captured[i])
            window.draw(pawn2[i]);

    // Draw rooks (only if not captured)
    for (int i = 0; i < 2; i++)
        if (!rook1Captured[i])
            window.draw(rook1[i]);
    for (int i = 0; i < 2; i++)
        if (!rook2Captured[i])
            window.draw(rook2[i]);

    // Draw bishops (only if not captured)
    for (int i = 0; i < 2; i++)
        if (!bishop1Captured[i])
            window.draw(bishop1[i]);
    for (int i = 0; i < 2; i++)
        if (!bishop2Captured[i])
            window.draw(bishop2[i]);

    // Draw kings
    if (!king1Captured)
        window.draw(king1);
    if (!king2Captured)
        window.draw(king2);

    // Draw queens (only if not captured)
    if (!queen1Captured)
        window.draw(queen1);
    if (!queen2Captured)
        window.draw(queen2);

    // Draw knights (only if not captured)
    for (int i = 0; i < 2; i++)
        if (!knight1Captured[i])
            window.draw(knight1[i]);
    for (int i = 0; i < 2; i++)
        if (!knight2Captured[i])
            window.draw(knight2[i]);

    // Draw promoted queens
    for (int i = 0; i < promotedQueen1Count; i++)
        if (!promotedQueen1Captured[i])
            window.draw(promotedQueen1[i]);

    for (int i = 0; i < promotedQueen2Count; i++)
        if (!promotedQueen2Captured[i])
            window.draw(promotedQueen2[i]);
}

void initialize_board_names()
{
    // Clear board and piece tracking
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = "";
            pawnIndexAt[i][j] = -1;
            rookIndexAt[i][j] = -1;
            bishopIndexAt[i][j] = -1;
            queenIndexAt[i][j] = -1;  // Clear queen tracking
            knightIndexAt[i][j] = -1;  // Clear knight tracking
        }

    // Initialize pawns with their index tracking
    for (int i = 0; i < 8; i++)
    {
        board[1][i] = "Black Pawn";
        pawnIndexAt[1][i] = i;

        board[6][i] = "White Pawn";
        pawnIndexAt[6][i] = i;
    }

    // Major pieces
    board[0][0] = "Black Rook";
    rookIndexAt[0][0] = 0;

    board[0][7] = "Black Rook";
    rookIndexAt[0][7] = 1;

    board[0][1] = "Black Knight";
    knightIndexAt[0][1] = 0;

    board[0][6] = "Black Knight";
    knightIndexAt[0][6] = 1;

    board[0][2] = "Black Bishop";
    bishopIndexAt[0][2] = 0;

    board[0][5] = "Black Bishop";
    bishopIndexAt[0][5] = 1;

    board[0][4] = "Black King";
    board[0][3] = "Black Queen";
    queenIndexAt[0][3] = 0;     // Track black queen

    board[7][0] = "White Rook";
    rookIndexAt[7][0] = 0;

    board[7][7] = "White Rook";
    rookIndexAt[7][7] = 1;

    board[7][1] = "White Knight";
    knightIndexAt[7][1] = 0;

    board[7][6] = "White Knight";
    knightIndexAt[7][6] = 1;

    board[7][2] = "White Bishop";
    bishopIndexAt[7][2] = 0;

    board[7][5] = "White Bishop";
    bishopIndexAt[7][5] = 1;

    board[7][4] = "White King";
    board[7][3] = "White Queen";
    queenIndexAt[7][3] = 0;  // Track white queen
}

bool pawnMove(string piece, int startRow, int startCol, int endRow, int endCol)
{
    int direction = (piece == "White Pawn") ? -1 : 1;

    // Forward move
    if (endCol == startCol && board[endRow][endCol] == "")
    {
        if (endRow == startRow + direction)
            return true;
        // Condition for 2 steps move in the first movement of pawn
        if ((startRow == 6 && piece == "White Pawn") || (startRow == 1 && piece == "Black Pawn"))
        {
            if (endRow == startRow + 2 * direction && board[startRow + direction][startCol] == "")
                return true;
        }
    }

    // Diagonal capture
    if ((endCol == startCol + 1 || endCol == startCol - 1) && endRow == startRow + direction)
    {
        if (board[endRow][endCol] != "")
        {
            if (piece == "White Pawn" && board[endRow][endCol][0] == 'B')
            {
                return true;
            }
            if (piece == "Black Pawn" && board[endRow][endCol][0] == 'W')
            {
                return true;
            }
        }
    }

    return false;
}

bool rookMove(string piece, int startRow, int startCol, int endRow, int endCol)
{
    // Check if move is horizontal (same row, different column)
    if (startRow == endRow && startCol != endCol)
    {
        int step = (endCol > startCol) ? 1 : -1;

        for (int col = startCol + step; col != endCol; col += step)
        {
            if (board[startRow][col] != "")
                return false;
        }

        if (board[endRow][endCol] == "")
            return true;

        //Destination can be empty or contain an opponent piece
        if (piece[0] == 'W' && board[endRow][endCol][0] == 'B')
            return true;
        if (piece[0] == 'B' && board[endRow][endCol][0] == 'W')
            return true;
    }

    // Check if move is vertical (same column, different row)
    if (startCol == endCol && startRow != endRow)
    {
        int step = (endRow > startRow) ? 1 : -1;

        for (int row = startRow + step; row != endRow; row += step)
        {
            if (board[row][startCol] != "")
                return false;
        }

        if (board[endRow][endCol] == "")
            return true;

        if (piece[0] == 'W' && board[endRow][endCol][0] == 'B')
            return true;
        if (piece[0] == 'B' && board[endRow][endCol][0] == 'W')
            return true;
    }

    return false;
}

bool bishopMove(string piece, int startRow, int startCol, int endRow, int endCol)
{
    int rowDiff = endRow - startRow;
    int colDiff = endCol - startCol;

    if (abs(rowDiff) != abs(colDiff))
        return false;

    if (rowDiff == 0)
        return false;

    int rowStep = (rowDiff > 0) ? 1 : -1;
    int colStep = (colDiff > 0) ? 1 : -1;

    int currentRow = startRow + rowStep;
    int currentCol = startCol + colStep;

    while (currentRow != endRow && currentCol != endCol)
    {
        if (board[currentRow][currentCol] != "")
            return false;

        currentRow += rowStep;
        currentCol += colStep;
    }

    if (board[endRow][endCol] == "")
        return true;

    if (piece[0] == 'W' && board[endRow][endCol][0] == 'B')
        return true;
    if (piece[0] == 'B' && board[endRow][endCol][0] == 'W')
        return true;

    return false;
}

// Queen movement function
bool queenMove(string piece, int startRow, int startCol, int endRow, int endCol)
{
    // Queen can move like a rook OR like a bishop

    if (rookMove(piece, startRow, startCol, endRow, endCol))
        return true;  // Valid rook like move

    if (bishopMove(piece, startRow, startCol, endRow, endCol))
        return true;  // Valid bishop like move

    return false;  // Not a valid queen move
}

// NEW: Knight movement function
bool knightMove(string piece, int startRow, int startCol, int endRow, int endCol)
{
    // Knight moves in an "L" shape: 2 squares in one direction, 1 square perpendicular

    // Calculate how many rows and columns the knight is trying to move
    int rowDiff = abs(endRow - startRow);  // Absolute difference in rows
    int colDiff = abs(endCol - startCol);  // Absolute difference in columns

    // Valid knight move: (2 rows, 1 column) OR (1 row, 2 columns)
    bool isLShape = (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);

    // If it's not an L-shape, it's invalid
    if (!isLShape)
        return false;

    // Knight can jump over pieces, so we don't need to check the path!

    // Check if destination is empty
    if (board[endRow][endCol] == "")
        return true;

    // Check if destination has opponents piece (capture)
    if (piece[0] == 'W' && board[endRow][endCol][0] == 'B')
        return true;  // White knight captures black piece
    if (piece[0] == 'B' && board[endRow][endCol][0] == 'W')
        return true;  // Black knight captures white piece

    return false;  // Destination has same color piece
}

// NEW: King movement function
bool kingMove(string piece, int startRow, int startCol, int endRow, int endCol)
{
    // King can move 1 square in ANY direction (8 directions total)
    // Directions: up, down, left, right, and 4 diagonals

    // Calculate how many rows and columns the king is trying to move
    int rowDiff = abs(endRow - startRow);  // count rows
    int colDiff = abs(endCol - startCol);  // count columns

    // Check if move is more than 1 square
    if (rowDiff > 1 || colDiff > 1)
        return false;  // King can't move that far!

    // Check if king is not moving at all
    if (rowDiff == 0 && colDiff == 0)
        return false;  // King must move somewhere!

    // Check destination square
    if (board[endRow][endCol] == "")
        return true;  // Empty square - valid move

    // Check if capturing opponent's piece
    if (piece[0] == 'W' && board[endRow][endCol][0] == 'B')
        return true;  // White king captures black piece
    if (piece[0] == 'B' && board[endRow][endCol][0] == 'W')
        return true;  // Black king captures white piece

    return false;  // Destination has same color piece
}

void addScoreForCapture(string capturedPiece)
{
    if (capturedPiece == "") return;

    // If white captured a black piece
    if (capturedPiece[0] == 'B')
        whiteScore++;

    // If black captured a white piece
    if (capturedPiece[0] == 'W')
        blackScore++;

    cout << "White Score: " << whiteScore << " | Black Score: " << blackScore << endl;
}

// Function to check if a position is under attack by the opponent
bool isSquareUnderAttack(int row, int col, bool byWhite)
{
    // Check all opponent pieces to see if they can attack this square

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            string piece = board[i][j];

            // Skip empty squares
            if (piece == "") continue;

            // Check if this is an opponent's piece
            bool isOpponentPiece = (byWhite && piece[0] == 'W') || (!byWhite && piece[0] == 'B');

            if (!isOpponentPiece) continue;

            // Check if this piece can attack the target square
            bool canAttack = false;

            if (piece == "White Pawn" || piece == "Black Pawn")
            {
                // Pawns attack diagonally (different from how they move forward)
                int direction = (piece == "White Pawn") ? -1 : 1;

                // Check if target square is diagonally adjacent to pawn
                if (row == i + direction && (col == j + 1 || col == j - 1))
                    canAttack = true;
            }
            else if (piece == "White Rook" || piece == "Black Rook")
            {
                canAttack = rookMove(piece, i, j, row, col);
            }
            else if (piece == "White Bishop" || piece == "Black Bishop")
            {
                canAttack = bishopMove(piece, i, j, row, col);
            }
            else if (piece == "White Queen" || piece == "Black Queen")
            {
                canAttack = queenMove(piece, i, j, row, col);
            }
            else if (piece == "White Knight" || piece == "Black Knight")
            {
                canAttack = knightMove(piece, i, j, row, col);
            }
            else if (piece == "White King" || piece == "Black King")
            {
                canAttack = kingMove(piece, i, j, row, col);
            }

            if (canAttack)
                return true; // This square is under attack!
        }
    }

    return false; // Square is safe
}

// Function to check if a specific king is in check
bool isKingInCheck(bool whiteKing)
{
    // Find the king's position on the board
    int kingRow = -1, kingCol = -1;
    string kingName = whiteKing ? "White King" : "Black King";

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] == kingName)
            {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1) break;  // King has been found, so we break the loop
    }

    // Check if king's position is under attack by opponent
    return isSquareUnderAttack(kingRow, kingCol, !whiteKing);
}

// Function to check if a move is legal (doesn't leave own king in check)

bool isMoveLegal(int startRow, int startCol, int endRow, int endCol)
{
    /*This function temporarily makes the king move from its original place and check if that move doesn't 
     leave the king in danger*/
    
    // Save the current state
    string originalStartPiece = board[startRow][startCol];
    string originalEndPiece = board[endRow][endCol];

    // Save index tracking for captured piece
    int capturedPawnIdx = pawnIndexAt[endRow][endCol];
    int capturedRookIdx = rookIndexAt[endRow][endCol];
    int capturedBishopIdx = bishopIndexAt[endRow][endCol];
    int capturedQueenIdx = queenIndexAt[endRow][endCol];
    int capturedKnightIdx = knightIndexAt[endRow][endCol];

    // Save moving piece index tracking
    int movingPawnIdx = pawnIndexAt[startRow][startCol];
    int movingRookIdx = rookIndexAt[startRow][startCol];
    int movingBishopIdx = bishopIndexAt[startRow][startCol];
    int movingQueenIdx = queenIndexAt[startRow][startCol];
    int movingKnightIdx = knightIndexAt[startRow][startCol];

    // Temporarily make the move
    board[endRow][endCol] = board[startRow][startCol];
    board[startRow][startCol] = "";

    // Update index tracking for moving piece
    if (originalStartPiece == "White Pawn" || originalStartPiece == "Black Pawn")
    {
        pawnIndexAt[endRow][endCol] = movingPawnIdx;
        pawnIndexAt[startRow][startCol] = -1;
    }
    else if (originalStartPiece == "White Rook" || originalStartPiece == "Black Rook")
    {
        rookIndexAt[endRow][endCol] = movingRookIdx;
        rookIndexAt[startRow][startCol] = -1;
    }
    else if (originalStartPiece == "White Bishop" || originalStartPiece == "Black Bishop")
    {
        bishopIndexAt[endRow][endCol] = movingBishopIdx;
        bishopIndexAt[startRow][startCol] = -1;
    }
    else if (originalStartPiece == "White Queen" || originalStartPiece == "Black Queen")
    {
        queenIndexAt[endRow][endCol] = movingQueenIdx;
        queenIndexAt[startRow][startCol] = -1;
    }
    else if (originalStartPiece == "White Knight" || originalStartPiece == "Black Knight")
    {
        knightIndexAt[endRow][endCol] = movingKnightIdx;
        knightIndexAt[startRow][startCol] = -1;
    }

    // Determine which king to check based on the piece that's moving
    bool checkWhiteKing = (originalStartPiece[0] == 'W');
    bool kingInCheck = isKingInCheck(checkWhiteKing);

    // Undo the move (restore original state)
    board[startRow][startCol] = originalStartPiece;
    board[endRow][endCol] = originalEndPiece;

    // Restore index tracking for moving piece
    if (originalStartPiece == "White Pawn" || originalStartPiece == "Black Pawn")
    {
        pawnIndexAt[startRow][startCol] = movingPawnIdx;
        pawnIndexAt[endRow][endCol] = capturedPawnIdx;
    }
    else if (originalStartPiece == "White Rook" || originalStartPiece == "Black Rook")
    {
        rookIndexAt[startRow][startCol] = movingRookIdx;
        rookIndexAt[endRow][endCol] = capturedRookIdx;
    }
    else if (originalStartPiece == "White Bishop" || originalStartPiece == "Black Bishop")
    {
        bishopIndexAt[startRow][startCol] = movingBishopIdx;
        bishopIndexAt[endRow][endCol] = capturedBishopIdx;
    }
    else if (originalStartPiece == "White Queen" || originalStartPiece == "Black Queen")
    {
        queenIndexAt[startRow][startCol] = movingQueenIdx;
        queenIndexAt[endRow][endCol] = capturedQueenIdx;
    }
    else if (originalStartPiece == "White Knight" || originalStartPiece == "Black Knight")
    {
        knightIndexAt[startRow][startCol] = movingKnightIdx;
        knightIndexAt[endRow][endCol] = capturedKnightIdx;
    }

    // Return true if move is legal (doesn't leave king in check)
    return !kingInCheck;
}

// Function to check if current player has any legal moves
bool hasAnyLegalMoves(bool whitePlayer)
{
    // Loop through all squares on the board
    for (int startRow = 0; startRow < 8; startRow++)
    {
        for (int startCol = 0; startCol < 8; startCol++)
        {
            string piece = board[startRow][startCol];

            // Skip empty squares
            if (piece == "") continue;

            // Check if this is the current player's piece
            bool isWhitePiece = (piece[0] == 'W');
            if (isWhitePiece != whitePlayer) continue;

            // Try all possible destination squares
            for (int endRow = 0; endRow < 8; endRow++)
            {
                for (int endCol = 0; endCol < 8; endCol++)
                {
                    // Skip if trying to move to same position
                    if (startRow == endRow && startCol == endCol) continue;

                    // Check if this move is valid according to piece rules
                    bool validMove = false;

                    if (piece == "White Pawn" || piece == "Black Pawn")
                    {
                        validMove = pawnMove(piece, startRow, startCol, endRow, endCol);
                    }
                    else if (piece == "White Rook" || piece == "Black Rook")
                    {
                        validMove = rookMove(piece, startRow, startCol, endRow, endCol);
                    }
                    else if (piece == "White Bishop" || piece == "Black Bishop")
                    {
                        validMove = bishopMove(piece, startRow, startCol, endRow, endCol);
                    }
                    else if (piece == "White Queen" || piece == "Black Queen")
                    {
                        validMove = queenMove(piece, startRow, startCol, endRow, endCol);
                    }
                    else if (piece == "White Knight" || piece == "Black Knight")
                    {
                        validMove = knightMove(piece, startRow, startCol, endRow, endCol);
                    }
                    else if (piece == "White King" || piece == "Black King")
                    {
                        validMove = kingMove(piece, startRow, startCol, endRow, endCol);
                    }

                    // If move is valid, check if it's legal (doesn't leave king in check)
                    if (validMove && isMoveLegal(startRow, startCol, endRow, endCol))
                    {
                        return true; // Found a legal move!
                    }
                }
            }
        }
    }

    return false; // No legal moves found
}

bool PawnPromotion(int startRow, int startCol, int endRow, int endCol)
{
    string piece = board[startRow][startCol];

    // Check if pawn reached the opposite end
    if (piece == "White Pawn" && endRow == 0)
    {
        cout << "White Pawn promoted to Queen!" << endl;

        // Get the pawn index
        int pawnIdx = pawnIndexAt[startRow][startCol];
        pawn2Captured[pawnIdx] = true;  // Remove the pawn

        // Create new promoted queen
        promotedQueen2[promotedQueen2Count].setTexture(queen2Texture);
        promotedQueen2[promotedQueen2Count].setPosition(endCol * 75, endRow * 75);

        // Update board
        board[endRow][endCol] = "White Queen";
        queenIndexAt[endRow][endCol] = -(promotedQueen2Count + 1);  // Negative to mark as promoted

        promotedQueen2Count++;
        return true;
    }
    else if (piece == "Black Pawn" && endRow == 7)
    {
        cout << "Black Pawn promoted to Queen!" << endl;

        // Get the pawn index
        int pawnIdx = pawnIndexAt[startRow][startCol];
        pawn1Captured[pawnIdx] = true;  // Remove the pawn

        // Create new promoted queen
        promotedQueen1[promotedQueen1Count].setTexture(queen1Texture);
        promotedQueen1[promotedQueen1Count].setPosition(endCol * 75, endRow * 75);

        // Update board
        board[endRow][endCol] = "Black Queen";
        queenIndexAt[endRow][endCol] = -(promotedQueen1Count + 1);  // Negative to mark as promoted

        promotedQueen1Count++;
        return true;
    }

    return false;
}

void mouse_click(sf::Event& event)
{
    if (gameOver)
    {
        return;
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
        int x = event.mouseButton.x;
        int y = event.mouseButton.y;
        int col = x / 75;
        int row = y / 75;
        char columnChar = 'a' + col;
        int rowNum = 8 - row;

        cout << "Clicked: " << columnChar << rowNum << " (row " << row + 1 << ", col " << col + 1 << ")" << endl;
        if (board[row][col] != "")
            cout << " -->" << board[row][col] << endl;

        // First click: select piece
        if (startRow == -1 && board[row][col] != "")
        {
            string piece = board[row][col];
            bool isWhitePiece = (piece[0] == 'W');
            bool isBlackPiece = (piece[0] == 'B');

            if ((isWhiteTurn && isBlackPiece) || (!isWhiteTurn && isWhitePiece))
            {
                cout << "Not your Turn!" << endl;
                return;
            }

            startRow = row;
            startCol = col;
            selectedHighlight.setPosition(startCol * 75, startRow * 75);
            cout << "Selected: " << board[row][col] << endl;
        }
        else if (startRow != -1)  // CHANGED: Combined else and if
        {
            int endRow = row;
            int endCol = col;

            string selectedPiece = board[startRow][startCol];
            bool validMove = false;

            if (selectedPiece == "White Pawn" || selectedPiece == "Black Pawn")
            {
                validMove = pawnMove(selectedPiece, startRow, startCol, endRow, endCol);
            }
            else if (selectedPiece == "White Rook" || selectedPiece == "Black Rook")
            {
                validMove = rookMove(selectedPiece, startRow, startCol, endRow, endCol);
            }
            else if (selectedPiece == "White Bishop" || selectedPiece == "Black Bishop")
            {
                validMove = bishopMove(selectedPiece, startRow, startCol, endRow, endCol);
            }
            else if (selectedPiece == "White Queen" || selectedPiece == "Black Queen")
            {
                validMove = queenMove(selectedPiece, startRow, startCol, endRow, endCol);
            }
            else if (selectedPiece == "White Knight" || selectedPiece == "Black Knight")
            {
                validMove = knightMove(selectedPiece, startRow, startCol, endRow, endCol);
            }
            else if (selectedPiece == "White King" || selectedPiece == "Black King")
            {
                validMove = kingMove(selectedPiece, startRow, startCol, endRow, endCol);
            }

            if (validMove)
            {
                // Check if this move is legal
                if (!isMoveLegal(startRow, startCol, endRow, endCol))
                {
                    cout << "ILLEGAL MOVE! This would leave your king in check!" << endl;
                    startRow = -1;
                    startCol = -1;
                    selectedHighlight.setPosition(-100, -100);
                    return;
                }

                cout << selectedPiece << " moved!" << endl;

                // HANDLE CAPTURES
                if (board[endRow][endCol] != "")
                {
                    string capturedPiece = board[endRow][endCol];

                    //Score Detection
                    addScoreForCapture(capturedPiece);

                    if (capturedPiece == "Black Pawn")
                    {
                        int capturedPawnIdx = pawnIndexAt[endRow][endCol];
                        pawn1Captured[capturedPawnIdx] = true;
                        cout << "Black pawn " << capturedPawnIdx + 1 << " captured!" << endl;
                    }
                    else if (capturedPiece == "White Pawn")
                    {
                        int capturedPawnIdx = pawnIndexAt[endRow][endCol];
                        pawn2Captured[capturedPawnIdx] = true;
                        cout << "White pawn " << capturedPawnIdx + 1 << " captured!" << endl;
                    }
                    else if (capturedPiece == "Black Rook")
                    {
                        int capturedRookIdx = rookIndexAt[endRow][endCol];
                        rook1Captured[capturedRookIdx] = true;
                        cout << "Black rook " << capturedRookIdx + 1 << " captured!" << endl;
                    }
                    else if (capturedPiece == "White Rook")
                    {
                        int capturedRookIdx = rookIndexAt[endRow][endCol];
                        rook2Captured[capturedRookIdx] = true;
                        cout << "White rook " << capturedRookIdx + 1 << " captured!" << endl;
                    }
                    else if (capturedPiece == "Black Bishop")
                    {
                        int capturedBishopIdx = bishopIndexAt[endRow][endCol];
                        bishop1Captured[capturedBishopIdx] = true;
                        cout << "Black bishop " << capturedBishopIdx + 1 << " captured!" << endl;
                    }
                    else if (capturedPiece == "White Bishop")
                    {
                        int capturedBishopIdx = bishopIndexAt[endRow][endCol];
                        bishop2Captured[capturedBishopIdx] = true;
                        cout << "White bishop " << capturedBishopIdx + 1 << " captured!" << endl;
                    }
                    else if (capturedPiece == "Black Queen")
                    {
                        int queenIdx = queenIndexAt[endRow][endCol];
                        if (queenIdx == 0)
                        {
                            queen1Captured = true;
                            cout << "Black queen captured!" << endl;
                        }
                        else
                        {
                            int promotedIdx = -queenIdx - 1;
                            promotedQueen1Captured[promotedIdx] = true;
                            cout << "Black promoted queen captured!" << endl;
                        }
                    }
                    else if (capturedPiece == "White Queen")
                    {
                        int queenIdx = queenIndexAt[endRow][endCol];
                        if (queenIdx == 0)
                        {
                            queen2Captured = true;
                            cout << "White queen captured!" << endl;
                        }
                        else
                        {
                            int promotedIdx = -queenIdx - 1;
                            promotedQueen2Captured[promotedIdx] = true;
                            cout << "White promoted queen captured!" << endl;
                        }
                    }
                    else if (capturedPiece == "Black Knight")
                    {
                        int capturedKnightIdx = knightIndexAt[endRow][endCol];
                        knight1Captured[capturedKnightIdx] = true;
                        cout << "Black knight " << capturedKnightIdx + 1 << " captured!" << endl;
                    }
                    else if (capturedPiece == "White Knight")
                    {
                        int capturedKnightIdx = knightIndexAt[endRow][endCol];
                        knight2Captured[capturedKnightIdx] = true;
                        cout << "White knight " << capturedKnightIdx + 1 << " captured!" << endl;
                    }
                    else if (capturedPiece == "Black King")
                    {
                        king1Captured = true;
                        cout << "Black king captured! WHITE WINS!" << endl;
                    }
                    else if (capturedPiece == "White King")
                    {
                        king2Captured = true;
                        cout << "White king captured! BLACK WINS!" << endl;
                    }
                }

                // CHECK FOR PAWN PROMOTION
                bool isPromotion = PawnPromotion(startRow, startCol, endRow, endCol);

                if (isPromotion)
                {
                    isWhiteTurn = !isWhiteTurn;
                    cout << "Turn: " << (isWhiteTurn ? "White" : "Black") << endl;
                    startRow = -1;
                    startCol = -1;
                    selectedHighlight.setPosition(-100, -100);
                    return;
                }

                // NORMAL PIECE MOVEMENT
                if (selectedPiece == "White Pawn")
                {
                    int pawnIdx = pawnIndexAt[startRow][startCol];
                    pawn2[pawnIdx].setPosition(endCol * 75, endRow * 75);
                }
                else if (selectedPiece == "Black Pawn")
                {
                    int pawnIdx = pawnIndexAt[startRow][startCol];
                    pawn1[pawnIdx].setPosition(endCol * 75, endRow * 75);
                }
                else if (selectedPiece == "White Rook")
                {
                    int rookIdx = rookIndexAt[startRow][startCol];
                    rook2[rookIdx].setPosition(endCol * 75, endRow * 75);
                }
                else if (selectedPiece == "Black Rook")
                {
                    int rookIdx = rookIndexAt[startRow][startCol];
                    rook1[rookIdx].setPosition(endCol * 75, endRow * 75);
                }
                else if (selectedPiece == "White Bishop")
                {
                    int bishopIdx = bishopIndexAt[startRow][startCol];
                    bishop2[bishopIdx].setPosition(endCol * 75, endRow * 75);
                }
                else if (selectedPiece == "Black Bishop")
                {
                    int bishopIdx = bishopIndexAt[startRow][startCol];
                    bishop1[bishopIdx].setPosition(endCol * 75, endRow * 75);
                }
                else if (selectedPiece == "White Queen")
                {
                    int queenIdx = queenIndexAt[startRow][startCol];
                    if (queenIdx == 0)
                    {
                        queen2.setPosition(endCol * 75, endRow * 75);
                    }
                    else
                    {
                        int promotedIdx = -queenIdx - 1;
                        promotedQueen2[promotedIdx].setPosition(endCol * 75, endRow * 75);
                    }
                }
                else if (selectedPiece == "Black Queen")
                {
                    int queenIdx = queenIndexAt[startRow][startCol];
                    if (queenIdx == 0)
                    {
                        queen1.setPosition(endCol * 75, endRow * 75);
                    }
                    else
                    {
                        int promotedIdx = -queenIdx - 1;
                        promotedQueen1[promotedIdx].setPosition(endCol * 75, endRow * 75);
                    }
                }
                else if (selectedPiece == "White Knight")
                {
                    int knightIdx = knightIndexAt[startRow][startCol];
                    knight2[knightIdx].setPosition(endCol * 75, endRow * 75);
                }
                else if (selectedPiece == "Black Knight")
                {
                    int knightIdx = knightIndexAt[startRow][startCol];
                    knight1[knightIdx].setPosition(endCol * 75, endRow * 75);
                }
                else if (selectedPiece == "White King")
                {
                    king2.setPosition(endCol * 75, endRow * 75);
                }
                else if (selectedPiece == "Black King")
                {
                    king1.setPosition(endCol * 75, endRow * 75);
                }

                // Update board array
                board[endRow][endCol] = board[startRow][startCol];
                board[startRow][startCol] = "";

                // Update index tracking
                if (selectedPiece == "White Pawn" || selectedPiece == "Black Pawn")
                {
                    int pawnIdx = pawnIndexAt[startRow][startCol];
                    pawnIndexAt[endRow][endCol] = pawnIdx;
                    pawnIndexAt[startRow][startCol] = -1;
                }
                else if (selectedPiece == "White Rook" || selectedPiece == "Black Rook")
                {
                    int rookIdx = rookIndexAt[startRow][startCol];
                    rookIndexAt[endRow][endCol] = rookIdx;
                    rookIndexAt[startRow][startCol] = -1;
                }
                else if (selectedPiece == "White Bishop" || selectedPiece == "Black Bishop")
                {
                    int bishopIdx = bishopIndexAt[startRow][startCol];
                    bishopIndexAt[endRow][endCol] = bishopIdx;
                    bishopIndexAt[startRow][startCol] = -1;
                }
                else if (selectedPiece == "White Queen" || selectedPiece == "Black Queen")
                {
                    int queenIdx = queenIndexAt[startRow][startCol];
                    queenIndexAt[endRow][endCol] = queenIdx;
                    queenIndexAt[startRow][startCol] = -1;
                }
                else if (selectedPiece == "White Knight" || selectedPiece == "Black Knight")
                {
                    int knightIdx = knightIndexAt[startRow][startCol];
                    knightIndexAt[endRow][endCol] = knightIdx;
                    knightIndexAt[startRow][startCol] = -1;
                }

                // Switch turns
                isWhiteTurn = !isWhiteTurn;
                cout << "Turn: " << (isWhiteTurn ? "White" : "Black") << endl;

                // Check if the current player's king is in check
                bool inCheck = isKingInCheck(isWhiteTurn);
                if (inCheck)
                {
                    cout << "*** CHECK! " << (isWhiteTurn ? "White" : "Black") << " king is under attack! ***" << endl;

                    // Find king position and highlight it
                    string kingName = isWhiteTurn ? "White King" : "Black King";
                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            if (board[i][j] == kingName)
                            {
                                checkHighlight.setPosition(j * 75, i * 75);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    checkHighlight.setPosition(-100, -100);
                }

                // Check for checkmate or stalemate
                if (!hasAnyLegalMoves(isWhiteTurn))
                {
                    if (inCheck)
                    {
                        cout << "\n========================================" << endl;
                        cout << "      CHECKMATE!" << endl;
                        cout << "      " << (!isWhiteTurn ? "WHITE" : "BLACK") << " WINS!" << endl;
                        cout << "========================================\n" << endl;
                        gameOver = true;
                    }
                    else
                    {
                        cout << "\n========================================" << endl;
                        cout << "      STALEMATE!" << endl;
                        cout << "      Game is a DRAW!" << endl;
                        cout << "========================================\n" << endl;
                        gameOver = true;
                    }
                }
            }
            else
            {
                cout << "Invalid move!" << endl;
            }

            startRow = -1;
            startCol = -1;
            selectedHighlight.setPosition(-100, -100);
        }
    }
}
    
void saveGame(const string& Chess_Scores)
{
    ofstream file("Chess_Scores.txt");

    if (!file.is_open())
    {
        cout << "Error: Could not save game!" << endl;
        return;
    }

    // Save whose turn it is
    file << (isWhiteTurn ? "White" : "Black") << endl;

    // Save scores
    file << "White Score: " << whiteScore << endl;
    file << "Black Score: " << blackScore << endl;

    file.close();
    cout << "Scores saved successfully!" << endl;
}

bool loadGame(const string& Chess_Scores)
{
    ifstream file("Chess_Scores.txt");

    if (!file.is_open())
    {
        cout << "Error: Could not load game! File not found." << endl;
        return false;
    }

    // Load whose turn it is
    string turn;
    file >> turn;
    isWhiteTurn = (turn == "White");

    // Load scores
    string label; // To skip the words "White Score:" and "Black Score:"
    file >> label >> label >> whiteScore;  // Reads "White Score: 5"
    file >> label >> label >> blackScore;  // Reads "Black Score: 3"

    file.close();

    cout << "Scores loaded successfully!" << endl;
    cout << "Turn: " << (isWhiteTurn ? "White" : "Black") << endl;
    cout << "White Score: " << whiteScore << endl;
    cout << "Black Score: " << blackScore << endl;

    return true;
}

void handleKeyboard(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        // Press 'S' to save game
        if (event.key.code == sf::Keyboard::S)
        {
            saveGame("chess_save.txt");
        }

        // Press 'L' to load game
        if (event.key.code == sf::Keyboard::L)
        {
            loadGame("chess_save.txt");
        }

        // Press 'N' to start new game
        if (event.key.code == sf::Keyboard::N)
        {
            // Reset everything
            gameOver = false;
            isWhiteTurn = true;
            whiteScore = 0;
            blackScore = 0;

            // Reset captured status
            for (int i = 0; i < 8; i++)
            {
                pawn1Captured[i] = false;
                pawn2Captured[i] = false;
            }

            for (int i = 0; i < 2; i++)
            {
                rook1Captured[i] = false;
                rook2Captured[i] = false;
                bishop1Captured[i] = false;
                bishop2Captured[i] = false;
                knight1Captured[i] = false;
                knight2Captured[i] = false;
            }

            queen1Captured = false;
            queen2Captured = false;
            king1Captured = false;
            king2Captured = false;

            promotedQueen1Count = 0;
            promotedQueen2Count = 0;

            for (int i = 0; i < 8; i++)
            {
                promotedQueen1Captured[i] = false;
                promotedQueen2Captured[i] = false;
            }

            // Reinitialize board and pieces
            initialize_board_names();
            chess_pieces();

            cout << "New game started!" << endl;
        }
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "Chess", sf::Style::Titlebar | sf::Style::Close);

    // Initialize chess pieces
    chess_pieces();

    // Initialize pieces names
    initialize_board_names();

    // Colors for the chessboard squares
    sf::Color lightColor(238, 238, 210);
    sf::Color darkColor(118, 150, 86);

    // Load font for labels
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        cout << "Error loading font!" << endl;
    }

    // Initialize selected piece highlight
    selectedHighlight.setSize(sf::Vector2f(75, 75));
    selectedHighlight.setFillColor(sf::Color::Transparent);
    selectedHighlight.setOutlineColor(sf::Color::Yellow);
    selectedHighlight.setOutlineThickness(4);

    // Initialize check highlight for king
    checkHighlight.setSize(sf::Vector2f(75, 75));
    checkHighlight.setFillColor(sf::Color::Transparent);
    checkHighlight.setOutlineColor(sf::Color::Red);
    checkHighlight.setOutlineThickness(3);
    checkHighlight.setPosition(-100, -100);  

    while (window.isOpen())
    {
        // event object, ye line ek "listener" prepare karti hai taake program user ke actions pe react kar sake.
        sf::Event event;

        // checks every event performed on the window, if there is any then it stores it in the event object
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            mouse_click(event);
            handleKeyboard(event);
        }

        window.clear();

        // Draw chessboard squares
        sf::RectangleShape square;
        square.setSize(sf::Vector2f(75, 75)); // setSize() needs a 2D vector (x, y)

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if ((i + j) % 2 == 0)
                    square.setFillColor(darkColor);
                else
                    square.setFillColor(lightColor);

                square.setPosition(j * 75, i * 75);
                window.draw(square);
            }
        }

        // Draw column labels (a-h) at the bottom of each column
        for (int i = 0; i < 8; i++)
        {
            sf::Text label;
            label.setFont(font);
            label.setString(string(1, 'a' + i));
            label.setCharacterSize(15);
            label.setFillColor(sf::Color::Black);
            label.setPosition(i * 75 + 66, 7 * 75 + 55);
            window.draw(label);
        }

        // Draw row labels (8-1) on the left side of each row
        for (int i = 0; i < 8; i++)
        {
            sf::Text label;
            label.setFont(font);
            label.setString(to_string(8 - i));
            label.setCharacterSize(14);
            label.setFillColor(sf::Color::Black);
            label.setPosition(2, i * 75 + 2);
            window.draw(label);
        }

        sf::RectangleShape textBackground;
        textBackground.setSize(sf::Vector2f(175, 15));  // Width and height of box
        textBackground.setFillColor(sf::Color(0, 0, 0, 150));  // Black with transparency
        textBackground.setPosition(1, 1);
        window.draw(textBackground);

        // Show controls to user
        sf::Text instructions;
        instructions.setFont(font);
        instructions.setString("S-Save | L-Load | N-New Game");
        instructions.setCharacterSize(12);
        instructions.setFillColor(sf::Color::White);
        instructions.setPosition(3, 0);
        window.draw(instructions);

        // Draw check highlight (RED BOX for king in check)
        window.draw(checkHighlight);

        // Draw pieces on top of the squares
        chess_pieces_draw(window);

        // Draw selected piece highlight
        if (startRow != -1)
        {
            window.draw(selectedHighlight);
        }

        window.display();

    }

    return 0;
}

