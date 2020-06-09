#include "configclass.h"
 
CConfigOperator::CConfigOperator(void)
{
}
CConfigOperator::~CConfigOperator(void)
{
}
/************************************
 设置配置文件路径
************************************/
void CConfigOperator::SetFilePath(const string &sFilePath)
{
  m_sFilePath = sFilePath;
}
 
/************************************
 得到配置文件路径
************************************/
string CConfigOperator::GetFilePath()
{
 return this->m_sFilePath;
}
 
/************************************
  打开配置文件
************************************/
bool CConfigOperator:: OpenFile()
{
 if(true == m_fin.is_open())
 {
  m_fin.close();
 }
 m_fin.open(m_sFilePath.c_str());
 if(NULL == m_fin)
 {
  m_sError = "can not open file " +m_sFilePath;
  return false;
 }
 return true;
}
/************************************
  查找配置文件的名字
************************************/
void CConfigOperator::FindName()
{
 if(-1 != m_sStr.find('['))
 {
  string sTemp = m_sStr.substr(m_sStr.find('[') + 1,m_sStr.find(']') - m_sStr.find('[') - 1);
  if(0 == strcmp(sTemp.c_str(),m_Name.c_str()))
  {
    m_bFindName = true;
    m_sError = "Find Name But Not Find Key";
  }
  else
  {
    m_bFindName = false;
  }
 }
}
/************************************
  查找配置文件的Key
************************************/
bool  CConfigOperator::FindKey()
{
 int iDelePlace = m_sStr.find("//");
 int iFindEqual = m_sStr.find("=");
    //被注释的行，或者是包含key但是已经被注视掉了，过滤
 if( (- 1 != iDelePlace && iDelePlace <  iFindEqual) || (- 1 != iDelePlace && -1 == iFindEqual) ||  -1 == iFindEqual )
 {
  return false;
 }
 string sKey = m_sStr.substr(0,m_sStr.find('='));
 
 if(0 == strcmp(sKey.c_str(),m_Key.c_str()))
 {
  m_value = m_sStr.substr(m_sStr.find('=') + 1 , m_sStr.length() - m_sStr.find('=') - 1);
  return true;
 }
 
 return false;
}
 
/************************************
 读取配置文件NEMA KEY 对应的Value信息
************************************/
bool CConfigOperator::GetConfigValue(const string &sName,const string &skey,string &sValue,string &sError)
{
    m_sError = "";
    m_Name = sName;
    m_Key = skey;
    if(false == OpenFile())
    {
      sError = m_sError;
      return false;
    }
    char str[1024];
    m_bFindName = false;
    while(NULL != m_fin.getline(str,sizeof(str)))
    {
      m_sStr = str;
      FindName();
      if(true == m_bFindName)
      {
        if(true == FindKey())
        {
        m_fin.close();
        sError = "";
        sValue = m_value;
        return true;
        }
      }

    }
    sError = m_sError;
    m_fin.close();
    return false;
}
/************************************
  写的方式打开文件
************************************/
bool CConfigOperator::WriteFile()
{
  m_fout.close();
  //关闭后要在清空一下，否则下次打开会出错
  m_fout.clear();
  m_fout.open(m_sFilePath.c_str(),ios::in|ios::out);
  if(NULL == m_fout)
  {
    m_sError = "can not open file " +m_sFilePath;
    return false;
  }
  return true;
}
 
/************************************
 修改配置文件Key对应的值
************************************/
bool  CConfigOperator::ModefyValue()
{
  int iDelePlace = m_sStr.find("//");
  int iFindEqual = m_sStr.find("=");
    //被注释的行，或者是包含key但是已经被注视掉了，过滤
  if( (- 1 != iDelePlace && iDelePlace <  iFindEqual) || (- 1 != iDelePlace && -1 == iFindEqual) ||  -1 == iFindEqual )
  {
    return false;
  }
  string sKey = m_sStr.substr(0,m_sStr.find('='));

  if(0 == strcmp(sKey.c_str(),m_Key.c_str()))
  {
    m_sStr =  m_sStr.substr(0,m_sStr.find('=') + 1) + m_value ;
    return true;
  }

  return false;
}
/************************************
修改后的配置文件信息重新写入文件
************************************/
void CConfigOperator::WriteToFile(vector<string> &vContent)
{
  if(false == WriteFile())
  {
    m_fout.close();
    return;
  }
  for(size_t iIndex = 0; iIndex < vContent.size(); iIndex ++)
  {
    m_fout<<vContent[iIndex]<<endl;
  }
  m_fout.close();
  vector<string>().swap(vContent);
}
/************************************
 修改配置文件NEMA KEY 对应的Value信息
************************************/
bool CConfigOperator::ModefyConfigValue(const string &sName,const string &skey,const string &sValue,string &sError)
{
  m_sError = "";
  m_Name = sName;
  m_Key = skey;
  m_value = sValue;
  if(false == WriteFile())
  {
    sError = m_sError;
    return false;
  }
  char str[1024];
  m_bFindName = false;
  vector<string> vContent;
  bool isModefy = false;
  while(NULL != m_fout.getline(str,sizeof(str)))
  {
    m_sStr = str;

    FindName();
    if(true == m_bFindName)
    {
      if(true == ModefyValue())
      {
        isModefy =  true;
      }
    }

    vContent.push_back(m_sStr);
  }
  sError = m_sError;
  WriteToFile(vContent);
  m_fout.close();
  return isModefy;
}
 
vector<double> CConfigOperator::TransStrToVec(string str){
    stringstream ss(str);
    string buff;
    vector<double> arr;
 
    while (ss >> buff)
            arr.push_back(atof(buff.c_str()));
 
    return arr;
}