/* 
 * File:   SubEvent.hpp
 * Author: jjdengjj
 */

#ifndef _SUBEVENT_HPP
#define	_SUBEVENT_HPP

#include <string>
#include "NTCFPackage.hpp"
#include "MainClient.hpp"

class SubEvent {
public:
    SubEvent();
    SubEvent(CMSG);
    SubEvent(const SubEvent& orig);
    virtual ~SubEvent();

public:
    SubEvent& operator=(const SubEvent&);
    void Exec();
    std::string m_getEventID();
    NTCFPackage m_getEventPKG();

private:
    std::string m_eventID;
    NTCFPackage m_eventPKG;
};

#endif	/* _SUBEVENT_HPP */

