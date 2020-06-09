#include <hiredis/hiredis.h>
#include <iostream>
#include<string>
#include<queue>
#include <boost/shared_ptr.hpp>


using std::string;
using std::queue;

class KGRedisClient
{
public:
    KGRedisClient(string ip, int port, int timeout = 2000);
    virtual ~KGRedisClient();
 
    bool ExecuteCmd(const char *cmd, size_t len, string &response);
    redisReply* ExecuteCmd(const char *cmd, size_t len);
 
private:
    int m_timeout;
    int m_serverPort;
    string m_setverIp;
  //  CCriticalSection m_lock;
    std::queue<redisContext *> m_clients;
 
    time_t m_beginInvalidTime;
    static const int m_maxReconnectInterval = 3;
 
    redisContext* CreateContext();
    void ReleaseContext(redisContext *ctx, bool active);
    bool CheckStatus(redisContext *ctx);
};
 
KGRedisClient::KGRedisClient(string ip, int port, int timeout)
{
    m_timeout = timeout;
    m_serverPort = port;
    m_setverIp = ip;
 
    m_beginInvalidTime = 0;
}
 
KGRedisClient::~KGRedisClient()
{
   // CAutoLock autolock(m_lock);
    while(!m_clients.empty())
    {
        redisContext *ctx = m_clients.front();
        redisFree(ctx);
        m_clients.pop();
    }
}
 
bool KGRedisClient::ExecuteCmd(const char *cmd, size_t len,string &response)
{
    redisReply *reply = ExecuteCmd(cmd, len);
    if(reply == NULL) return false;
 
    boost::shared_ptr<redisReply> autoFree(reply, freeReplyObject);
    if(reply->type == REDIS_REPLY_INTEGER)
    {
        response = std::to_string(reply->integer);
        return true;
    }
    else if(reply->type == REDIS_REPLY_STRING)
    {
        response.assign(reply->str, reply->len);
        return true;
    }
    else if(reply->type == REDIS_REPLY_STATUS)
    {
        response.assign(reply->str, reply->len);
        return true;
    }
    else if(reply->type == REDIS_REPLY_NIL)
    {
        response = "";
        return true;
    }
    else if(reply->type == REDIS_REPLY_ERROR)
    {
        response.assign(reply->str, reply->len);
        return false;
    }
    else if(reply->type == REDIS_REPLY_ARRAY)
    {
        response = "Not Support Array Result!!!";
        return false;
    }
    else
    {
        response = "Undefine Reply Type";
        return false;
    }
}
 
redisReply* KGRedisClient::ExecuteCmd(const char *cmd, size_t len)
{
    redisContext *ctx = CreateContext();
    if(ctx == NULL) return NULL;
 
    redisReply *reply = (redisReply*)redisCommand(ctx, "%b", cmd, len);
 
    ReleaseContext(ctx, reply != NULL);
 
    return reply;
}
 
redisContext* KGRedisClient::CreateContext()
{
    {
      //  CAutoLock autolock(m_lock);
        if(!m_clients.empty())
        {
            redisContext *ctx = m_clients.front();
            m_clients.pop();
 
            return ctx;
        }
    }
 
    time_t now = time(NULL);
    if(now < m_beginInvalidTime + m_maxReconnectInterval) return NULL;
 
    struct timeval tv;
    tv.tv_sec = m_timeout / 1000;
    tv.tv_usec = (m_timeout % 1000) * 1000;;
    redisContext *ctx = redisConnectWithTimeout(m_setverIp.c_str(), m_serverPort, tv);
    if(ctx == NULL || ctx->err != 0)
    {
        if(ctx != NULL) redisFree(ctx);
 
        m_beginInvalidTime = time(NULL);
        
        return NULL;
    }
 
    return ctx;
}
 
void KGRedisClient::ReleaseContext(redisContext *ctx, bool active)
{
    if(ctx == NULL) return;
    if(!active) {redisFree(ctx); return;}
 
   // CAutoLock autolock(m_lock);
    m_clients.push(ctx);
}
 
bool KGRedisClient::CheckStatus(redisContext *ctx)
{
    redisReply *reply = (redisReply*)redisCommand(ctx, "ping");
    if(reply == NULL) return false;
 
    boost::shared_ptr<redisReply> autoFree(reply, freeReplyObject);
 
    if(reply->type != REDIS_REPLY_STATUS) return false;
 //   if(strcasecmp(reply->str,"PONG") != 0) return false;
 
    return true;
}