#include <iostream>
#include <vector>
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>
#include <thread>
#include "../include/inputparser.h"

const char ALIVE = 'O';
const char DEAD = ' ';
const int DEFAULT_WIDTH = 5;
const int DEFAULT_HEIGHT = 5;

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

std::vector<std::vector<char>> load_board_state(const std::string filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<char>> board;
    while(std::getline(file, line)){
        std::vector<char> row;
        for(int i = 0; i < line.length(); i++){
            row.push_back((line[i] == '1') ? ALIVE : DEAD);
        }
        board.push_back(row);
    }
    file.close();
    return board;
}

void printHelpMenu() {
    std::cout << "TODO: write help menu." << std::endl;
}

int main(int argc, char *argv[]) {
    InputParser input(argc, argv);
    
    if(input.cmdOptionExists("-h")){
        printHelpMenu();
        return 0;
    }
    
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    std::vector<std::vector<char>> board;

    if(input.cmdOptionExists("-file")){
        //load from file
        const std::string &fileName = input.getCmdOption("-file");
        board = load_board_state(fileName);
    }
    else{
        if(input.cmdOptionExists("-width")){
            const std::string &str_width = input.getCmdOption("-width");
            width = std::stoi(str_width);
        }
        if(input.cmdOptionExists("-height")){
            const std::string &str_height = input.getCmdOption("-height");
            height = std::stoi(str_height);
        }

        board = random_state(width, height);
    }



    while(true){
        render(board);

        board = next_board_state(board);

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //1 second
    }
    
}
