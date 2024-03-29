//
// Created by trevo on 9/29/2021.
//
#include "Rule.h"

Rule::Rule(Predicate inPred, std::vector<Predicate> inPredVector) {
    headPredicate = inPred;
    bodyPredicates = inPredVector;
}
std::vector<Predicate> Rule::getRightHand() {
    return bodyPredicates;
}
Predicate Rule::getHead() {
    return headPredicate;
}
void Rule::to_String() {
    headPredicate.to_String();
    std::cout << " :- ";
    for(unsigned int i = 0; i < bodyPredicates.size(); i++){
        bodyPredicates[i].to_String();
        if(i != bodyPredicates.size()-1){
            std::cout << ",";
        }
    }
    std::cout << ".";
}