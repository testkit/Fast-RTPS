/*************************************************************************
 * Copyright (c) 2013 eProsima. All rights reserved.
 *
 * This copy of FastCdr is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/*
 * ThreadListen.h
 *
 *  Created on: Feb 25, 2014
 *      Author: Gonzalo Rodriguez Canosa
 *      email:  gonzalorodriguez@eprosima.com
 *      		grcanosa@gmail.com
 */

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>


#include "rtps_all.h"

#include "MessageReceiver.h"



#ifndef THREADLISTEN_H_
#define THREADLISTEN_H_

namespace eprosima {
namespace rtps {

class RTPSReader;
class RTPSWriter;
class Participant;

class ThreadListen {
public:
	ThreadListen();
	virtual ~ThreadListen();
	std::vector<RTPSWriter*> assoc_writers;
	std::vector<RTPSReader*> assoc_readers;
	Participant* participant;
	void listen();
	void init_thread();
	std::vector<Locator_t> locList;
	boost::thread* b_thread;
	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket listen_socket;
	MessageReceiver MR;
	//boost::asio::ip::udp::resolver resolver;


};

} /* namespace rtps */
} /* namespace eprosima */

#endif /* THREADLISTEN_H_ */