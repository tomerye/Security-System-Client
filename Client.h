
#ifndef CLIENT_H_
#define CLIENT_H_



#include "Packet.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include <deque>
#include <boost/serialization/vector.hpp>
#include <boost/thread.hpp>

namespace securityClient {

using boost::asio::ip::tcp;

typedef boost::shared_ptr<std::vector<boost::asio::const_buffer> > PacketBufferPtr;

class Client {
public:
	Client(boost::asio::io_service& io_service,
			tcp::resolver::iterator endpoint_iterator);
	virtual ~Client();
	void send(const Packet &packet);

private:
	Packet recv_packet_;
	std::deque<std::vector < PacketBufferPtr> >send_vector_;
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	boost::thread *receive_thread_;

	void handle_connect(const boost::system::error_code& e);
	void handle_read(const boost::system::error_code& e);
	void handle_write(const boost::system::error_code& error);
	void handle_command(const Packet packet);
	void do_write(PacketBufferPtr packet);
	void handle_receive_packet();
	void handle_receive_action(Packet packer);
};

} /* namespace securityClient */
#endif /* CLIENT_H_ */
