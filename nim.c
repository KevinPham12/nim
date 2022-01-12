#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nim.h"

int *new_board(int board_size) {

   int *arrayBoard = malloc(board_size * sizeof(int));

   if (arrayBoard == NULL) {

      fprintf(stderr, "Malloc failed... Exiting program.");
      exit(-1);

   }

   return arrayBoard;

}

int *copy_board(int board_size, int *board) {

   int *newBoard = new_board(board_size);

   for (int i = 0; i < board_size; ++i) {

      newBoard[i] = board[i];

   }

   return newBoard;

}

int *board_from_argv(int board_size, char **argv) {

   int *newBoard = new_board(board_size);

   for (int i = 0; i < board_size; ++i) {

      newBoard[i] = atoi(argv[i]);

   }

   return newBoard;

}

struct node *mk_nim_hash(int max_hash, int board_size, int *start_board) {

   struct node * nodesStruct = malloc(max_hash * sizeof(struct node));

   if (nodesStruct == NULL) {

      fprintf(stderr, "Malloc failed... Exiting program");
      exit(-1);

   }

   for (int i = 0; i < max_hash; ++i) {

      nodesStruct[i].moves = -1;
      nodesStruct[i].move = NULL;
      nodesStruct[i].nimsum = -1;

      int *boardVal = hash2board(board_size, start_board, i);

      nodesStruct[i].board = boardVal;

   }

   return nodesStruct;

}

void free_board(int *board) {

   free(board);

}

void free_nim_hash(int max_hash, struct node *nim_hash) {

   for (int i = 0; i < max_hash; ++i) {

      free(nim_hash[i].move);
      free(nim_hash[i].board);

   }

   free(nim_hash);

}

int game_over(int board_size, int *board) {

   int numMatches = 0;

   for (int i = 0; i < board_size; ++i) {

      numMatches = numMatches + board[i];

   }

   if (numMatches == 1) {

      return 1;

   } else {

      return 0;

   }

}

void join_graph(struct node *nim_hash, int hash, int board_size, int *start_board) {

   int possibleMoves = 0;
   int counter = 0;

   if (nim_hash[hash].moves == -1) {

      for (int i = 0; i < board_size; ++i) {

         possibleMoves = possibleMoves + nim_hash[hash].board[i];

      }

      struct move *movesArray = malloc(possibleMoves * sizeof(struct move));

      nim_hash[hash].moves = possibleMoves;
      nim_hash[hash].move = movesArray;

      for (int row = 0; row < board_size; ++row) {

         int numMatches = 1;

         while (numMatches <= nim_hash[hash].board[row]) {

            movesArray[counter].row = row;
            movesArray[counter].matches = numMatches;

            int *currentBoard = copy_board(board_size, nim_hash[hash].board);

            currentBoard[row] = currentBoard[row] - numMatches;

            int hashVal = board2hash(board_size, start_board, currentBoard);

            free(currentBoard);

            movesArray[counter].hash = hashVal;

            join_graph(nim_hash, movesArray[counter].hash, board_size, start_board);

            ++numMatches;
            ++counter;

         }

      }


   } else if (nim_hash[hash].moves != -1) {

      return;

   }

}

