#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct player_state{


    bool turn = false;
    int score  = 5.0;
    int arrow_dir = 15.0;
    int arrow_len = 15.0;
    std::vector<CurlingStone> stones;

    void print()
    {
        std::cout << "Name: " << this->score << "\tTurn: " << this->turn << "\tScore: " << this->score <<  "\tArrowDir: " <<  this->arrow_dir << "\tArrowLen: " << this->arrow_len << std::endl;
    }
};

struct game_state{
    bool connected = false;
    int client_id; 
    char buffer[256];
    player_state state;
};
