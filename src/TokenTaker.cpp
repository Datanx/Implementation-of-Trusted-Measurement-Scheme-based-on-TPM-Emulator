/* 
 * File:   TokenTaker.cpp
 * Author: root
 * 
 * Created on January 8, 2010, 9:29 PM
 */

#include "TokenTaker.h"

TokenTaker::TokenTaker(std::string logic_str) {
    this->m_logic_str = logic_str;
    this->m_logic_str_pointer = 0;
}

TokenTaker::TokenTaker(const TokenTaker& orig) {
}

TokenTaker::~TokenTaker() {
}

int TokenTaker::GetNextToken(std::string& token)
{
    token.clear();
    
    if(this->m_logic_str.empty())
    {
	return GETTOKEN_EMPTYSOURCE;
    }

    while(this->m_logic_str_pointer < this->m_logic_str.length())
    {
	//space found
	if(this->m_logic_str[this->m_logic_str_pointer] == ' ' ||
	  this->m_logic_str[this->m_logic_str_pointer] == '\t')
	{
	    //if token isn't empty, one token found
	    if(!token.empty())
	    {
		this->m_logic_str_pointer++;
		return GETTOKEN_TOKENFOUND;
	    }
	    else //jump over space
	    {
		this->m_logic_str_pointer++;
	    }
	}
	else
	{
	    if(this->m_logic_str[this->m_logic_str_pointer] == '(' ||
	       this->m_logic_str[this->m_logic_str_pointer] == ')')
	    {
		if(!token.empty())
		    return GETTOKEN_TOKENFOUND;
		else
		{
		    token.append(1, this->m_logic_str[this->m_logic_str_pointer]);
		    this->m_logic_str_pointer++;
		    return GETTOKEN_TOKENFOUND;
		}
	    }
	    else
	    {
		token.append(1, this->m_logic_str[this->m_logic_str_pointer]);
		this->m_logic_str_pointer ++;
                if(this->m_logic_str_pointer >= this->m_logic_str.length())
                    return GETTOKEN_TOKENFOUND;
	    }
	}//else
    }//while

    return GETTOKEN_SOURCESCANFIN;
}

void TokenTaker::Truncate()
{
    this->m_logic_str.clear();
    this->m_logic_str_pointer=0;
}
