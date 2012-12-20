/*
 * Packet.h
 *
 *  Created on: Dec 8, 2012
 *      Author: root
 */

#ifndef PACKET_H_
#define PACKET_H_

namespace securityClient {

struct Packet {

	std::string packet_type_;
	int client_id_;
	int level_;
	std::string image_path_;

	Packet(std::string packet_type, int client_id
			,int level, std::string image_path)
	:packet_type_(packet_type),client_id_(client_id)
	,level_(level),image_path_(image_path)
	{}

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & packet_type_;
		ar & client_id_;
		ar & image_path_;
		ar & level_;
	}



};

} /* namespace securityClient */
#endif /* PACKET_H_ */
