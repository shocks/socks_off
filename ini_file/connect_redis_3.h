#include <hiredis/hiredis.h>
#include <iostream>
#include<string>
#include<queue>
#include <boost/shared_ptr.hpp>
#include<map>
#include"redis_struct.h"
using std::string;
using std::queue;
using std::cout;
using std::endl;

class KGRedisClient
{
public:
    KGRedisClient(string ip, int port, int timeout = 2000);
    virtual ~KGRedisClient();
 
    bool ExecuteCmd(const char *cmd, size_t len, string &response);
    redisReply* ExecuteCmd(const char *cmd, size_t len);
    redisContext* CreateContext();
private:
    int m_timeout;
    
    string m_setverIp;
    int m_serverPort;
    std::string m_serverPsd;
	int32_t m_serverDbIdx;							//db idx
  //  CCriticalSection m_lock;
    std::queue<redisContext *> m_clients;
 
    time_t m_beginInvalidTime;
    static const int m_maxReconnectInterval = 3;
 
    
    void ReleaseContext(redisContext *ctx, bool active);
    bool CheckStatus(redisContext *ctx);
    
};