#include <mutex>
#include <atomic>
#include <string>
#include <memory>
#include <mysql/mysql.h>
#include <vector>
#include <iostream>


using std::cout;
using std::endl;

/*
 * 数据库的链接单例
 * */
class DBServer {
  public:
    static std::shared_ptr<DBServer> getDBServer() {
        if (DBServer::m_pDBServer != nullptr) {
            return m_pDBServer;
        } else {
            std::mutex mtx;
            std::lock_guard<std::mutex> lock(mtx);
            if (!m_OK) {
                // 注意这里无法使用make_shared
                m_pDBServer = std::shared_ptr<DBServer>(new DBServer());
                m_OK = true;
            }
            return m_pDBServer;
        }
    }

	~DBServer() {
        mysql_close(m_SQLCON);
    }

    // 注册用户信息
    bool registerUserInfo(const std::string &username, const std::string &password, std::string &result);

    // 登录信息验证, 成功返回true
    bool signInQuery(const std::string &username, const std::string &password, std::string &result);

    // 更新用户密码
    bool updateUserPassword(const std::string &username, std::string &result);

    // 更新作战的局数, delta正数获胜, 负数失败
    bool updateUserCombatNum(const std::string &username, int delta, std::string &result);

  private:
    explicit DBServer();
    void finishQuery() {
        MYSQL_RES* pRes;
        do {
            pRes = mysql_use_result(m_SQLCON);
            mysql_free_result(pRes);
        } while(!mysql_next_result(m_SQLCON));

    }
    MYSQL *m_SQLCON;

    static std::shared_ptr<DBServer> m_pDBServer;
    static std::mutex m_mtx;
    static bool m_OK;
};

bool DBServer::m_OK = false;
std::shared_ptr<DBServer> DBServer::m_pDBServer = nullptr;

void finish_with_error(MYSQL *con) {
    std::cout<<"finish_with_error!"<<std::endl;
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

DBServer::DBServer() {
    m_SQLCON = mysql_init(NULL);
    if (m_SQLCON == NULL) {
        std::cerr << "can not connect to mysql server\n";
        exit(1);
    }
    if ((mysql_real_connect(m_SQLCON, "localhost", "root", "s",
                            "akdcenter", 0, NULL, CLIENT_MULTI_STATEMENTS)) == NULL) {
        finish_with_error(m_SQLCON);
    }
}

bool DBServer::registerUserInfo(const std::string &username, const std::string &password, std::string &result) {
    std::string sql("SELECT * FROM socks_test;");
      //  "INSERT INTO userinfo(username, password, wintimes, failtimes) " + std::string("VALUES (\"") + username + "\", \"" + password +
       // "\", 0, 0);");
    if (mysql_query(m_SQLCON, sql.c_str())) {
        result = std::string(mysql_error(m_SQLCON));
        return false;
    }

    finishQuery();

    return true;
}

bool DBServer::signInQuery(const std::string &username, const std::string &password, std::string &result) {
    std::string sql("SELECT id,user FROM socks_test;");
    
    if(mysql_query(m_SQLCON,sql.c_str())){
        cout<<"mysql_query failed!!"<<endl;
        cout<<std::string(mysql_error(m_SQLCON))<<endl;
        return false;
    }
    MYSQL_RES *res=mysql_store_result(m_SQLCON);
    MYSQL_ROW row ;
    while(row=mysql_fetch_row(res)){
        cout<<"id="<<row[0];
        cout<<"\t user="<<row[1]<<endl;
    }

    finishQuery();

    return true;
}

bool DBServer::updateUserPassword(const std::string &username, std::string &result) {
    return false;
}

bool DBServer::updateUserCombatNum(const std::string &username, int delta, std::string& result) {
    std::string sql("SELECT * FROM user_pve_stage WHERE uid = 1;");
    if (mysql_query(m_SQLCON, sql.c_str())) {  // 确认用户确实存在
        result.clear();
        result += std::string(mysql_error(m_SQLCON));
        return false;
    }

    finishQuery();

    sql.clear();
    std::string num;
    num.clear();
    if (delta > 0) {  // 胜利的次数
        sql = std::string("UPDATE user_pve_stage SET stage_num = 1 where uid=1 ;");
    } else {  // 失败了
        sql = std::string("UPDATE user_pve_stage SET stage_num = 2 where uid=1 ;");
    }

    if (mysql_query(m_SQLCON, sql.c_str())) {
        result.clear();
        result = std::string("update failed, ") + std::string(mysql_error(m_SQLCON));
        return false;
    }

    finishQuery();

    return true;
}

int mysql_main() {
    auto it = DBServer::getDBServer();
    std::cout<<"start!!"<<std::endl;
    std::string res;
    std::cout << it->signInQuery("tiger", "123", res) << std::endl;
    // std::cout << res << std::endl;
    // res.clear();
    // std::cout << it->updateUserCombatNum("tiger", 1, res);
    // std::cout << res << std::endl;
    // res.clear();
    // std::cout << it->registerUserInfo("lion", "345", res) << std::endl;
    // std::cout << res << std::endl;
    // std::cout<<"end!!"<<std::endl;
    return 0;
}
