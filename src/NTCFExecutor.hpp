/* 
 * File:   NCSFExecutor.hpp
 * Author: jjdengjj
 *
 * Created on November 24, 2009, 2:58 PM
 */

#ifndef _NCSFEXECUTOR_HPP
#define	_NCSFEXECUTOR_HPP

#include "NTCFPackage.hpp"
#include "HWCollection.hpp"
#include "MainClient.hpp"
#include "SubEvent.hpp"
#include "MainLogger.hpp"
#include "SQLConnector.hpp"
#include "ClientPolicy.hpp"
#include "ServerPolicy.hpp"
#include "t_ntcs_hwpolicy.hpp"
#include <map>

typedef void*(*Func)(SubEvent);


class NTCFExecutor {
public:
    NTCFExecutor();
    NTCFExecutor(const NTCFExecutor& orig);
    virtual ~NTCFExecutor();
    
    static void Exec(SubEvent);
    static void Init();
    
private:
    static void* FuncREQ(SubEvent);
    static void* FuncRTN(SubEvent);
    static void* FuncCLS(SubEvent);
private:
    static std::map<std::string, void*(*)(SubEvent)> mapFunc;
    static bool Status;
    
};

#endif	/* _NCSFEXECUTOR_HPP */

