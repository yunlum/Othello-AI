#ifndef MYOTHELLOAI_HPP
#define MYOTHELLOAI_HPP

#include "OthelloAI.hpp"


namespace yunlum
{
    class MyOthelloAI: public OthelloAI
    {
        public:
            virtual std::pair<int,int> chooseMove(const OthelloGameState& state);
            std::vector<std::pair<int,int>> PossibleMove(const OthelloGameState& state, const OthelloBoard& board);
            int Eval(OthelloGameState& state, OthelloCell& color);
            int search(OthelloGameState& state,int depth, OthelloCell& color);
            int FinalEvaluation(int x, int y, int result, const OthelloBoard& board, OthelloCell& color);
            int MidEvaluation( int evaluation, const OthelloBoard& board, OthelloCell& color);

    };
}

#endif

