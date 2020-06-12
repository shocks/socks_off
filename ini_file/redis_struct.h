#ifndef __REDIS_STRUCT_H__
#define __REDIS_STRUCT_H__
#include<iostream>
#include<string>
#include<map>
#include"js_data_define.h"
using std::cout;
using std::endl;

typedef std::map<int32_t, std::string> CStringArray;
typedef std::map<std::string,std::string> CJS_STR_MAP_KEY_VALUE;

struct T_JS_Redis_Connect_Param {
	std::string strIP;
	std::string strPsd;
	int16_t nPort;
	int32_t nNumb;							//db idx

	void Init() {
		strIP.clear();
		strPsd.clear();
		nPort = 0;
		nNumb = -1;
	}

	void SetData(T_JS_Redis_Connect_Param& tIn) {
		this->strIP = tIn.strIP;
		this->strPsd = tIn.strPsd;
		this->nPort = tIn.nPort;
		this->nNumb = tIn.nNumb;
	}

	void SetData(const T_JS_Redis_Connect_Param* pIn) {
		if (pIn) {
			this->strIP = pIn->strIP;
			this->strPsd = pIn->strPsd;
			this->nPort = pIn->nPort;
			this->nNumb = pIn->nNumb;
		}
	}
	
	T_JS_Redis_Connect_Param() {
		Init();
	}
};
#endif