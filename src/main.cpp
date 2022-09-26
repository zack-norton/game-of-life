#include <iostream>
#include <vector>
#include <iterator>
#include <cstdlib>
#include <ctime>
#include "../include/inputparser.h"

const char ALIVE = 'O';
const char DEAD = ' ';

std::vector<std::vector<char>>dead_state(int width, int height) {
    return std::vector<std::vector<char>>(height, std::vector<char>(width, DEAD));
}

std::vector<std::vector<char>> random_state(int width, int height) {
    std::vector<std::vector<char>> board = dead_state(width, height);


    srand(time(0));

    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[i].size(); j++){
            board[i][j] = ((rand() / (double)RAND_MAX) >= 0.8) ? ALIVE : DEAD;
        }
    }

    return board;
}

void render(std::vector<std::vector<char>> board) {
    std::string str = "";
    for(std::vector<char> v : board)
    {
        for(char x : v)
        {
            str += x;
        }
        str += '\n';
    }
    std::cout << str;
}

std::vector<std::vector<char>> next_board_state(std::vector<std::vector<char>> board) {
    std::vector<std::vector<char>> next_state = dead_state(board[0].size(), board.size());

    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[i].size(); j++){
            int liveNeighbors = 0;

            //left-up
            if(i > 0 && j > 0){
                if(board[i-1][j-1] == ALIVE){
                    liveNeighbors += 1;
                }
            }
            //up
            if(i > 0){
                if(board[i-1][j] == ALIVE){
                    liveNeighbors += 1;
                }
            }
            //right-up
            if(i > 0 && j < board[i].size() - 1){
                if(board[i-1][j+1] == ALIVE){
                    liveNeighbors += 1;
                }
            }
            //right
            if(j < board[i].size() - 1){
                if(board[i][j+1] == ALIVE){
                    liveNeighbors += 1;
                }
            }
            //right-down
            if(i < board.size() - 1 && j < board[i].size() - 1){
                if(board[i+1][j+1] == ALIVE){
                    liveNeighbors += 1;
                }
            }
            //down
            if(i < board.size() - 1){
                if(board[i+1][j] == ALIVE){
                    liveNeighbors += 1;
                }
            }
            //left-down
            if(i < board.size() - 1 && j > 0){
                if(board[i+1][j-1] == ALIVE){
                    liveNeighbors += 1;
                }
            }
            //left
            if(j > 0){
                if(board[i][j-1] == ALIVE){
                    liveNeighbors += 1;
                }
            }
            
            char status = board[i][j];
            if(status == ALIVE && (liveNeighbors == 0 || liveNeighbors == 1)){
                status = DEAD;
            }
            else if(status == ALIVE && (liveNeighbors == 2 || liveNeighbors == 3)){
                status = ALIVE;
            }
            else if(status == ALIVE && liveNeighbors > 3){
                status = DEAD;
            }
            else if(status == DEAD && liveNeighbors == 3){
                status = ALIVE;
            }
            else{
                status = DEAD;
            }

            next_state[i][j] = status;
        }
    }
    
    return next_state;
}

int main(int argc, char *argv[]) {
    InputParser input(argc, argv);
    
    if(input.cmdOptionExists("-h")){
        std::cout << "HELP!" << std::endl;
        return 0;
    }

    std::vector<std::vector<char>> board = random_state(50,50);

    while(true){
        render(board);

        board = next_board_state(board);

    }
    
}
