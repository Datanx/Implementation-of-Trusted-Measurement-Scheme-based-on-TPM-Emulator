/* 
 * File:   SubEvent.cpp
 * Author: jjdengjj
 */

#include "SubEvent.hpp"

SubEvent::SubEvent() {
}

SubEvent::SubEvent(CMSG cmsg)
{
    this->m_eventID = cmsg.id;
    this->m_eventPKG.set_Base64Str(cmsg.msg.c_str());
}

SubEvent::SubEvent(const SubEvent& orig) {
    this->m_eventID = orig.m_eventID;
    this->m_eventPKG = orig.m_eventPKG;
}

SubEvent::~SubEvent() {
}

SubEvent& SubEvent::operator =(const SubEvent& orig)
{
    this->m_eventID = orig.m_eventID;
    this->m_eventPKG = orig.m_eventPKG;
}

std::string SubEvent::m_getEventID()
{
    return this->m_eventID;
}

NTCFPackage SubEvent::m_getEventPKG()
{
    return this->m_eventPKG;
}

