/*
 * Connection.h
 *
 *  Created on: Dec 8, 2012
 *      Author: root
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

namespace securityClient {

class Connection {
public:
	Connection(boost::asio::io_service& io_service);
	virtual ~Connection();
	boost::asio::ip::tcp::socket& socket();

	template<typename T, typename Handler>
	void async_write(const T& t, Handler handler);

	/// Asynchronously read a data structure from the socket.
	template<typename T, typename Handler>
	void async_read(T& t, Handler handler);

	/// Handle a completed read of a message header. The handler is passed using
	/// a tuple since boost::bind seems to have trouble binding a function object
	/// created using boost::bind as a parameter.
	template<typename T, typename Handler>
	void handle_read_header(const boost::system::error_code& e, T& t,
			boost::tuple<Handler> handler);

	/// Handle a completed read of message data.
	template<typename T, typename Handler>
	void handle_read_data(const boost::system::error_code& e, T& t,
			boost::tuple<Handler> handler);

private:
	/// The underlying socket.
	boost::asio::ip::tcp::socket socket_;

	/// The size of a fixed length header.
	enum {
		header_length = 8
	};

	/// Holds an outbound header.
	std::string outbound_header_;

	/// Holds the outbound data.
	std::string outbound_data_;

	/// Holds an inbound header.
	char inbound_header_[header_length];

	/// Holds the inbound data.
	std::vector<char> inbound_data_;
};

typedef boost::shared_ptr<Connection> connection_ptr;


} /* namespace securityClient */
#endif /* CONNECTION_H_ */
