#include <FastLED.h>

#define NUM_LEDS 64
#define PIN A4
// Размеры доски и фигур
const CRGB COLOR_WHITE = CRGB::White;
const CRGB COLOR_BLACK = CRGB::Black;
const int BOARD_SIZE = 8;
const int PIECE_NONE = 0;
const int PIECE_WHITE_PAWN = 1;
const int PIECE_WHITE_ROOK = 2;
const int PIECE_WHITE_KNIGHT = 3;
const int PIECE_WHITE_BISHOP = 4;
const int PIECE_WHITE_QUEEN = 5;
const int PIECE_WHITE_KING = 6;
const int PIECE_BLACK_PAWN = -1;
const int PIECE_BLACK_ROOK = -2;
const int PIECE_BLACK_KNIGHT = -3;
const int PIECE_BLACK_BISHOP = -4;
const int PIECE_BLACK_QUEEN = -5;
const int PIECE_BLACK_KING = -6;
CRGB leds[NUM_LEDS];
const int ROWS = 8;
const int COLS = 8;
const int rowPins[ROWS] = { 2, 3, 4, 5, 6, 7, 8, 9 }; // установитье какие пины будут у вертикального 
const int colPins[COLS] = { 10, 11, 12, 13, A0, A1, A2, A3 };//соответственно горизонтального
bool isPieceUp = false;
bool cellState[BOARD_SIZE][BOARD_SIZE];
int board[BOARD_SIZE][BOARD_SIZE];
int currentPlayer = 1;
int selectedRow = -1;
int selectedCol = -1;
int playerColor = 0;
void displayBoard() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      int ledIndex;
      if (row % 2 == 0) {
        ledIndex = row * 8 + col;
      } else {
        ledIndex = row * 8 + 7 - col;
      }
      if ((row + col) % 2 == 0) {
        leds[ledIndex] = CRGB::White;
      } else {
        leds[ledIndex] = CRGB::Blue;
      }
    }
  }
  FastLED.show();
}
void test() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (row % 2 == 1) {
        leds[row * 8 - col] = CRGB::Coral;
        FastLED.show();
        delay(1000);
      } else {
        leds[row * 8 + col] = CRGB::Red;
        delay(1000);
      }
    }
  }
}
void resetBoard() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = PIECE_NONE;
    }
  }
  board[0][0] = PIECE_WHITE_ROOK;
  board[0][1] = PIECE_WHITE_KNIGHT;
  board[0][2] = PIECE_WHITE_BISHOP;
  board[0][3] = PIECE_WHITE_QUEEN;
  board[0][4] = PIECE_WHITE_KING;
  board[0][5] = PIECE_WHITE_BISHOP;
  board[0][6] = PIECE_WHITE_KNIGHT;
  board[0][7] = PIECE_WHITE_ROOK;
  for (int i = 1; i < 8; i++) {
    board[1][i] = PIECE_WHITE_PAWN;
    cellState[0][i] = true;
    cellState[1][i] = true;
  }

  // Размещаем черные фигуры на доске
  board[7][0] = PIECE_BLACK_ROOK;
  board[7][1] = PIECE_BLACK_KNIGHT;
  board[7][2] = PIECE_BLACK_BISHOP;
  board[7][3] = PIECE_BLACK_QUEEN;
  board[7][4] = PIECE_BLACK_KING;
  board[7][5] = PIECE_BLACK_BISHOP;
  board[7][6] = PIECE_BLACK_KNIGHT;
  board[7][7] = PIECE_BLACK_ROOK;
  for (int i = 0; i < 8; i++) {
    board[6][i] = PIECE_BLACK_PAWN;
    cellState[6][i] = true;
    cellState[7][i] = true;
  }
  // Устанавливаем начальное состояние переменных
  currentPlayer = 1;
  selectedRow = -1;
  selectedCol = -1;
  displayBoard();
}
void highlightMoves(bool moves[8][8]) {
  // if(true){
  //   leds[0] = CRGB::Crimson;
  //   FastLED.show();
  //   delay(500000000);
  // }
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      int piece = board[i][j];
      if (piece == 0 && moves[i][j] == true) {
        if (i % 2 == 1) {
          leds[i * COLS + (8 - j - 1)] = CRGB::Green;
        } else {
          leds[i * COLS + j] = CRGB::Red;
        }
      }
    }
  }
  leds[2 * 8 + 4] = CRGB::Yellow;
  FastLED.show();
}

bool canMove(int fromRow, int fromCol, int toRow, int toCol) {

  int piece = board[fromRow][fromCol];
  if (piece == PIECE_NONE) {
    return false;
  }


  if (piece > 0 && currentPlayer != 1) {
    return false;
  }
  if (piece < 0 && currentPlayer != -1) {
    return false;
  }
  if (toRow < 0 || toRow >= BOARD_SIZE || toCol < 0 || toCol >= BOARD_SIZE) {
    return false;
  }


  int destPiece = board[toRow][toCol];
  if (destPiece != PIECE_NONE && destPiece > 0 && piece > 0) {
    return false;
  }
  if (destPiece != PIECE_NONE && destPiece < 0 && piece < 0) {
    return false;
  }

  int deltaRow = toRow - fromRow;
  int deltaCol = toCol - fromCol;
  switch (piece) {
    case PIECE_WHITE_PAWN:
      if (abs(deltaCol) > 1) {
        return false;
      }
      if (abs(deltaCol) == 1 && deltaRow == 1 && destPiece < 0) {
        return true;
      }
      if (deltaRow == 1 && deltaCol == 0) {
        return true;
      }
      if (fromRow == 1 && toRow == 3 && deltaCol == 0) {
        return true;
      }
      return false;
    case PIECE_BLACK_PAWN:
      if (deltaCol > 1) {
        return false;
      }
      if (abs(deltaCol) == 1 && deltaRow == -1 && destPiece > 0) {
        return true;
      }
      if (piece < 0 && deltaRow == 1) {
        return true;
      }
      if (fromRow == 7 && toRow == 5 && destPiece == 0) {
        return true;
      }
      return false;
    case PIECE_WHITE_ROOK:
      if (deltaRow != 0 && deltaCol != 0) {
        return false;
      }
      if (deltaRow == 0) {
        int step = deltaCol > 0 ? 1 : -1;
        for (int c = toCol; c != fromCol; c -= step) {
          if (board[fromRow][c] != PIECE_NONE && c != toCol) {
            return false;
          }
        }
        for (int col = fromCol + step; col != toCol; col += step) {
          if (board[fromRow][col] != PIECE_NONE) {
            if (board[fromRow][col] < 0) {
              return true;
            }
            return false;
          }
        }
        return true;
      }
      if (deltaCol == 0) {
        int step = deltaRow > 0 ? 1 : -1;
        for (int r = toRow; r != fromRow; r -= step) {
          if (board[r][fromCol] != PIECE_NONE){
            if (des) {
            return false;
          }
        }
        for (int row = fromRow + step; row != toRow; row += step) {
          if (board[row][fromCol] != PIECE_NONE) {
            if (board[row][fromCol] < 0) {
              return true;
            }
            return false;
          }
        }
        return true;
      }
      return false;
    case PIECE_BLACK_ROOK:
      if (deltaRow != 0 && deltaCol != 0) {
        return false;
      }
      if (deltaRow == 0) {
        int step = deltaCol > 0 ? 1 : -1;
        for (int c = toCol; c != fromCol; c -= step) {
          if (board[fromRow][c] != PIECE_NONE && c != toCol) {
            return false;
          }
        }
        for (int col = fromCol + step; col != toCol; col += step) {
          if (board[fromRow][col] != PIECE_NONE) {
            if (board[fromRow][col] < 0) {
              return true;
            }
            return false;
          }
        }
        return true;
      }
      if (deltaCol == 0) {
        int step = deltaRow > 0 ? 1 : -1;
        for (int r = toRow; r != fromRow; r -= step) {
          if (board[r][fromCol] != PIECE_NONE && r != toRow) {
            return false;
          }
        }
        for (int row = fromRow + step; row != toRow; row += step) {
          if (board[row][fromCol] != PIECE_NONE) {
            if (board[row][fromCol] < 0) {
              return true;
            }
            return false;
          }
        }
        return true;
      }
      return false;
    case PIECE_BLACK_KNIGHT:
      if (abs(deltaRow) == 1 && abs(deltaCol) == 2) {
        return true;
      }
      if (abs(deltaRow) == 2 && abs(deltaCol) == 1) {
        return true;
      }
      return false;
    case PIECE_WHITE_KNIGHT:
      if (abs(deltaRow) == 1 && abs(deltaCol) == 2) {
        return true;
      }
      if (abs(deltaRow) == 2 && abs(deltaCol) == 1) {
        return true;
      }
      return false;
    case PIECE_WHITE_BISHOP:
      if (abs(deltaRow) != abs(deltaCol)) {
        return false;
      }
      int rowStep = deltaRow > 0 ? 1 : -1;
      int colStep = deltaCol > 0 ? 1 : -1;
      for (int r = toRow, c = toCol;
           r > fromRow && c > fromCol;
           r -= rowStep, c -= colStep) {
        if (board[r][c] != PIECE_NONE && board[r][c] > 0) {
          return false;
        }
      }
      for (int row = fromRow + rowStep, col = fromCol + colStep;
           row != toRow && col != toCol;
           row += rowStep, col += colStep) {
        if (board[row][col] != PIECE_NONE) {
          if (board[row][col] < 0) {
            return true;
          }
          return false;
        }
      }
      return true;
    case PIECE_BLACK_BISHOP:
      if (abs(deltaRow) != abs(deltaCol)) {
        return false;
      }
      rowStep = deltaRow > 0 ? 1 : -1;
      colStep = deltaCol > 0 ? 1 : -1;
      for (int r = toRow, c = toCol;
           r > fromRow && c > fromCol;
           r -= rowStep, c -= colStep) {
        if (board[r][c] != PIECE_NONE && board[r][c] < 0) {
          return false;
        }
      }
      for (int row = fromRow + rowStep, col = fromCol + colStep;
           row != toRow && col != toCol;
           row += rowStep, col += colStep) {
        if (board[row][col] != PIECE_NONE) {
          if (board[row][col] > 0) {
            return true;
          }
          return false;
        }
      }
      return true;
    case PIECE_WHITE_QUEEN:
      if (deltaRow == 0 || deltaCol == 0) {
        // Rook-like move
        if (deltaRow == 0) {
          int step = deltaCol > 0 ? 1 : -1;
          for (int c = toCol; c != fromCol; c -= step) {
            if (board[fromRow][c] != PIECE_NONE && c != toCol) {
              return false;
            }
          }
          for (int col = fromCol + step; col != toCol; col += step) {
            if (board[fromRow][col] != PIECE_NONE) {
              if (board[fromRow][col] < 0) {
                return true;
              }
              return false;
            }
          }
          return true;
        } else {
          int step = deltaRow > 0 ? 1 : -1;
          for (int r = toRow; r != fromRow; r -= step) {
            if (board[r][fromCol] != PIECE_NONE && r != toRow) {
              return false;
            }
          }
          for (int row = fromRow + step; row != toRow; row += step) {
            if (board[row][fromCol] != PIECE_NONE) {
              if (board[row][fromCol] < 0) {
                return true;
              }
              return false;
            }
          }
          return true;
        }
      } else if (abs(deltaRow) == abs(deltaCol)) {
        // Bishop-like move
        int rowStep = deltaRow > 0 ? 1 : -1;
        int colStep = deltaCol > 0 ? 1 : -1;
        for (int r = toRow, c = toCol;
             r > fromRow && c > fromCol;
             r -= rowStep, c -= colStep) {
          if (board[r][c] != PIECE_NONE && board[r][c] > 0) {
            return false;
          }
        }
        for (int row = fromRow + rowStep, col = fromCol + colStep;
             row != toRow && col != toCol;
             row += rowStep, col += colStep) {
          if (board[row][col] != PIECE_NONE) {
            if (board[row][col] < 0) {
              return true;
            }
            return false;
          }
        }
        return true;
      }
      return false;
    case PIECE_BLACK_QUEEN:
      if (deltaRow == 0 || deltaCol == 0) {
        // Rook-like move
        if (deltaRow == 0) {
          int step = deltaCol > 0 ? 1 : -1;
          for (int c = toCol; c != fromCol; c -= step) {
            if (board[fromRow][c] != PIECE_NONE && c != toCol) {
              return false;
            }
          }
          for (int col = fromCol + step; col != toCol; col += step) {
            if (board[fromRow][col] != PIECE_NONE) {
              if (board[fromRow][col] > 0) {
                return true;
              }
              return false;
            }
          }
          return true;
        } else {
          int step = deltaRow > 0 ? 1 : -1;
          for (int r = toRow; r != fromRow; r -= step) {
            if (board[r][fromCol] != PIECE_NONE && r != toRow) {
              return false;
            }
          }
          for (int row = fromRow + step; row != toRow; row += step) {
            if (board[row][fromCol] != PIECE_NONE) {
              if (board[row][fromCol] > 0) {
                return true;
              }
              return false;
            }
          }
          return true;
        }
      } else if (abs(deltaRow) == abs(deltaCol)) {
        // Bishop-like move
        int rowStep = deltaRow > 0 ? 1 : -1;
        int colStep = deltaCol > 0 ? 1 : -1;
        for (int r = toRow, c = toCol;
             r > fromRow && c > fromCol;
             r -= rowStep, c -= colStep) {
          if (board[r][c] != PIECE_NONE && board[r][c] < 0) {
            return false;
          }
        }
        for (int row = fromRow + rowStep, col = fromCol + colStep;
             row != toRow && col != toCol;
             row += rowStep, col += colStep) {
          if (board[row][col] != PIECE_NONE) {
            if (board[row][col] > 0) {
              return true;
            }
            return false;
          }
        }
        return true;
      }
      return false;
    case PIECE_WHITE_KING:
      if (abs(deltaRow) <= 1 && abs(deltaCol) <= 1) {
        return true;
      }
      return false;
    default:
      return false;
    case PIECE_BLACK_KING:
      if (abs(deltaRow) <= 1 && abs(deltaCol) <= 1) {
        return true;
      }
      return false;
    default:
      return false;
  }
}

void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
  int piece = board[fromRow][fromCol];
  board[fromRow][fromCol] = PIECE_NONE;
  board[toRow][toCol] = piece;
}


void setup() {
  FastLED.addLeds<WS2812B, PIN>(leds, NUM_LEDS);
  // Инициализация пинов
  for (int i = 0; i < 8; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
  }
  for (int i = 0; i < 8; i++) {
    pinMode(colPins[i], OUTPUT);
    digitalWrite(colPins[i], HIGH);
  }
  resetBoard();
  displayBoard();
}
int scanBoard(int& col, int& row) {
  int curr_pos[8][8];
  int new_pos[8][8];
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      curr_pos[r][c] = digitalRead(rowPins[r]) * digitalRead(colPins[c]);
    }
  }
  while (true) {
    for (int r = 0; r < 4; r++) {
      for (int c = 0; c < 4; c++) {
        new_pos[r][c] = digitalRead(rowPins[r]) * digitalRead(colPins[c]);
      }
    }
    for (int r = 0; r < 4; r++) {
      for (int c = 0; c < 4; c++) {
        if (curr_pos[r][c] != new_pos[r][c]) {
          row = r;
          col = c;
          return;
        }
      }
    }
    delay(50);
  }
}
void loop() {
  displayBoard();
  int fromCol = 0;
  int fromRow = 0;
  // while (fromCol == -1 || fromRow == -1) {
  //   scanBoard(fromCol, fromRow);
  // }
  int piece = board[fromRow][fromCol];
  bool moves[BOARD_SIZE][BOARD_SIZE];
  memset(moves, false, sizeof(moves));
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      moves[row][col] = canMove(fromRow, fromCol, row, col);
      if (canMove(fromRow, fromCol, row, col)) {
        moves[row][col] = true;
      }
    }
  }
  highlightMoves(moves);


  leds[4] = CRGB::Yellow;
  FastLED.show();
  int toRow = -1, toCol = -1;
  while (toRow == -1 || toCol == -1) {
    scanBoard(toRow, toCol);
    delay(10);
  }
  if (canMove(fromRow, fromCol, toRow, toCol)) {
    movePiece(fromRow, fromCol, toRow, toCol);
  }
  currentPlayer = 0 - currentPlayer;
  delay(500);
  displayBoard();
}