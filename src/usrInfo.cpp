#include "../include/usrInfoModule.h"

#include <wfrest/json.hpp>
#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLUtil.h>

using std::string;
using Json = nlohmann::json;

void UsrInfoModule::addModule(wfrest::HttpServer& server, Config& config)
{
    server.GET("/static/view/home.html",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("static/view/home.html");
    });
    server.GET("/static/js/auth.js",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("static/js/auth.js");
    });
    server.GET("/static/img/avatar.jpeg",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("static/img/avatar.jpeg");
    });

    server.POST("/user/info",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        // 1 解析用户请求
        auto queryInfo = req->query_list();
        // 2 校验token
        //queryInfo["token"]
        // 3 根据username查询数据库
        string uri = "mysql://root:123456@localhost";
        string sql = "SELECT user_name,signup_at FROM cloudisk.tbl_user WHERE user_name='" + queryInfo["username"] + "' LIMIT 1;";
        fprintf(stderr, "sql = %s\n", sql.c_str());
        auto mysqlTask = WFTaskFactory::create_mysql_task(uri, 0, [resp](WFMySQLTask *mysqlTask){
            // 4 根据数据库的结果构造响应
            auto resp4mysql = mysqlTask->get_resp();
            protocol::MySQLResultCursor cursor(resp4mysql);
            std::vector<std::vector<protocol::MySQLCell>> rows;
            cursor.fetch_all(rows);
            string username = rows[0][0].as_string();
            string signupat = rows[0][1].as_datetime();

            // 构造响应
            Json respJs;
            Json dataJs;
            dataJs["Username"] = username;
            dataJs["SignupAt"] = signupat;
            respJs["code"] = 0;
            respJs["msg"] = "OK";
            respJs["data"] = dataJs;

            resp->String(respJs.dump());
        });

        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);
    });

    server.POST("/file/query",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        // 1 解析用户请求 得到用户名和token
        // uri的query
        auto queryInfo = req->query_list();
        std::string username = queryInfo["username"];
        //std::string token = queryInfo["token"];
        // POST 报文体
        auto postForm = req->form_kv();
        std::string limit = postForm["limit"];
        // 2 校验token
        std::string token = queryInfo["token"];
        std::string uri = "mysql://root:123456@localhost";
        std::string sql = "SELECT user_token FROM cloudisk.tbl_user_token WHERE user_name = '" + username + "';";
        auto mysqlTask = WFTaskFactory::create_mysql_task(uri, 0, [resp, token, username, limit, uri](WFMySQLTask *mysqlTask){
            auto resp4mysql = mysqlTask->get_resp();
            protocol::MySQLResultCursor cursor(resp4mysql);
            std::vector<std::vector<protocol::MySQLCell>> rows;
            cursor.fetch_all(rows);

            // 3 token校验成功，根据用户名去查询数据库，得到单个用户的文件列表
            if(rows[0][0].as_string() == token)
            {
                std::string sql = "SELECT file_sha1,file_name,file_size,upload_at,last_update FROM cloudisk.tbl_user_file WHERE ";
                sql += "user_name='" + username + "'LIMIT " + limit + ";";
                fprintf(stderr,"sql = %s\n", sql.c_str());
                auto mysqlTask2 = WFTaskFactory::create_mysql_task(uri, 0, [resp](WFMySQLTask *mysqlTask2){
                    auto resp4mysql = mysqlTask2->get_resp();
                    protocol::MySQLResultCursor cursor(resp4mysql);
                    std::vector<std::vector<protocol::MySQLCell>> rows;
                    cursor.fetch_all(rows);

                    Json respJs;
                    for(auto &row:rows){
                        Json itemJs;
                        itemJs["FileHash"] = row[0].as_string();
                        itemJs["FileName"] = row[1].as_string();
                        //itemJs["FileSize"] = std::to_string(row[2].as_int());
                        itemJs["FileSize"] = row[2].as_ulonglong();
                        itemJs["UploadAt"] = row[3].as_datetime();
                        itemJs["LastUpdated"] = row[4].as_datetime();
                        respJs.push_back(itemJs);
                    }
                    resp->String(respJs.dump());
                });
                mysqlTask2->get_req()->set_query(sql);
                series_of(mysqlTask)->push_back(mysqlTask2);
            }
            
            // 4 token校验失败
            else
            {
                resp->String("invalid token");
            }
        });

        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);
    });
}