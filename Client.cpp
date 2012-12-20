#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include "Client.h"
#include "Packet.h"
#include <boost/shared_ptr.hpp>

namespace securityClient {

Client::Client(boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_iterator) :
		io_service_(io_service), socket_(io_service) {
	boost::asio::async_connect(socket_, endpoint_iterator,
			boost::bind(&Client::handle_connect, this,
					boost::asio::placeholders::error));
}

void Client::handle_connect(const boost::system::error_code& e) {
	receive_thread_ = new boost::thread(&Client::handle_receive_packet);
}

void Client::handle_receive_packet() {
	// read header
	size_t header;
	boost::asio::read(socket_, boost::asio::buffer(&header, sizeof(header)));
    std::cout << "body is " << header << " bytes" << std::endl;
	// read body
	boost::asio::streambuf buf;
	const size_t rc = boost::asio::read(socket_, buf.prepare(header));
	buf.commit(header);
	std::cout << "read " << rc << " bytes" << std::endl;
	// deserialize
	std::istream is(&buf);
	boost::archive::text_iarchive ar(is);
	ar & recv_packet_;
	handle_receive_action(recv_packet_);
}

void Client::handle_receive_action(Packet packet)
{
	std::cout << packet.client_id_;
}

void securityClient::Client::do_write(PacketBufferPtr serializedBufffer) {
	bool write_in_progress = !send_vector_.empty();
	send_vector_.push_back(serializedBufffer);
	if (!write_in_progress) {
		boost::asio::async_write(socket_, send_vector_.front(),
				boost::bind(&Client::handle_write, this,
						boost::asio::placeholders::error));
	}
}
void securityClient::Client::send(const Packet &packet) {
	boost::asio::streambuf buf;
	std::ostream os(&buf);
	boost::archive::text_oarchive ar(os);
	ar & packet;
	const size_t header = buf.size();
	PacketBufferPtr serializedBufffer = new std::vector<boost::asio::const_buffer>();
	serializedBufffer->push_back(boost::asio::buffer(&header, sizeof(header)));
	serializedBufffer->push_back(buf.data());
	io_service_.post(boost::bind(&Client::do_write, this, serializedBufffer));

}

void securityClient::Client::handle_write(
		const boost::system::error_code& error) {
	if (!error) {
		send_vector_.pop_front();
		if (!send_vector_.empty()) {
			boost::asio::async_write(socket_, send_vector_.front(),
					boost::bind(&Client::handle_write, this,
							boost::asio::placeholders::error));
		}
	} else {
		socket_.close();
	}
}
} //namespace
