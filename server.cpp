
#include <iostream>
#include <csignal>
#include "./binserializer/entity.h"
#include "./netcomm/client.h"
#include "./netcomm/server.h"
#include "./btree/btree.h"
#include "common.h"

class StorageServer
{
public:
	StorageServer(int port) : initPoint_{}, tree_{&initPoint_, initPoint_.sizeInBytes(), false, false, "nodes", 128, 4}, netcommServer_{port}
	{

		transformFunctionType transformer = [this](const std::vector<uint8_t> &input, std::vector<uint8_t> &output)
		{
			const std::string errorStr = "1";
			const std::string successStr = "0";

			std::string res = successStr;

			if (input[0] == 's')
			{
				PointBTWrapper p{};
				size_t pos = input.size() - 1;

				p.point_.decode_point(input, pos);

				tree_.insertKey(p);

				std::cout << "inserted: " << p.point_.get_x() << " " << p.point_.get_y() << " " << p.point_.get_z() << std::endl;
			}
			else if (input[0] == 'f')
			{
				PointBTWrapper p{};
				size_t pos = input.size() - 1;

				p.point_.decode_point(input, pos);

				if (tree_.searchKey(p) == false)
				{
					res = errorStr;
				}

				std::cout << "searched: " << p.point_.get_x() << " " << p.point_.get_y() << " " << p.point_.get_z() << std::endl;
			}
			else if (input[0] == 'd')
			{
				PointBTWrapper p{};
				size_t pos = input.size() - 1;

				p.point_.decode_point(input, pos);

				tree_.deleteKey(p);

				std::cout << "deleted: " << p.point_.get_x() << " " << p.point_.get_y() << " " << p.point_.get_z() << std::endl;
			}
			else
			{
				res = errorStr;

				std::cout << "ERROR" << std::endl;
			}

			for (size_t i = 0; i < res.size(); i++)
			{
				output.push_back(res[i]);
			}
		};

		netcommServer_.set_transform_function(transformer);
	};

	void run()
	{
		netcommServer_.run();
	}

	PointBTWrapper initPoint_;
	B_Tree tree_;

	NetCommServer netcommServer_;
};

std::function<void()> sighandler;

void signal_handler(int signal)
{
	sighandler();
}

int main()
{
	StorageServer srvr{9050};

	sighandler = [&]()
	{
		srvr.netcommServer_.stop();
		std::cout << "server stopped" << std::endl;
	};

	std::signal(SIGINT, signal_handler);

	std::thread t{
		[&]()
		{
			srvr.run();
		}};

	t.join();
}