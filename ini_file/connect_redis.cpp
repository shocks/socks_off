#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <hiredis/hiredis.h>
#include "connect_redis_3.h"
#include "CJSRedis.h"

void doTest()
{
    int timeout = 10000;
    struct timeval timeoutt = {10, 1000000}; //超时时间
    struct timeval tv;
    tv.tv_sec = timeout / 1000;         //秒数
    tv.tv_usec = timeout * 1000;        //微秒数
    //以带有超时的方式链接Redis服务器，同时获取与Redis连接的上下文对象。
    //该对象将用于其后所有与Redis操作的函数。
    redisContext* c = redisConnectWithTimeout("127.0.0.1",6379,timeoutt);
    //redisContext* c = redisConnect("127.0.0.1",6379);
    if (c->err) {
        std::cout<<"connect failed!!"<<std::endl;
        redisFree(c);
        return;
    }
    const char* command1 = "set stest1 value1";
    redisReply* r = (redisReply*)redisCommand(c,command1);
    //需要注意的是，如果返回的对象是NULL，则表示客户端和服务器之间出现严重错误，必须重新链接。
    //这里只是举例说明，简便起见，后面的命令就不再做这样的判断了。
    if (NULL == r) {
        redisFree(c);
        return;
    }
    //不同的Redis命令返回的数据类型不同，在获取之前需要先判断它的实际类型。
    //至于各种命令的返回值信息，可以参考Redis的官方文档，或者查看该系列博客的前几篇
    //有关Redis各种数据类型的博客。:)
    //字符串类型的set命令的返回值的类型是REDIS_REPLY_STATUS，然后只有当返回信息是"OK"
    //时，才表示该命令执行成功。后面的例子以此类推，就不再过多赘述了。
    if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str,"OK") == 0)) {
        printf("Failed to execute command[%s].\n",command1);
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    //由于后面重复使用该变量，所以需要提前释放，否则内存泄漏。
    freeReplyObject(r);
    printf("Succeed to execute command[%s].\n",command1);

    const char* command2 = "strlen stest1";
    r = (redisReply*)redisCommand(c,command2);
    if (r->type != REDIS_REPLY_INTEGER) {
        printf("Failed to execute command[%s].\n",command2);
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    int length = r->integer;
    freeReplyObject(r);
    printf("The length of 'stest1' is %d.\n",length);
    printf("Succeed to execute command[%s].\n",command2);

    const char* command3 = "get stest1";
    r = (redisReply*)redisCommand(c,command3);
    if (r->type != REDIS_REPLY_STRING) {
        printf("Failed to execute command[%s].\n",command3);
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    printf("The value of 'stest1' is %s.\n",r->str);
    freeReplyObject(r);
    printf("Succeed to execute command[%s].\n",command3);

    const char* command4 = "get stest2";
    r = (redisReply*)redisCommand(c,command4);
    //这里需要先说明一下，由于stest2键并不存在，因此Redis会返回空结果，这里只是为了演示。
    if (r->type != REDIS_REPLY_NIL) {
        printf("Failed to execute command[%s].\n",command4);
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    freeReplyObject(r);
    printf("Succeed to execute command[%s].\n",command4);

    const char* command5 = "mget stest1 stest2";
    r = (redisReply*)redisCommand(c,command5);
    //不论stest2存在与否，Redis都会给出结果，只是第二个值为nil。
    //由于有多个值返回，因为返回应答的类型是数组类型。
    if (r->type != REDIS_REPLY_ARRAY) {
        printf("Failed to execute command[%s].\n",command5);
        freeReplyObject(r);
        redisFree(c);
        //r->elements表示子元素的数量，不管请求的key是否存在，该值都等于请求是键的数量。
        assert(2 == r->elements);
        return;
    }
    for (int i = 0; i < r->elements; ++i) {
        redisReply* childReply = r->element[i];
        //之前已经介绍过，get命令返回的数据类型是string。
        //对于不存在key的返回值，其类型为REDIS_REPLY_NIL。
        if (childReply->type == REDIS_REPLY_STRING)
            printf("The value is %s.\n",childReply->str);
    }
    //对于每一个子应答，无需使用者单独释放，只需释放最外部的redisReply即可。
    freeReplyObject(r);
    printf("Succeed to execute command[%s].\n",command5);

    printf("Begin to test pipeline.\n");
    //该命令只是将待发送的命令写入到上下文对象的输出缓冲区中，直到调用后面的
    //redisGetReply命令才会批量将缓冲区中的命令写出到Redis服务器。这样可以
    //有效的减少客户端与服务器之间的同步等候时间，以及网络IO引起的延迟。
    //至于管线的具体性能优势，可以考虑该系列博客中的管线主题。
    if (REDIS_OK != redisAppendCommand(c,command1)
        || REDIS_OK != redisAppendCommand(c,command2)
        || REDIS_OK != redisAppendCommand(c,command3)
        || REDIS_OK != redisAppendCommand(c,command4)
        || REDIS_OK != redisAppendCommand(c,command5)) {
        redisFree(c);
        return;
    }

    redisReply* reply = NULL;
    //对pipeline返回结果的处理方式，和前面代码的处理方式完全一直，这里就不再重复给出了。
    if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
        printf("Failed to execute command[%s] with Pipeline.\n",command1);
        freeReplyObject(reply);
        redisFree(c);
    }
    freeReplyObject(reply);
    printf("Succeed to execute command[%s] with Pipeline.\n",command1);

    if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
        printf("Failed to execute command[%s] with Pipeline.\n",command2);
        freeReplyObject(reply);
        redisFree(c);
    }
    freeReplyObject(reply);
    printf("Succeed to execute command[%s] with Pipeline.\n",command2);

    if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
        printf("Failed to execute command[%s] with Pipeline.\n",command3);
        freeReplyObject(reply);
        redisFree(c);
    }
    freeReplyObject(reply);
    printf("Succeed to execute command[%s] with Pipeline.\n",command3);

    if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
        printf("Failed to execute command[%s] with Pipeline.\n",command4);
        freeReplyObject(reply);
        redisFree(c);
    }
    freeReplyObject(reply);
    printf("Succeed to execute command[%s] with Pipeline.\n",command4);

    if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
        printf("Failed to execute command[%s] with Pipeline.\n",command5);
        freeReplyObject(reply);
        redisFree(c);
    }
    freeReplyObject(reply);
    printf("Succeed to execute command[%s] with Pipeline.\n",command5);
    //由于所有通过pipeline提交的命令结果均已为返回，如果此时继续调用redisGetReply，
    //将会导致该函数阻塞并挂起当前线程，直到有新的通过管线提交的命令结果返回。
    //最后不要忘记在退出前释放当前连接的上下文对象。
    redisFree(c);
    return;
}

int main() 
{
  //  doTest();
    string ip="127.0.0.1" ;
    int port=6379;
    int timeout = 1000;
    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 100;
   // KGRedisClient myredis(ip,port,timeout);
    string cmd1="select";
    string cmd2="5";
    string key1="t1";
    string key2="t2";
    string valu1="b1";
    string valu2="b2";     
    string respond;
    respond.clear();
    CJSRedis myredis;
    T_JS_Redis_Connect_Param param;
    param.strIP=ip;
    param.nPort=port;
    param.strPsd.clear();
    param.nNumb=6;
    myredis.SetConnectParam(&param);
    myredis.Connect();
    std::string strKey="key";
    std::string strField1="strField1";
    std::string strValue1="strValue1";
    std::string strField2="strField2";
    std::string strValue2="strValue2";
    std::string strField3="strField3";
    std::string strValue3="strValue3";
    //std::string strOut;
    CJS_STR_MAP_KEY_VALUE mapIn;
    mapIn.insert(CJS_STR_MAP_KEY_VALUE::value_type(strField1,strValue1));
    mapIn.insert(CJS_STR_MAP_KEY_VALUE::value_type(strField2,strValue2));
    mapIn.insert(CJS_STR_MAP_KEY_VALUE::value_type(strField3,strValue3));
   // myredis.H_MSet(strKey,mapIn);
    JS_BOOL bExist;
    if(myredis.H_Exists(strKey,"sss",bExist)==true)
        cout<<"bExist="<<bExist<<endl;
  //  CJS_STR_MAP_KEY_VALUE mapOut;
  //  mapOut.insert(CJS_STR_MAP_KEY_VALUE::value_type(strField1,""));
 //   mapOut.insert(CJS_STR_MAP_KEY_VALUE::value_type(strField2,""));
  //  mapOut.insert(CJS_STR_MAP_KEY_VALUE::value_type(strField3,""));
  //  mapOut.insert(CJS_STR_MAP_KEY_VALUE::value_type("ss",""));

    // if(myredis.H_GetAll("key",mapOut)==false)
    //     cout<<"H_MGet failed!!"<<endl;
    // cout<<"mapOut.size="<<mapOut.size()<<endl;
    // CJS_STR_MAP_KEY_VALUE::const_iterator itBg=mapOut.begin();
    // CJS_STR_MAP_KEY_VALUE::const_iterator itEd=mapOut.end();
    // cout<<endl;
    // for(;itBg!=itEd;++itBg)
    // {
    //     cout<<"it->first="<<itBg->first<<"\tit->second="<<itBg->second<<endl;
    // }
    // myredis.H_Set(strKey,strField1,strValue1);
    // strOut.clear();
    // myredis.H_Get(strKey,strField1,strOut);
    // cout<<"strout="<<strOut<<endl;
    cout<<endl;
    // redisReply* reply=myredis.ExecuteCmd(cmd.c_str(),"%b"cmd.size());

    // cout<<"type="<<reply->type<<endl;
    // cout<<"reply->str="<<reply->str<<endl;
    // freeReplyObject(reply);

    // cmd="set tt bb";
    // reply=myredis.ExecuteCmd(cmd.c_str(),cmd.size());
    // cout<<"type="<<reply->type<<endl;
    // cout<<"reply->str="<<reply->str<<endl;

    // cmd="get tt";
    // reply=myredis.ExecuteCmd(cmd.c_str(),cmd.size());
    // cout<<"type="<<reply->type<<endl;
    // cout<<"reply->str="<<reply->str<<endl;


    // redisContext* c =redisConnectWithTimeout(ip.c_str(),port,tv);


    // //redisReply* r = (redisReply*)redisCommand(c,"%b",cmd.c_str(),cmd.length());
    // redisReply* r = (redisReply*)redisCommand(c,"select %s",cmd2.c_str());
    // cout<<"type="<<r->type<<endl;
    // cout<<"r->str="<<r->str<<endl;
    // freeReplyObject(r);

    // r = (redisReply*)redisCommand(c,"set %s %s",cmd3.c_str(),cmd4.c_str());
    // cout<<"type="<<r->type<<endl;
    // cout<<"r->str="<<r->str<<endl;
    // freeReplyObject(r);

    // r = (redisReply*)redisCommand(c,"get %s",cmd3.c_str());
    // cout<<"type="<<r->type<<endl;
    // cout<<"r->str="<<r->str<<endl;
    // freeReplyObject(r);

    // r = (redisReply*)redisCommand(c,"mget %s %s",key1.c_str(),key2.c_str());
    // if(REDIS_REPLY_ARRAY==r->type)
    // {
    //     for (int i = 0; i < r->elements; ++i) {
    //         redisReply* childReply = r->element[i];
    //         //之前已经介绍过，get命令返回的数据类型是string。
    //         //对于不存在key的返回值，其类型为REDIS_REPLY_NIL。
    //         if (childReply->type == REDIS_REPLY_STRING)
    //             printf("The value is %s.\n",childReply->str);
    // }
    // }
    // freeReplyObject(r);



    return 0;
}