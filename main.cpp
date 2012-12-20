/*
 * main.cpp
 *
 *  Created on: Dec 19, 2012
 *      Author: root
 */
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include "connection.hpp" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include "stock.hpp"
#include "Client.cpp"


int main(int argc, char* argv[])
{

	  try
	  {
	    // Check command line arguments.
	    if (argc != 3)
	    {
	      std::cerr << "Usage: client <host> <port>" << std::endl;
	      return 1;
	    }

	    boost::asio::io_service io_service;
	    s11n_example::client client(io_service, argv[1], argv[2]);
	    io_service.run();
	  }
	  catch (std::exception& e)
	  {
	    std::cerr << e.what() << std::endl;
	  }

	  return 0;
}
