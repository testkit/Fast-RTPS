/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file Endpoint.h
 */



#ifndef ENDPOINT_H_
#define ENDPOINT_H_
#include "eprosimartps/common/types/common_types.h"
#include "eprosimartps/common/types/Locator.h"
#include "eprosimartps/common/types/Guid.h"


namespace boost
{
	class recursive_mutex;
}

namespace eprosima {
namespace rtps {

class ParticipantImpl;
class ResourceSend;
class ResourceEvent;

class EndpointAttributes
{
public:
	EndpointAttributes()
	{
		topicKind = NO_KEY;
		reliabilityKind = BEST_EFFORT;
		payloadMaxSize = 0;
		m_userDefinedID = -1;
		m_entityID = -1;
	};
	virtual ~EndpointAttributes();
	TopicKind_t topicKind;
	ReliabilityKind_t reliabilityKind;
	uint32_t payloadMaxSize;
	LocatorList_t unicastLocatorList;
	LocatorList_t multicastLocatorList;
	inline int16_t getUserDefinedID() const {return m_userDefinedID;}
	inline int16_t getEntityID() const {return m_entityID;}
private:
	int16_t m_userDefinedID;
	int16_t m_entityID;
};



/**
 * Class Endpoint, all entities of the RTPS network are a specification of this class.
 * Although the Participant is also defined as an endpoint in the RTPS specification in this implementation
 * the Participant class DOESN'T inherit from this class. The elements needed where added directly to the
 * Participant class. This way each instance of our class (Endpoint) has a pointer to the participant they belong to.
 * @ingroup COMMONMODULE
 */
class Endpoint
{
public:
	Endpoint(ParticipantImpl* pimpl,GuidPrefix_t guid,EntityId_t entId,ReliabilityKind_t rl = BEST_EFFORT,EndpointKind_t end = WRITER,int16_t userDefinedId=-1);
	virtual ~Endpoint();

	inline const GUID_t& getGuid() const {	return m_guid;	}

	inline boost::recursive_mutex* getMutex() const {return mp_mutex;}

	inline ParticipantImpl* getParticipant() const {return mp_participant;}


protected:
	//!Pointer to the participant containing this endpoints
	ParticipantImpl* mp_participant;
	//! Guid of the Endpoint
	const GUID_t m_guid;
	//!Endpoint Attributes
	EndpointAttributes m_att;
	//!Mutex of the object
	boost::recursive_mutex* mp_mutex;
	//!Send Resource
	ResourceSend* mp_send_thr;
	//!Event Resource
	ResourceEvent* mp_event_thr;
};



} /* namespace rtps */
} /* namespace eprosima */

#endif /* ENDPOINT_H_ */