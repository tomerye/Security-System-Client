/*
 * main.cpp
 *
 *  Created on: Dec 19, 2012
 *      Author: root
 */
#include <iostream>
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
	    io_service.run();
	  }
	  catch (std::exception& e)
	  {
	    std::cerr << e.what() << std::endl;
	  }

	  return 0;
}
