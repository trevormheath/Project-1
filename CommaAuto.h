//
// Created by trevo on 9/13/2021.
//

#ifndef COMMAAUTO_H
#define COMMAAUTO_H

#include "Automaton.h"

class CommaAuto : public Automaton
{
public:
    CommaAuto() : Automaton(TokenType::COMMA) {}

    void S0(const std::string& input);
};
#endif //COMMAAUTO_H
