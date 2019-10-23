#include<vector>
#include"MyOthelloAI.hpp"
#include"OthelloCell.hpp"
#include"OthelloGameState.hpp"
#include"OthelloBoard.hpp"
#include<ics46/factory/DynamicFactory.hpp>
#include<iostream>

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, yunlum::MyOthelloAI, "Wawwaw Miao AI (Required)");

// The PossibleMove function checks the possible move of the AI by using the method isValidMove() to add the possible move into a vector of the pair of coordinates named psmoves.  
std::vector<std::pair<int,int>> yunlum::MyOthelloAI::PossibleMove(const OthelloGameState& state, const OthelloBoard& board)
{
    std::vector<std::pair<int,int>> psmoves;

    for (int w = 0; w < board.width(); w++)
    {
        for (int h = 0; h < board.height(); h++)
        {
            if (state.isValidMove(w,h) && !state.isGameOver())
            {
                std::pair<int,int> move = std::make_pair(w,h);
                psmoves.push_back(move);
            }
        }
    }
    return psmoves;
}

// The Eval function makes the evaluatin of the move based of the color of the AI and the evaluation method MidEvaluation and returns the result of the evaluation.
int yunlum::MyOthelloAI::Eval(OthelloGameState& state, OthelloCell& color)
{
    int evaluation;
    const OthelloBoard& board = state.board();

    if (color == OthelloCell::black)
    {
        evaluation = state.blackScore() - state.whiteScore();
        evaluation = MidEvaluation( evaluation, board, color);
    }
    else
    {
        evaluation = state.whiteScore() - state.blackScore();
        evaluation = MidEvaluation( evaluation, board, color);

    }

    
    return evaluation;
}

// The search function does the heuristic search of the possible moves in the vector psmoves. By checking whether the game is over and whether the depth == 0, it will call the evaluation function above to return the evaluation. If not, it will first check the turn of the game if it is the AI's turn and make a move from the possible move and copy the state and pass the state and depth-1 in to the search function by search recursively. If it is the AI's turn, it will try to find the max evaluation for the best move. If it is the opposite turn, it will find the min evaluation since it pretends that the opposite AI will also find the max evaluation for itself, which is the min evaluation of my AI. 
int yunlum::MyOthelloAI::search(OthelloGameState& state, int depth, OthelloCell& color)
{
    std::vector<std::pair<int,int>> psmoves = PossibleMove(state,state.board());
    OthelloCell turn;
    if (state.isBlackTurn())
    {
        turn = OthelloCell::black;
    }
    else
    {
        turn = OthelloCell::white;
    }


    if (depth == 0 || state.isGameOver())
    {
        
        return Eval(state,color);
    }

    else
    {
        if (turn == color)
        {
           

            int max;
            
            for (int n = 0; n < psmoves.size(); n++)
            {
                std::unique_ptr<OthelloGameState> state_copy = state.clone();
                
                state_copy->makeMove(psmoves[n].first, psmoves[n].second);
              
                int score = search(*state_copy,depth-1,color);
                if (n == 0)
                {
                    max = score;
                }
                else if (score >= max)
                {
                    max = score;
                }
            }
            
            return max;
        }

        else
        {

            int min;
            for (int m = 0; m < psmoves.size(); m++)
            {
                std::unique_ptr<OthelloGameState> state_copy = state.clone();
                state_copy->makeMove(psmoves[m].first, psmoves[m].second);
                int score = search(*state_copy,depth-1,color);
                
                if (m == 0)
                {
                    min = score;
                }
                else if (score <= min)
                {
                    min = score;
                }
            }
             
            return min;
        }

   }
}

// The chooseMove function returns the best move after search and evaluation. It fisrt checks the color of the AI and builds a psmoves vector and for every possible move in the vector, it makes a copy of the state and pass it in to the search function. After getting the result from the search function, it will take the final evaluation and return the final result. After comparison, it will find the best reult and pass its related index in order to find the pair of coordinates in the vector. Finally returns the best move. 
std::pair<int,int> yunlum::MyOthelloAI::chooseMove(const OthelloGameState& state)
{
    const OthelloBoard& board = state.board();
    std::vector<std::pair<int,int>> psmoves = PossibleMove(state,board);
    OthelloCell color;
    int best;
    int index = 0;

    if (state.isBlackTurn())
    {
        color = OthelloCell::black;
       
    }
    else
    {
        color = OthelloCell::white;
        
    }
    
    
    for (int i = 0; i < psmoves.size(); i++)
    {   
        std::unique_ptr<OthelloGameState> state_copy = state.clone();
        int result = search(*state_copy,3,color);
        result = FinalEvaluation(psmoves[i].first,psmoves[i].second,result,board,color);        
        if (i == 0)
        {
            best = result;
        }
                                    
        else if (result > best)
        {
            index = i;
            best = result;
        }
    }                                            
    
    
    return psmoves[index];
}

// The FinalEvaluation function checks the possible moves in the vector by checking the position of the coorinates in order to occupy the important position(eg. the four edges and corners) on the state quickly by changing the value of the result. 
int yunlum::MyOthelloAI::FinalEvaluation(int x, int y, int result, const OthelloBoard& board, OthelloCell& color)
{
    if (x == 0 && y == 0)
    {
        result += 64;
    }
    else if (x == 0 && y == board.height() - 1)
    {
        result += 64;
    }
    else if (x == board.width() - 1 && y == 0)
    {
        result += 64;
    }
    else if (x == board.width() - 1 && y == board.height() - 1)
    {
        result += 64;
    }
    
    else if (board.cellAt(0, board.height() - 1) != color &&((x == 0 && y == board.height() - 2)|| (x == 1 && y == board.height() - 2) || (x == 1 && y == board.height() - 1)))
    {
        result -= 64;
    }

    else if (board.cellAt(board.width() - 1, 0) != color && ((x == board.width() - 1 && y == 1) || (x == board.width() - 2 && y ==  0) ||( x == board.width() - 2 && y == 1)))
    {
        result -= 64;
    }

    else if (board.cellAt(board.width() - 1 , board.height() - 1) != color && ((x == board.width() - 1 && y == board.height() - 2) ||( x == board.width() - 2 && y == board.height() - 2) || (x == board.width() - 2 && y == board.height() - 1)))
    {
        result -= 64;
    }

    else if (board.cellAt(0,0) != color && ((x == 1 && y == 1) || ( x == 1 && y == 0) || (x == 0 && y == 1)))
    {
        result -= 64;
    }
    return result;
}

// The MidEvaluation function checks the color of the important positions of the state during the search function in order to make the best move in the future steps by changing the value of the evaluation. 
int yunlum::MyOthelloAI::MidEvaluation( int evaluation, const OthelloBoard& board, OthelloCell& color)
{
    if (board.cellAt(0,0) == color || board.cellAt(0, board.height() - 1) == color || board.cellAt(board.width() - 1, 0) == color || board.cellAt(board.width() - 1 , board.height() - 1) == color)
    {
        evaluation += 64;
    }
    else if (board.cellAt(0, board.height() - 1) != color && (board.cellAt (0,board.height() - 2) == color || board.cellAt (1,board.height() - 2) == color || board.cellAt (1,board.height() - 1) == color)) 
    {
        evaluation -= 64;
    }
    else if (board.cellAt(board.width() - 1, 0) != color && (board.cellAt(board.width() - 1, 1) == color || board.cellAt(board.width() - 2, 0) == color || board.cellAt(board.width() - 2, 1) != color))
    {
        evaluation -= 64;
    }
    else if (board.cellAt(board.width() - 1 , board.height() - 1) != color && (board.cellAt(board.width() - 1 , board.height() - 2) == color || board.cellAt(board.width() - 2 , board.height() - 2) == color || board.cellAt(board.width() - 2 , board.height() - 1) == color)) 
    {
        evaluation -= 64;
    }
    else if (board.cellAt(0,0) != color && (board.cellAt(1,1) == color || board.cellAt(1,0) == color || board.cellAt(0,1) == color))
    {
        evaluation -= 64;
    }
    
    return evaluation;
}
