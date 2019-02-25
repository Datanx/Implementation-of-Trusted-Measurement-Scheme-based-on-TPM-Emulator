/* 
 * File:   TokenTaker.h
 * Author: root
 *
 * Created on January 8, 2010, 9:29 PM
 */

#ifndef _TOKENTAKER_H
#define	_TOKENTAKER_H

//get token return values
#define     GETTOKEN_EMPTYSOURCE    1
#define     GETTOKEN_TOKENFOUND     2
#define     GETTOKEN_SOURCESCANFIN  3

#include <string>

class TokenTaker {
public:
    TokenTaker(std::string logic_str);
    TokenTaker(const TokenTaker& orig);
    virtual ~TokenTaker();
    int GetNextToken(std::string& token);
    void Truncate();    //initialize all members
private:
    int m_logic_str_pointer;
    std::string m_logic_str;
};

#endif	/* _TOKENTAKER_H */

