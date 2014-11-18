/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file Participant.h
 */

#ifndef PARTICIPANTIMPL_H_
#define PARTICIPANTIMPL_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#if defined(_WIN32)
#include <process.h>
#else
#include <unistd.h>
#endif


namespace boost
{
namespace interprocess
{
class interprocess_semaphore;
}
}

#include "eprosimartps/pubsub/attributes/all_attributes.h"

#include "eprosimartps/resources/ResourceEvent.h"
#include "eprosimartps/resources/ResourceSend.h"
#include "eprosimartps/builtin/BuiltinProtocols.h"



namespace eprosima {

namespace pubsub
{
class RTPSDomain;
class TopicDataType;
class PublisherListener;
class SubscriberListener;
class ParticipantListener;


}

using namespace pubsub;

namespace rtps {

class StatelessWriter;
class StatelessReader;
class StatefulWriter;
class StatefulReader;
class RTPSReader;
class RTPSWriter;






/**
 * @brief Class ParticipantImpl, it contains the private implementation of the Participant functions and allows the creation and removal of writers and readers. It manages the send and receive threads.
 * @ingroup MANAGEMENTMODULE
 */
class ParticipantImpl
{
public:
	ParticipantImpl(const ParticipantAttributes &param,
			const GuidPrefix_t& guidP,ParticipantListener* plisten);
	virtual ~ParticipantImpl();


private:
	//!Attributes of the Participant
	ParticipantAttributes m_att;
	//!Guid of the participant.
	const GUID_t m_guid;
	//! Sending resources.
	ResourceSend m_send_thr;
	//! Event Resource
	ResourceEvent m_event_thr;
	//! BuiltinProtocols of this participant
	BuiltinProtocols m_builtinProtocols;
public:
	/**
	 * Create a Writer in this Participant.
	 * @param Writer Pointer to pointer of the Writer, used as output. Only valid if return==true.
	 * @param param WriterAttributes to define the Writer.
	 * @param entityId EntityId assigned to the Writer.
	 *  * @param isBuiltin Bool value indicating if the Writer is builtin (Discovery or Liveliness protocol) or is created for the end user.
	 * @return True if the Writer was correctly created.
	 */
	bool createWriter(RTPSWriter** Writer,EndpointAttributes& param,const EntityId_t& entityId = c_EntityId_Unknown,
			bool isBuiltin = false);


	/**
	 * Create a Reader in this Participant.
	 * @param Reader Pointer to pointer of the Reader, used as output. Only valid if return==true.
	 * @param RParam SubscriberAttributes to define the Reader.
	 * @param payload_size Maximum payload size.
	 * @param isBuiltin Bool value indicating if the Reader is builtin (Discovery or Liveliness protocol) or is created for the end user.
	 * @param kind STATEFUL or STATELESS.
	 * @param ptype Pointer to the TOpicDataType object (optional).
	 * @param slisten Pointer to the SubscriberListener object (optional).
	 * @param entityId EntityId assigned to the Reader.
	 * @return True if the Reader was correctly created.
	 */
	bool createReader(RTPSReader** Reader,SubscriberAttributes& RParam,uint32_t payload_size,bool isBuiltin,StateKind_t kind,
			TopicDataType* ptype = NULL,SubscriberListener* slisten=NULL,const EntityId_t& entityId = c_EntityId_Unknown);

	/**
	 * Register a writer in the builtin protocols.
	 * @param Writer Pointer to the RTPSWriter to register.
	 */
	void registerWriter(RTPSWriter* Writer);
	/**
	 * Register a reader in the builtin protocols.
	 * @param Reader Pointer to the RTPSReader to register.
	 */
	void registerReader(RTPSReader* Reader);
	/**
	 * Assign an endpoint to the listenResources.
	 * @param endp Pointer to the endpoint.
	 * @param isBuiltin Boolean indicating if it is builtin.
	 * @return True if correct.
	 */
	bool assignEndpointListenResources(Endpoint* endp,bool isBuiltin);
	/**
	 * Assign a locator to a listen resources.
	 * @param pend Pointer to the endpoint.
	 * @param lit Locator list iterator.
	 * @param isMulticast Boolean indicating that is multicast.
	 * @param isFixed Boolean indicating that is a fixed listenresource.
	 * @return True if assigned.
	 */
	bool assignLocator2ListenResources(Endpoint* pend,LocatorListIterator lit,bool isMulticast,bool isFixed);


	/**
	 * Remove Endpoint from the participant. It closes all entities related to them that are no longer in use.
	 * For example, if a ResourceListen is not useful anymore the thread is closed and the instance removed.
	 * @param[in] p_endpoint Pointer to the Endpoint that is going to be removed.
	 * @param[in] type Char indicating if it is Reader ('R') or Writer ('W')
	 * @return True if correct.
	 */
	bool deleteUserEndpoint(Endpoint* p_endpoint,char type);


	std::vector<RTPSReader*>::iterator userReadersListBegin(){return m_userReaderList.begin();};

	std::vector<RTPSReader*>::iterator userReadersListEnd(){return m_userReaderList.end();};

	std::vector<RTPSWriter*>::iterator userWritersListBegin(){return m_userWriterList.begin();};

	std::vector<RTPSWriter*>::iterator userWritersListEnd(){return m_userWriterList.end();};


	//!Used for tests
	void loose_next_change(){m_send_thr.loose_next();};
	//! Announce ParticipantState (force the sending of a DPD message.)
	void announceParticipantState();
	//!Stop the Participant Announcement (used in tests to avoid multiple packets being send)
	void stopParticipantAnnouncement();
	//!Reset to timer to make periodic Participant Announcements.
	void resetParticipantAnnouncement();
	/**
	 * Get the GUID_t of the Participant.
	 * @return GUID_t of the Participant.
	 */
	const GUID_t& getGuid() const {
		return m_guid;
	}
	/**
	 * Get the Participant Name.
	 * @return String with the participant Name.
	 */
	const std::string& getParticipantName() const {
		return m_participantName;
	}



	void ResourceSemaphorePost();

	void ResourceSemaphoreWait();

	const BuiltinAttributes& getBuiltinAttributes() const {
		return m_builtin;
	}

	ResourceEvent* getEventResource()
	{
		return &m_event_thr;
	}

	uint32_t getParticipantId() const {
		return m_participantID;
	}

	void setParticipantId(uint32_t participantId) {
		m_participantID = participantId;
	}

	uint32_t getListenSocketBufferSize() const {
		return m_listen_socket_buffer_size;
	}

	uint32_t getSendSocketBufferSize() const {
		return m_send_socket_buffer_size;
	}

	BuiltinProtocols* getBuiltinProtocols(){return &m_builtinProtocols;}

	bool existsEntityId(const EntityId_t& ent,EndpointKind_t kind) const;

	bool newRemoteEndpointDiscovered(const GUID_t& pguid, int16_t userDefinedId,EndpointKind_t kind);

	void setListener(ParticipantListener* lis) {mp_participantListener = lis;}

	ParticipantListener* getListener() const {return mp_participantListener;}

	Participant* getUserParticipant() const {return mp_userParticipant;}

	std::vector<octet> getUserData() const {return m_userData;}

	uint32_t getParticipantID() const{return m_participantID;}
private:




	//!Semaphore to wait for the listen thread creation.
	boost::interprocess::interprocess_semaphore* mp_ResourceSemaphore;
	//!Id counter to correctly assign the ids to writers and readers.
	uint32_t IdCounter;
	//!Writer List.
	std::vector<RTPSWriter*> m_allWriterList;
	//!Reader List
	std::vector<RTPSReader*> m_allReaderList;
	//!Listen thread list.
	//!Writer List.
	std::vector<RTPSWriter*> m_userWriterList;
	//!Reader List
	std::vector<RTPSReader*> m_userReaderList;
	//!Listen thread list.
	//std::vector<ResourceListen*> m_threadListenList;

	std::vector<ListenResource*> m_listenResourceList;
	/*!
	 * Assign a given Endpoint to one of the current listen thread or create a new one.
	 * @param[in] endpoint Pointer to the Endpoint to add.
	 * @param[in] type Type of the Endpoint (R or W)(Reader or Writer).
	 * @param[in] isBuiltin Indicates if the endpoint is Builtin or not.
	 * @return True if correct.
	 */
	bool assignEnpointToListenResources(Endpoint* endpoint,char type,bool isBuiltin);
	//	/*!
	//	 * Create a new listen thread in the specified locator.
	//	 * @param[in] loc Locator to use.
	//	 * @param[out] listenthread Pointer to pointer of this class to correctly initialize the listening recourse.
	//	 * @param[in] isMulticast To indicate whether the new lsited thread is multicast.
	//	 * @param[in] isBuiltin Indicates that the endpoint is builtin.
	//	 * @return True if correct.
	//	 */
	//	bool addNewListenResource(Locator_t& loc,ResourceListen** listenthread,bool isMulticast,bool isBuiltin);

	//ParticipantDiscoveryProtocol* mp_PDP;






	ParticipantListener* mp_participantListener;





};


} /* namespace rtps */
} /* namespace eprosima */

#endif /* PARTICIPANT_H_ */




