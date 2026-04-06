
#include <iostream>
#include "./binserializer/entity.h"
#include "./netcomm/client.h"
#include "./netcomm/server.h"
#include "./btree/btree.h"
#include "common.h"

class StorageClient
{
public:
	StorageClient(const std::string &ipAddr, int port) : netcommClient_{ipAddr, port} {
														 };

	bool saveVal(PointBTWrapper &p)
	{
		auto reqSize = p.sizeInBytes() + 1;

		std::vector<uint8_t> out(reqSize, 0);

		out[0] = 's';

		p.toBytes(out, 1);

		netcommClient_.send_bytes(out);

		std::vector<uint8_t> in{};

		netcommClient_.get_bytes(in);

		return in[0] == '1';
	}

	bool checkVal(PointBTWrapper &p)
	{
		auto reqSize = p.sizeInBytes() + 1;

		std::vector<uint8_t> out(reqSize, 0);

		out[0] = 'f';

		p.toBytes(out, 1);

		netcommClient_.send_bytes(out);

		std::vector<uint8_t> in{};

		netcommClient_.get_bytes(in);

		return in[0] == '1';
	}

	bool deleteVal(PointBTWrapper &p)
	{
		auto reqSize = p.sizeInBytes() + 1;

		std::vector<uint8_t> out(reqSize, 0);

		out[0] = 'd';

		p.toBytes(out, 1);

		netcommClient_.send_bytes(out);

		std::vector<uint8_t> in{};

		netcommClient_.get_bytes(in);

		return in[0] == '1';
	}

	NetCommClient netcommClient_;
};

int main()
{
	StorageClient sc("127.0.0.1", 9050);

	for (size_t i = 0; i < 1000; i++)
	{
		PointBTWrapper p1;
		p1.point_.set_x(rand() % 10000);
		p1.point_.set_y(rand() % 10000);
		p1.point_.set_z(rand() % 10000);

		std::cout << "find " << sc.checkVal(p1) << std::endl;
	}

	// sc.saveVal(p1);

	// std::cout << "find " << sc.checkVal(p1) << std::endl;

	// sc.deleteVal(p1);

	// std::cout << "find " << sc.checkVal(p1) << std::endl;
}