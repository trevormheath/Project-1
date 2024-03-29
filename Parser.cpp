//
// Created by trevo on 9/27/2021.
//

#include "Parser.h"


Parser::Parser(std::vector<Token*> tokensList) {
    tokens = tokensList;
}
void Parser::parse(datalogProgram &fileStorage){

    for(unsigned int i = 0; i < tokens.size(); i++){
        if(tokens[i]->match(TokenType::COMMENT)){
           tokens.erase(tokens.begin() + i);
           i--;
        }
    }
    try {
        this->match(TokenType::SCHEMES);
        this->match(TokenType::COLON);
        this->parseScheme();
        if(tokens[tokenIndex]->getToken() != TokenType::FACTS) {
            this->parseSchemeList();
        }
        if(tokens[tokenIndex]->getToken() == TokenType::FACTS) {
            this->match(TokenType::FACTS);
            this->match(TokenType::COLON);
            this->parseFactList();
        }
        if(tokens[tokenIndex]->getToken() == TokenType::RULES) {
            this->match(TokenType::RULES);
            this->match(TokenType::COLON);
            this->parseRuleList();
        }
        this->match(TokenType::QUERIES);
        this->match(TokenType::COLON);
        this->parseQuery();
        this->parseQueryList();

        this->match(TokenType::EOF_CP);
    }
    catch(Token* a){
        std::cout << "Failure!\n  (" << a->codeToString() << ",\"" << a->getDesc() << "\"," << a->getLineNum() << ")";
        return;
    }
   /* std::cout << "Success!\n";

    std::cout << "Schemes(" << schemes.size() << "):\n";
   for(unsigned int i = 0; i < schemes.size(); i++){
       std::cout << "  ";
       schemes[i].to_String();
       std::cout << "\n";
   }
    std::cout << "Facts(" << facts.size() << "):\n";
    for(unsigned int i = 0; i < facts.size(); i++){
        std::cout << "  ";
        facts[i].to_String();
        std::cout << ".\n";
    }
    std::cout << "Rules(" << rules.size() << "):\n";
    for(unsigned int i = 0; i < rules.size(); i++){
        std::cout << "  ";
        rules[i].to_String();
        std::cout << "\n";
    }
    std::cout << "Queries(" << queries.size() << "):\n";
    for(unsigned int i = 0; i < queries.size(); i++){
        std::cout << "  ";
        queries[i].to_String();
        std::cout << "?\n";
    }
    std::cout << "Domain(" << domain.size() << "):\n";
    for(auto i = domain.begin(); i != domain.end(); i++){
        std::cout << "  " << *i;
        std::cout << "\n";
    }*/
    fileStorage.copy_scheme(schemes);
    fileStorage.copy_fact(facts);
    fileStorage.copy_rule(rules);
    fileStorage.copy_query(queries);
}

void Parser::match(TokenType a) {
    if(tokens[tokenIndex]->getToken() == a){
        tokenIndex++;
    }
    else{
        throw tokens[tokenIndex];
    }
}

void Parser::parseScheme() {
    std::string tempID;
    std::vector<Parameter> tempParam;
    this->match(TokenType::ID);
    tempID = tokens[tokenIndex-1]->getDesc();
    this->match(TokenType::LEFT_PAREN);
    this->match(TokenType::ID);
    tempParam.push_back(tokens[tokenIndex-1]->getDesc());
    if(tokens[tokenIndex]->getToken() != TokenType::RIGHT_PAREN) {
        tempParam = this->parseIdList(tempParam);
    }
    Predicate a(tempID, tempParam);
    schemes.push_back(a);
    this->match(TokenType::RIGHT_PAREN);
}
void Parser::parseSchemeList() {
    if(tokens[tokenIndex]->getToken() != TokenType::FACTS) {
        this->parseScheme();
        this->parseSchemeList();
    }
}
void Parser::parseFactList() {
    if(tokens[tokenIndex]->getToken() != TokenType::RULES) {
        this->parseFact();
        this->parseFactList();
    }
}
void Parser::parseFact() {
    std::string tempID;
    std::vector<Parameter> tempParam;
    this->match(TokenType::ID);
    tempID = tokens[tokenIndex-1]->getDesc();
    this->match(TokenType::LEFT_PAREN);
    this->match(TokenType::STRING);
    tempParam.push_back(tokens[tokenIndex-1]->getDesc());
    domain.insert(tokens[tokenIndex-1]->getDesc());
    if(tokens[tokenIndex]->getToken() != TokenType::RIGHT_PAREN) {
        tempParam = this->parseStringList(tempParam);
    }
    Predicate a(tempID, tempParam);
    facts.push_back(a);
    this->match(TokenType::RIGHT_PAREN);
    this->match(TokenType::PERIOD);
}
void Parser::parseRuleList() {
    if(tokens[tokenIndex]->getToken() != TokenType::QUERIES) {
        this->parseRule();
        this->parseRuleList();
    }
}
void Parser::parseRule(){
    std::vector<Predicate> tempPred;
    Predicate headPred = this->parseHeadPredicate();
    this->match(TokenType::COLON_DASH);
    tempPred.push_back(this->parsePredicate());
    tempPred = this->parsePredicateList(tempPred);
    this->match(TokenType::PERIOD);
    Rule a(headPred,tempPred);
    rules.push_back(a);
}
std::vector<Parameter> Parser::parseStringList(std::vector<Parameter> inParam) {
    std::vector<Parameter> tempParam = inParam;
    this->match(TokenType::COMMA);
    this->match(TokenType::STRING);
    tempParam.push_back(tokens[tokenIndex-1]->getDesc());
    domain.insert(tokens[tokenIndex-1]->getDesc());
    if(tokens[tokenIndex]->getToken() != TokenType::RIGHT_PAREN) {
        tempParam = this->parseStringList(tempParam);
    }
    return tempParam;
}
std::vector<Parameter> Parser::parseIdList(std::vector<Parameter> inParam) {
    std::vector<Parameter> tempParam = inParam;
    this->match(TokenType::COMMA);
    this->match(TokenType::ID);
    tempParam.push_back(tokens[tokenIndex-1]->getDesc());
    if(tokens[tokenIndex]->getToken() != TokenType::RIGHT_PAREN) {
        tempParam = this->parseIdList(tempParam);
    }
    return tempParam;
}
Predicate Parser::parseHeadPredicate() {
    //just have it run a scheme lol
    std::vector<Parameter> tempParam;
    std::string tempID;
    this->match(TokenType::ID);
    tempID = tokens[tokenIndex-1]->getDesc();
    this->match(TokenType::LEFT_PAREN);
    this->match(TokenType::ID);
    tempParam.push_back(tokens[tokenIndex-1]->getDesc());
    if(tokens[tokenIndex]->getToken() != TokenType::RIGHT_PAREN) {
        tempParam = this->parseIdList(tempParam);
    }
    Predicate a(tempID, tempParam);
    this->match(TokenType::RIGHT_PAREN);
    return a;
}
Predicate Parser::parsePredicate() {
    std::string tempID;
    std::vector<Parameter> tempParam;
    this->match(TokenType::ID);
    tempID = tokens[tokenIndex-1]->getDesc();
    this->match(TokenType::LEFT_PAREN);
    tempParam.push_back(this->parseParameter());
    if(tokens[tokenIndex]->getToken() != TokenType::RIGHT_PAREN) {
        tempParam = this->parseParameterList(tempParam);
    }
    this->match(TokenType::RIGHT_PAREN);
    Predicate tempPred(tempID,tempParam);
    return tempPred;
}
std::vector<Predicate> Parser::parsePredicateList(std::vector<Predicate> inPred) {
    std::vector<Predicate> tempPred = inPred;
    if(tokens[tokenIndex]->getToken() != TokenType::PERIOD) {
        this->match(TokenType::COMMA);
        tempPred.push_back(this->parsePredicate());
    }
    if(tokens[tokenIndex]->getToken() != TokenType::PERIOD) {
        tempPred = this->parsePredicateList(tempPred);
    }
    return tempPred;
}
Parameter Parser::parseParameter() {
    if(tokens[tokenIndex]->getToken() == TokenType::STRING) {
        this->match(TokenType::STRING);
        return tokens[tokenIndex-1]->getDesc();
    }
    else{
        this->match(TokenType::ID);
        return tokens[tokenIndex-1]->getDesc();
    }
}
std::vector<Parameter> Parser::parseParameterList(std::vector<Parameter> inParam) {
    std::vector<Parameter> tempParam = inParam;
    this->match(TokenType::COMMA);
    tempParam.push_back(this->parseParameter());
    if(tokens[tokenIndex]->getToken() != TokenType::RIGHT_PAREN) {
        tempParam = this->parseParameterList(tempParam);
    }
    return tempParam;
}
void Parser::parseQuery() {
    std::vector<Predicate> tempPred;
    queries.push_back(this->parsePredicate());
    this->match(TokenType::Q_MARK);
}
void Parser::parseQueryList() {
    if (tokens[tokenIndex]->getToken() != TokenType::EOF_CP) {
        this->parseQuery();
        this->parseQueryList();
    }
}