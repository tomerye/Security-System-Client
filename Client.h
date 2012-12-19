/*
 * Client.h
 *
 *  Created on: Dec 8, 2012
 *      Author: root
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "Connection.h"
#include "Packet.h"
#include "/home/tomer/project/boost_1_52_0/boost/asio.hpp"
#include </home/tomer/project/boost_1_52_0/boost/bind.hpp>
#include <iostream>
#include <vector>
#include <deque>
#include </home/tomer/project/boost_1_52_0/boost/serialization/vector.hpp>

namespace securityClient {

class Client {
public:
	Client(boost::asio::io_service& io_service, const std::string& host,const std::string& service);
	virtual ~Client();
	void send(const Packet packet);

private:
	std::vector<Packet> recv_vector_;
	std::deque<Packet> send_vector_;
	Connection connection_;
	void handle_connect(const boost::system::error_code& e, connection_ptr conn);
	void handle_read(const boost::system::error_code& e);
	void handle_write();
	void handle_command(const Packet packet);
	void do_nothing(boost::system::error_code& e, connection_ptr Conn);
};

} /* namespace securityClient */
#endif /* CLIENT_H_ */
