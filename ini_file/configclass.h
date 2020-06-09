#ifndef CONFIGCLASS_H
#define CONFIGCLASS_H
 
#include<iostream>
#include <fstream>
#include <vector>
#include<string>
#include <sstream>
#include <cstring>
using namespace std;
class CConfigOperator
{
public:
    CConfigOperator(void);
    ~CConfigOperator(void);
    //设置配置文件路径
    void SetFilePath(const string &sFilePath);
    //得到配置文件路径
    string GetFilePath();
    // 读取配置文件NEMA KEY 对应的Value信息
    bool GetConfigValue(const string &sName,const string &skey,string &sValue,string &sError);
    // 修改配置文件NEMA KEY 对应的Value信息
    bool ModefyConfigValue(const string &sName,const string &skey,const string &sValue,string &sError);
    //str to double
    vector<double> TransStrToVec(string str);
private:
    //打开配置文件
    bool OpenFile();
    //查找配置文件的名字
    void FindName();
    //查找配置文件的Key
    bool FindKey();
    //写的方式打开文件
    bool WriteFile();
    // 修改配置文件Key对应的值
    bool ModefyValue();
    //修改后的配置文件信息重新写入文件
    void WriteToFile(vector<string> &vContent);
    fstream m_fout;
    ifstream m_fin;
    string m_sFilePath;
    string m_Name;
    string m_Key;
    string m_value;
    string m_sError;
    string m_sStr;
    bool m_bFindName;
};
 
#endif // CONFIGCLASS_H