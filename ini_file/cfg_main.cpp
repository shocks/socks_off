#include"configclass.h"
int main_1()
{
    /**打开配置文件**/
    CConfigOperator tCConfigOperator;
    string sFielPath = "cfg.ini";
    tCConfigOperator.SetFilePath(sFielPath);
    string sName = "";
    string sKey = "";
    string sValue = "";
    string sEroor = "";
    //tCConfigOperator.GetConfigValue(sName,sKey,sValue,sEroor);
    //cout<<"valuse is"<<sValue<<" sEroor is"<<sEroor<<endl;
 
    /**获取机械臂的ip和控制端口**/
    sName = "config";
    sKey = "robot_host";
    tCConfigOperator.GetConfigValue(sName,sKey,sValue,sEroor);
    cout<<"sValue="<<sValue<<endl;
    const char* SERVER_HOST = sValue.c_str();
    sKey = "robot_port";
    tCConfigOperator.GetConfigValue(sName,sKey,sValue,sEroor);
    cout<<"sValue="<<sValue<<endl;
    int SERVER_PORT = atoi(sValue.c_str());
 
    /**获取最大加速度和速度值**/
    sKey = "maxvel";
    tCConfigOperator.GetConfigValue(sName,sKey,sValue,sEroor);
    cout<<"sValue="<<sValue<<endl;
    double maxvel = atof(sValue.c_str());
    sKey = "maxacc";
    tCConfigOperator.GetConfigValue(sName,sKey,sValue,sEroor);
    cout<<"sValue="<<sValue<<endl;
    double maxacc = atof(sValue.c_str());
    sKey = "maxdq";
    tCConfigOperator.GetConfigValue(sName,sKey,sValue,sEroor);
    cout<<"sValue="<<sValue<<endl;
    double maxdq = atof(sValue.c_str());
    sKey = "maxdqq";
    tCConfigOperator.GetConfigValue(sName,sKey,sValue,sEroor);
    cout<<"sValue="<<sValue<<endl;
    double maxdqq = atof(sValue.c_str());
 
    /**获取路点变量**/
    sName = "waypoint";
    sKey = "lefthome";
    tCConfigOperator.GetConfigValue(sName,sKey,sValue,sEroor);
    vector<double> point = tCConfigOperator.TransStrToVec(sValue);
    //Util::initJointAngleArray(lefthomepoint, point[0]/180.0*M_PI, point[1]/180.0*M_PI, point[2]/180.0*M_PI,
     //       point[3]/180.0*M_PI, point[4]/180.0*M_PI, point[5]/180.0*M_PI);
 
    sKey = "righthome";
    tCConfigOperator.GetConfigValue(sName,sKey,sValue,sEroor);
    point = tCConfigOperator.TransStrToVec(sValue);
    //Util::initJointAngleArray(righthomepoint, point[0]/180.0*M_PI, point[1]/180.0*M_PI, point[2]/180.0*M_PI,
      //      point[3]/180.0*M_PI, point[4]/180.0*M_PI, point[5]/180.0*M_PI);
 
    sKey = "box1quleft";
    tCConfigOperator.GetConfigValue(sName,sKey,sValue,sEroor);
    point = tCConfigOperator.TransStrToVec(sValue);
    //Util::initJointAngleArray(box1quleftpoint, point[0]/180.0*M_PI, point[1]/180.0*M_PI, point[2]/180.0*M_PI,
      //      point[3]/180.0*M_PI, point[4]/180.0*M_PI, point[5]/180.0*M_PI);
 
    sKey = "box8quright";
    tCConfigOperator.GetConfigValue(sName,sKey,sValue,sEroor);
    point = tCConfigOperator.TransStrToVec(sValue);
    //Util::initJointAngleArray(box8qurightpoint, point[0]/180.0*M_PI, point[1]/180.0*M_PI, point[2]/180.0*M_PI,
      //      point[3]/180.0*M_PI, point[4]/180.0*M_PI, point[5]/180.0*M_PI);
    return 0;
}