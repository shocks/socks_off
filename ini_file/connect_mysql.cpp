// #include <mutex>
// #include <atomic>
// #include <string>
// #include <memory>
// #include <mysql/mysql.h>
// #include <vector>
// #include <iostream>

// /*
//  * 数据库的链接单例
//  * */
// class DBServer {
//   public:
//     static std::shared_ptr<DBServer> getDBServer() {
//         if (DBServer::m_pDBServer != nullptr) {
//             return m_pDBServer;
//         } else {
//             std::mutex mtx;
//             std::lock_guard<std::mutex> lock(mtx);
//             if (!m_OK) {
//                 // 注意这里无法使用make_shared
//                 m_pDBServer = std::shared_ptr<DBServer>(new DBServer());
//                 m_OK = true;
//             }
//             return m_pDBServer;
//         }
//     }

// 	~DBServer() {
//         mysql_close(m_SQLCON);
//     }

//     // 注册用户信息
//     bool registerUserInfo(const std::string &username, const std::string &password, std::string &result);

//     // 登录信息验证, 成功返回true
//     bool signInQuery(const std::string &username, const std::string &password, std::string &result);

//     // 更新用户密码
//     bool updateUserPassword(const std::string &username, std::string &result);

//     // 更新作战的局数, delta正数获胜, 负数失败
//     bool updateUserCombatNum(const std::string &username, int delta, std::string &result);

//   private:
//     explicit DBServer();
//     void finishQuery() {
//         MYSQL_RES* pRes;
//         do {
//             pRes = mysql_use_result(m_SQLCON);
//             mysql_free_result(pRes);
//         } while(!mysql_next_result(m_SQLCON));

//     }

//     MYSQL *m_SQLCON;

//     static std::shared_ptr<DBServer> m_pDBServer;
//     static std::mutex m_mtx;
//     static bool m_OK;
// };

// bool DBServer::m_OK = false;
// std::shared_ptr<DBServer> DBServer::m_pDBServer = nullptr;

// void finish_with_error(MYSQL *con) {
//     fprintf(stderr, "%s\n", mysql_error(con));
//     mysql_close(con);
//     exit(1);
// }

// DBServer::DBServer() {
//     m_SQLCON = mysql_init(NULL);
//     if (m_SQLCON == NULL) {
//         std::cerr << "can not connect to mysql server\n";
//         exit(1);
//     }
//     if ((mysql_real_connect(m_SQLCON, "localhost", "root", "s*",
//                             "testdb", 0, NULL, CLIENT_MULTI_STATEMENTS)) == NULL) {
//         finish_with_error(m_SQLCON);
//     }
// }

// bool DBServer::registerUserInfo(const std::string &username, const std::string &password, std::string &result) {
//     std::string sql(
//         "INSERT INTO userinfo(username, password, wintimes, failtimes) " + std::string("VALUES (\"") + username + "\", \"" + password +
//         "\", 0, 0);");
//     if (mysql_query(m_SQLCON, sql.c_str())) {
//         result = std::string(mysql_error(m_SQLCON));
//         return false;
//     }

//     finishQuery();

//     return true;
// }

// bool DBServer::signInQuery(const std::string &username, const std::string &password, std::string &result) {
//     std::string sql("SELECT username, password FROM userinfo WHERE username = \"" + username + "\" AND password = \"" +
//                     password + "\";");
//     if (mysql_query(m_SQLCON, sql.c_str())) {
//         result.clear();
//         result += std::string(mysql_error(m_SQLCON));
//         return false;
//     }
//     auto res = mysql_store_result(m_SQLCON);
//     int num_fields = mysql_num_fields(res);
//     if (num_fields <= 0) {
//         result = std::string("Query error\n");
//         return false;
//     }

//     MYSQL_ROW row = mysql_fetch_row(res);
//     if (!row) {
//         result = std::string("username: " + username + " does not exist!");
//         return false;
//     }
//     auto username_ = std::string(row[0]);
//     auto password_ = std::string(row[1]);
//     if (username != username_) {
//         result = std::string("username: " + username + " does not exist!");
//         return false;
//     }
//     if (password_ != password) {
//         result = std::string("password error!");
//         return false;
//     }

//     finishQuery();

//     return true;
// }

// bool DBServer::updateUserPassword(const std::string &username, std::string &result) {
//     return false;
// }

// bool DBServer::updateUserCombatNum(const std::string &username, int delta, std::string& result) {
//     std::string sql("SELECT username FROM userinfo WHERE username = \"" + username + "\";");
//     if (mysql_query(m_SQLCON, sql.c_str())) {  // 确认用户确实存在
//         result.clear();
//         result += std::string(mysql_error(m_SQLCON));
//         return false;
//     }

//     finishQuery();

//     sql.clear();
//     std::string num;
//     num.clear();
//     if (delta > 0) {  // 胜利的次数
//         sql = std::string(
//                   "UPDATE userinfo SET wintimes = wintimes + " + std::to_string(delta) + " where username = \"" + username + "\"");
//     } else {  // 失败了
//         sql = std::string(
//                   "UPDATE userinfo SET failtimes = failtimes + " + std::to_string(-delta) + " where username = \"" + username + "\"");
//     }

//     if (mysql_query(m_SQLCON, sql.c_str())) {
//         result.clear();
//         result = std::string("update failed, ") + std::string(mysql_error(m_SQLCON));
//         return false;
//     }

//     finishQuery();

//     return true;
// }

// int mysql_main() {
//     auto it = DBServer::getDBServer();
//     std::string res;
//     std::cout << it->signInQuery("tiger", "123", res) << std::endl;
//     std::cout << res << std::endl;
//     res.clear();
//     std::cout << it->updateUserCombatNum("tiger", 1, res);
//     std::cout << res << std::endl;
//     res.clear();
//     std::cout << it->registerUserInfo("lion", "345", res) << std::endl;
//     std::cout << res << std::endl;
//     return 0;
// }
