#include<sstream>
#include"CJSRedis.h"
using namespace std;
CJSRedis::CJSRedis(){
    _connect=nullptr;
    _reply=nullptr;
}
CJSRedis::~CJSRedis(){

}

//设置redis连接参数
JS_VOID CJSRedis::SetConnectParam(const T_JS_Redis_Connect_Param* pIn){
    m_tConnectParam.SetData(pIn);
}
//连接redis
JS_INT32 CJSRedis::Connect(){
    redisFree(_connect);
    _connect=redisConnect(m_tConnectParam.strIP.c_str(),m_tConnectParam.nPort);
    if(_connect==nullptr || _connect->err !=0)
    {
        std::cout<<"redisConnectWithTimeout failed!!"<<std::endl;
        if(_connect != NULL) 
            redisFree(_connect);       
        return 0;
    }
    std::cout<<"redisConnect sussecc!\n"<<std::endl;
    if(!m_tConnectParam.strPsd.empty()){
        freeReplyObject(_reply);
        _reply=(redisReply*) redisCommand(_connect, "AUTH %s", m_tConnectParam.strPsd.c_str()); 
        if(_reply==nullptr){
            std::cout<<"redisCommand AUTH Failed!!"<<std::endl;
            freeReplyObject(_reply);
            return false;     
        }
        if( !(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str,"OK")==0))  
        { 
            std::cout << " redis auth failed!!!!" << std::endl;
            freeReplyObject(_reply);
            _reply = NULL;
            return false;
        }
    }

    char initCmd[100];
    memset(initCmd,0,sizeof(initCmd));
    sprintf(initCmd,"select %d",m_tConnectParam.nNumb);
    _reply=(redisReply*)redisCommand(_connect,initCmd);
    if(_reply==nullptr){
        std::cout<<"redisCommand select Failed!!"<<std::endl;
        return false;
    }
    if((_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str,"OK")==0))
    {
        freeReplyObject(_reply);
        return true;
    }
    freeReplyObject(_reply);
    return false;
}
//设置key超时
JS_BOOL CJSRedis::Expireat(std::string strKey, JS_TIME_T llOutDayUTC){
    if(JS_FALSE==IsConnectValid())
    {
        return false;
    }
    freeReplyObject(_reply);
    _reply=(redisReply*)redisCommand(_connect,"PEXPIREAT %s %d",strKey.c_str(),llOutDayUTC);
    if(_reply==nullptr){
        std::cout<<"redisCommand Get Failed!!"<<std::endl;
        return false;
    }
    if(_reply->type != REDIS_REPLY_ERROR && _reply->type >0 && _reply->type<6)
    {
        std::cout<<"reply->type="<<_reply->type<<std::endl;
        return _reply->str;
    }
}

//删除当前数据库的所有信息（根据库号）
JS_VOID CJSRedis::TruncateCurrDB(){
    if(JS_FALSE==IsConnectValid())
    {
        return;
    }
    redisReply* reply=(redisReply*)redisCommand(_connect,"flushdb");
    if(reply==nullptr){
        std::cout<<"redisCommand Get Failed!!"<<std::endl;
        return;
    }
    if(!(reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str,"OK")==0))
    {
        std::cout<<"reply->type="<<reply->type<<std::endl;
        return;
    }
}

//检测当前连接是否生效
JS_BOOL CJSRedis::IsConnectValid(){
    _reply = (redisReply*)redisCommand(_connect, "ping");
    if(_reply == NULL){ 
        Connect();
        std::cout<<"redisCommand ping Failed!!"<<std::endl;
        return false;
    }
    boost::shared_ptr<redisReply> autoFree(_reply, freeReplyObject);
 
    if(_reply->type != REDIS_REPLY_STATUS) {
        std::cout<<"redisCommand Failed! reply->type != REDIS_REPLY_STATUS ."<<std::endl;
        freeReplyObject(_reply);
        Connect();
        return false;
    }
    //std::cout<<"IsConnectValid!!"<<std::endl;
    return true;
}

//检测key是否存在
JS_BOOL CJSRedis::Exists(std::string strKey){
    if(JS_FALSE==IsConnectValid())
    {
        return false;
    }
    _reply=(redisReply*)redisCommand(_connect,"exists %s",strKey.c_str());
    if(_reply==nullptr){
        std::cout<<"redisCommand Exists Failed!!"<<std::endl;
        freeReplyObject(_reply);
        _reply = NULL;
        return false;
    }
    cout<<strKey<<" _reply->elements="<<_reply->elements<<endl;
    cout<<"_reply->type ="<<_reply->type <<endl;
    if(_reply->type != REDIS_REPLY_INTEGER && _reply->integer!=1)
    {
        cout<<strKey<<"non-Existent"<<endl;
        freeReplyObject(_reply);
        _reply = NULL;
        return false;
    }

    freeReplyObject(_reply);
    _reply = NULL;
    return true;
}

JS_VOID CJSRedis::Set(std::string strKey, std::string strValue){
    if(JS_FALSE==IsConnectValid())
    {
        return;
    }
    _reply=(redisReply*)redisCommand(_connect,"set %s %s",strKey.c_str(),strValue.c_str());
    if(_reply==nullptr){
        std::cout<<"redisCommand Set Failed!!"<<std::endl;
        return;
    }
    //执行失败
    if( !(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str,"OK")==0))  
    {
        std::cout << "set redis faliled" << std::endl;
        freeReplyObject(_reply);
        _reply = NULL;
        return;    
    }
    
    std::cout << "set redis success"<<std::endl;
    freeReplyObject(_reply);
    _reply = NULL;
}

//string oper
std::string CJSRedis::Get(std::string strKey){
    if(JS_FALSE==IsConnectValid())
    {
        return "";
    }
    if(!Exists(strKey))
    {   
        return "";
    }
    _reply=(redisReply*)redisCommand(_connect,"get %s",strKey.c_str());
    if(_reply==nullptr){
        std::cout<<"redisCommand Get Failed!!"<<std::endl;
        return "";
    }
     //get成功返回结果为 REDIS_REPLY_STRING 
    if( _reply->type != REDIS_REPLY_STRING && _reply->type != REDIS_REPLY_INTEGER)  
    {
        std::cout << "get redis faliled" << std::endl;
        std::cout<<"_reply->type="<<_reply->type<<std::endl;
        freeReplyObject(_reply);
        _reply = NULL;
        return "";    
    }
    std::cout << "get "<<strKey<<" redis success!"<<std::endl;
    std::string valuestr = _reply->str;
    freeReplyObject(_reply);
    _reply = NULL;
    
    return valuestr ;
}

//hash oper
JS_BOOL CJSRedis::H_Set(std::string strKey, std::string strField, std::string strValue){
    if(JS_FALSE==IsConnectValid())
    {
        return false;
    }
    _reply=(redisReply*)redisCommand(_connect,"hset %s %s %s",strKey.c_str(),strField.c_str(),strValue.c_str());
    if(_reply==nullptr){
        std::cout<<"redisCommand Set Failed!!"<<std::endl;
        return false;
    }
    //执行失败
    if( _reply->type != REDIS_REPLY_INTEGER && (_reply->integer!=1 || _reply->integer!=0))  
    {
        std::cout << "hset redis faliled" << std::endl;
        freeReplyObject(_reply);
        _reply = NULL;
        return false;    
    }
    
    std::cout << "hset redis success"<<std::endl;
    freeReplyObject(_reply);
    _reply = NULL;
    return true;
}
JS_BOOL CJSRedis::H_Get(std::string strKey, std::string strField, std::string& strOut){
    if(JS_FALSE==IsConnectValid())
    {
        return false;
    }
    _reply=(redisReply*)redisCommand(_connect,"HGet %s %s",strKey.c_str(),strField.c_str());
    if(_reply==nullptr){
        std::cout<<"redisCommand HGet Failed!!"<<std::endl;
        return false;
    }
     //get成功返回结果为 REDIS_REPLY_STRING 
    if( _reply->type != REDIS_REPLY_STRING && _reply->type != REDIS_REPLY_INTEGER)  
    {
        std::cout << "HGet redis faliled" << std::endl;
        std::cout<<"_reply->type="<<_reply->type<<std::endl;
        freeReplyObject(_reply);
        _reply = NULL;
        return false;    
    }
    std::cout << "HGet "<<strKey<<" redis success!"<<std::endl;
    strOut = _reply->str;
    freeReplyObject(_reply);
    _reply = NULL;
    
    return JS_TRUE ;
}
JS_BOOL CJSRedis::H_MSet(std::string strKey, CJS_STR_MAP_KEY_VALUE& mapIn){
    if(mapIn.size()==0)
        return false;
    if(JS_FALSE==IsConnectValid())
    {
        return false;
    }
    ostringstream Ostr;
    Ostr<<"HMSET "<<strKey;
    CJS_STR_MAP_KEY_VALUE::const_iterator itBg=mapIn.begin();
    CJS_STR_MAP_KEY_VALUE::const_iterator itEd=mapIn.end();
    for(;itBg!=itEd;++itBg){
        Ostr<<" ";
        Ostr<<itBg->first<<" "<<itBg->second;
    }
    _reply=(redisReply*)redisCommand(_connect,Ostr.str().c_str());
    if(_reply==nullptr){
        std::cout<<"redisCommand H_MSet Failed!!"<<std::endl;
        return false;
    }
    //执行失败
    if(!(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str,"OK")==0))  
    {
        std::cout << "H_MSet redis faliled" << std::endl;
        freeReplyObject(_reply);
        _reply = NULL;
        return false;    
    }
    
    std::cout << "H_MSet redis success"<<std::endl;
    freeReplyObject(_reply);
    _reply = NULL;
    
    return true;
}
JS_BOOL CJSRedis::H_MGet(std::string strKey, CJS_STR_MAP_KEY_VALUE& mapOut){
    if(mapOut.size()==0)
        return false;
    if(JS_FALSE==IsConnectValid())
    {
        return false;
    }
    ostringstream Ostr;
    Ostr<<"HMGET "<<strKey;
    CJS_STR_MAP_KEY_VALUE::iterator itBg=mapOut.begin();
    CJS_STR_MAP_KEY_VALUE::iterator itEd=mapOut.end();
    for(;itBg!=itEd;++itBg){
        cout<<itBg->first<<endl;
        Ostr<<" ";
        Ostr<<itBg->first;
    }
    _reply=(redisReply*)redisCommand(_connect,Ostr.str().c_str());
    if(_reply==nullptr){
        std::cout<<"redisCommand H_MGet Failed!!"<<std::endl;
        return false;
    }
    //执行失败
    if(_reply->type != REDIS_REPLY_ARRAY )  
    {
        std::cout << "H_MGet redis faliled" << std::endl;
        freeReplyObject(_reply);
        _reply = NULL;
        return false;    
    }
    if(_reply->elements==0){
        std::cout << "H_MGet redis faliled elements=0" << std::endl;
        return false;
    }
    bool hasEmpty=false;
    itBg=mapOut.begin();
    for(int nPos=0;nPos<_reply->elements;++nPos){
        redisReply *result=_reply->element[nPos];
        if(result->type==REDIS_REPLY_NIL)
        {
            hasEmpty=true;
            ++itBg;
            continue;
        }
        if(itBg!=itEd){
            string s(result->str,result->str+strlen(result->str));
            itBg->second=s;
            ++itBg;
        }
    }

    std::cout << "H_MGet redis success"<<std::endl;
    freeReplyObject(_reply);
    _reply = NULL;
    if(hasEmpty==true)
    {
        return false;
    }
    return true;
}
JS_BOOL CJSRedis::H_GetAll(std::string strKey, CJS_STR_MAP_KEY_VALUE& mapOut){
    if(JS_FALSE==IsConnectValid())
    {
        return false;
    }
    ostringstream Ostr;
    Ostr<<"HGETALL "<<strKey;
    cout<<"str="<<Ostr.str()<<endl;
    _reply=(redisReply*)redisCommand(_connect,Ostr.str().c_str());
    if(_reply==nullptr){
        std::cout<<"redisCommand H_GetAll Failed!!"<<std::endl;
        return false;
    }
    //执行失败
    if(_reply->type != REDIS_REPLY_ARRAY )  
    {
        std::cout << "H_GetAll redis faliled" << std::endl;
        freeReplyObject(_reply);
        _reply = NULL;
        return false;    
    }
    if(_reply->elements==0){
        std::cout << "H_GetAll redis faliled elements=0" << std::endl;
        return false;
    }

    for(int nPos=0;nPos<_reply->elements;++nPos){
        redisReply *result=_reply->element[nPos];
        if(result->type==REDIS_REPLY_NIL)
        {
            cout<<"H_GetAll REDIS_REPLY_NIL!"<<endl;
            return false;
        }
        string firstValue(result->str,result->str+strlen(result->str));
        freeReplyObject(result);
        result = NULL;
        result=_reply->element[++nPos];
        if(result->type==REDIS_REPLY_NIL)
        {
            cout<<"H_GetAll REDIS_REPLY_NIL!"<<endl;
            return false;
        }
        string secondValue(result->str,result->str+strlen(result->str));
        if(mapOut.find(firstValue)==mapOut.end())
        {
            mapOut.insert(CJS_STR_MAP_KEY_VALUE::value_type(firstValue,secondValue));
        }
        
    }

    std::cout << "H_GetAll redis success"<<std::endl;
    freeReplyObject(_reply);
    _reply = NULL;

    return true;
}
JS_BOOL CJSRedis::H_Exists(std::string strKey, std::string strField, JS_BOOL& bExist){
    if(JS_FALSE==IsConnectValid())
    {
        return false;
    }
    _reply=(redisReply*)redisCommand(_connect,"HEXISTS %s %s",strKey.c_str(),strField.c_str());
    if(_reply==nullptr){
        std::cout<<"redisCommand H_Exists Failed!!"<<std::endl;
        return false;
    }
    
    if( _reply->type != REDIS_REPLY_INTEGER || _reply->integer != 1)  
    {
        std::cout << "H_Exists redis faliled" << std::endl;
        freeReplyObject(_reply);
        _reply = NULL;
        bExist=false;
        return false;    
    }
    std::cout << "H_Exists "<<strKey<<" redis success!"<<std::endl;
    bExist=true;
    return true ;

}

//list oCJSRedis::per
JS_BOOL CJSRedis::L_Push(std::string strKey, std::string strValue){
    return true;
}
JS_BOOL CJSRedis::L_Index(std::string strKey, JS_INT32 nIdx, std::string& strOut){
    return true;
}
JS_BOOL CJSRedis::L_Range(std::string strKey, JS_INT32 nStart, JS_INT32 nStop, CStringArray& straOut){
    return true;
}
JS_BOOL CJSRedis::L_Len(std::string strKey, JS_INT32& nOut){
    return true;
}