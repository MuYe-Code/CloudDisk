#include "../include/signinModule.h"
#include "../include/token.h"

#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLUtil.h>
#include <wfrest/json.hpp>

using std::string;
using Json = nlohmann::json;

void SigninModule::addModule(wfrest::HttpServer& server, Config& config)
{
    server.GET("/static/view/signin.html",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("static/view/signin.html");
    });

    server.POST("/user/signin", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        // 1 解析用户请求
        auto formKV = req->form_kv();
        string username = formKV["username"];
        string password = formKV["password"];
        string salt = "12345678";
        const char * encodedPassword = crypt(password.c_str(),salt.c_str());
        // 2 通过用户名查到密文密码，校验
        string uri = "mysql://root:123456@localhost";
        string sql = "SELECT user_pwd FROM cloudisk.tbl_user WHERE user_name = '" + username + "' LIMIT 1;";
        auto mysqlTask = WFTaskFactory::create_mysql_task(uri, 0, [username, encodedPassword, resp, uri](WFMySQLTask *mysqlTask){
            auto resp4mysql = mysqlTask->get_resp();
            protocol::MySQLResultCursor cursor(resp4mysql);
            std::vector<std::vector<protocol::MySQLCell>> rows;
            cursor.fetch_all(rows);
            std::string storeEncodedPassword = rows[0][0].as_string(); 
            fprintf(stderr, "storeEncodedPassword = %s\n encodedPassword = %s\n", storeEncodedPassword.c_str(), encodedPassword);
            if(storeEncodedPassword != encodedPassword){
                //resp->String("FAIL");
                resp->set_status_code("500");
                return;
            }
            // 3 生成token
            Token token(username,"12345678");
            Json respJs;
            Json dataJs;
            dataJs["Token"] = token.getToken();
            dataJs["Username"] = username;
            dataJs["Location"] = "/static/view/home.html";
            respJs["data"] = dataJs;
            respJs["code"] = 0;
            respJs["msg"] = "OK";
            resp->String(respJs.dump());

            //更新缓存中的token
            string sql = "UPDATE cloudisk.tbl_user_token SET user_token = '" + token.getToken() 
                + "' WHERE user_name = '" + username + "';";
            auto mysqlTask2 = WFTaskFactory::create_mysql_task(uri, 0, nullptr);
            mysqlTask2->get_req()->set_query(sql);
            series_of(mysqlTask)->push_back(mysqlTask2);
        });

        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);    
    });
}