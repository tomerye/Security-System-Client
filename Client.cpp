/*
 * Client.cpp
 *
 *  Created on: Dec 8, 2012
 *      Author: root
 */

#include "Client.h"

namespace securityClient {

Client::Client(boost::asio::io_service& io_service,  const std::string& host, const std::string& service):
				connection_(io_service)
{
	// Resolve the host name into an IP address.
	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(host, service);
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

	// Start an asynchronous connect operation.
	boost::asio::async_connect(connection_.socket(), endpoint_iterator,
			boost::bind(&Client::handle_connect, this,boost::asio::placeholders::error));
}

/// Handle completion of a connect operation.
void Client::handle_connect(const boost::system::error_code& e, connection_ptr conn)
{
	if (!e)
	{
		Packet p;
		p.packet_type_="hello";
		p.image_path_="";
		p.level_=-1;
		p.client_id_=123;
		send_vector_.push_back(p);
		connection_.async_write(send_vector_,
				boost::bind(&Client::do_nothing, this,
						boost::asio::placeholders::error, conn));
	}
	else
	{
		std::cerr << e.message() << std::endl;
	}
}

void do_nothing(boost::system::error_code& e, connection_ptr conn)
{

}
/// Handle completion of a read operation.
void Client::handle_read(const boost::system::error_code& e)
{
	if (!e)
	{
		for(std::size_t i=0;i<recv_vector_.size();++i)
		{
			handle_command(recv_vector_[i]);
		}
	}
	else
	{
		// An error occurred.
		std::cerr << e.message() << std::endl;
	}

}

void Client::handle_write()
{
	send_vector_.pop_front();
	if(!send_vector_.empty())
	{
		connection_.async_write(send_vector_.front(),
				boost::bind(&Client::handle_write, this, boost::asio::placeholders::error, connection_));
	}
}
void Client::send(const Packet packet)
{
	bool write_in_progress = !send_vector_.empty();
	send_vector_.push_back(packet);
	if(!write_in_progress)
	{
		connection_.async_write(send_vector_.front(),
				boost::bind(&Client::handle_write, this, boost::asio::placeholders::error, connection_));
	}
}

void Client::handle_command(const Packet packet)
{
	std::cout << "receve packet" << packet.client_id_;
}

Client::~Client() {
	// TODO Auto-generated destructor stub
}

} /* namespace securityClient */

