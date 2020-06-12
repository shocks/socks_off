/*
 * CJSRedis.h
 *
 *  Created on: Apr 11, 2017
 *      Author: root
 */

#ifndef CJSREDIS_H_
#define CJSREDIS_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <queue>
#include <hiredis/hiredis.h>
#include <boost/shared_ptr.hpp>
#include "js_data_define.h"
#include "redis_struct.h"

class CJSRedis {
public:
	CJSRedis();
	virtual ~CJSRedis();

	//设置redis连接参数
	JS_VOID SetConnectParam(const T_JS_Redis_Connect_Param* pIn);
    JS_INT32 Connect();

    //设置key超时
    JS_BOOL Expireat(std::string strKey, JS_TIME_T llOutDayUTC);

    //删除当前数据库的所有信息（根据库号）
    JS_VOID TruncateCurrDB();

    //检测当前连接是否生效
    JS_BOOL IsConnectValid();

    //检测key是否存在
    JS_BOOL Exists(std::string strKey);

    //string oper
    std::string Get(std::string strKey);
    JS_VOID Set(std::string strKey, std::string strValue);

    //hash oper
    JS_BOOL H_Exists(std::string strKey, std::string strField, JS_BOOL& bExist);
    JS_BOOL H_Set(std::string strKey, std::string strField, std::string strValue);
    JS_BOOL H_Get(std::string strKey, std::string strField, std::string& strOut);
    JS_BOOL H_MSet(std::string strKey, CJS_STR_MAP_KEY_VALUE& mapIn);
    JS_BOOL H_MGet(std::string strKey, CJS_STR_MAP_KEY_VALUE& mapIn);
    JS_BOOL H_GetAll(std::string strKey, CJS_STR_MAP_KEY_VALUE& mapOut);

    //list oper
    JS_BOOL L_Push(std::string strKey, std::string strValue);
    JS_BOOL L_Index(std::string strKey, JS_INT32 nIdx, std::string& strOut);
    JS_BOOL L_Range(std::string strKey, JS_INT32 nStart, JS_INT32 nStop, CStringArray& straOut);
    JS_BOOL L_Len(std::string strKey, JS_INT32& nOut);


private:

    redisContext* _connect;
    redisReply* _reply;
    T_JS_Redis_Connect_Param m_tConnectParam;
};

#endif /* CJSREDIS_H_ */
