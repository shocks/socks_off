// #include <iostream>
// #include <string>
// #include <vector>
// #include <functional>
// #include "boost/asio/io_service.hpp"
// #include "boost/asio/ip/address.hpp"
// #include "redisclient/redissyncclient.h"
// #include "redisclient/redisasyncclient.h"
 
// // 设置redis服务器的ip和port
// const boost::asio::ip::address server_address = boost::asio::ip::address::from_string("127.0.0.1");
// const unsigned short server_port = 6379;
 
// // 同步方式连接操作
// void handleRedisSyncClient(
// 	boost::asio::io_service &io_service,
// 	boost::asio::ip::address address,
// 	unsigned short port)
// {
// 	// 定义redisclient
// 	redisclient::RedisSyncClient redis(io_service);
// 	std::string errmsg;
 
// 	if (!redis.connect(address, port, errmsg))
// 		std::cerr << "Can't connect to redis: " << errmsg << std::endl;
 
// 	redisclient::RedisValue result;
 
// 	// 执行redis命令
// 	//result = redis.command("lpush", {"list", "value1"});
// 	//result = redis.command("lpush", {"list", "value2"});
 
// 	//if (result.isError())
// 	//	std::cerr << "error: " << result.toString() << "\n";
 
// 	result = redis.command("lrange", {"list", "0", "1"});
 
// 	if (result.isOk())
// 		std::cout << result.toString() << "\n";
// 	else
// 		std::cerr << "error: " << result.toString() << "\n";
// }
 
// // 异步方式连接操作
// void handleRedisAsycClient(
// 	boost::asio::io_service &ioService, 
// 	redisclient::RedisAsyncClient &redis_client,
// 	std::string &key,
// 	std::string &value,
// 	bool ok, 
// 	const std::string &errmsg)
// {
// 	if (ok)
// 	{
// 		// 发送指令
// 		redis_client.command("set", {key, value}, [&](const redisclient::RedisValue &v) {
// 			std::cout << "set: " << v.toString() << std::endl;
			
// 			// 获得回调，再调用指令
// 			redis_client.command("get", {key}, [&](const redisclient::RedisValue &v) {
// 				std::cerr << "get: " << v.toString() << std::endl;
 
				
// 				/*redis_client.command("del", {key}, [&](const redisclient::RedisValue &) {
// 					ioService.stop();
// 				});*/
// 			});
// 		});
// 	}
// 	else
// 	{
// 		std::cerr << "Can't connect to redis: " << errmsg << std::endl;
// 	}
// }
 
// int main(int argc, char **argv)
// {
// 	boost::asio::io_service ioService;
 
// 	// 1,同步
// 	//handleRedisSyncClient(ioService, server_address, server_port);
	
// 	// 2，异步
// 	redisclient::RedisAsyncClient redis_client(ioService);
// 	std::string mykey = "age";
// 	std::string myvalue = "23";
// 	redis_client.asyncConnect(server_address, server_port,
// 		std::bind(
// 			&handleRedisAsycClient,
// 			std::ref(ioService), 
// 			std::ref(redis_client),
// 			std::ref(mykey),
// 			std::ref(myvalue),
// 			std::placeholders::_1,
// 			std::placeholders::_2));
// 	ioService.run();
 
// 	getchar();
// 	return 0;
// }