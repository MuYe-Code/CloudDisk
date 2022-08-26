#include "../include/signupModule.h"

#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLUtil.h>

void SignupModule::addModule(wfrest::HttpServer& server, Config& config)
{
    //显示注册页面
    server.GET("/user/signup",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("static/view/signup.html");
    });

    server.POST("/user/signup",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        // 1 解析用户的请求 urlencoded
        auto formKV = req->form_kv();
        std::string username = formKV["username"];
        std::string password = formKV["password"];

        // 2 将用户的密码加密
        std::string salt = "12345678";
        const char * encodedPassword = crypt(password.c_str(),salt.c_str());

        // 3 插入tbl_user表中
        std::string uri = "mysql://root:123456@localhost";
        std::string sql = "INSERT INTO cloudisk.tbl_user (user_name,user_pwd) VALUES ('"+ 
            username + "', '" + encodedPassword + "');";

        auto mysqlTask = WFTaskFactory::create_mysql_task(uri, 0, [resp, username, uri](WFMySQLTask *mysqlTask){
            if(mysqlTask->get_state() != WFT_STATE_SUCCESS){
                fprintf(stderr,"error msg: %s\n", WFGlobal::get_error_string(mysqlTask->get_state(), mysqlTask->get_error()));
                //resp->set_status_code("500");
                resp->String("FAIL");
                return;
            }
            auto resp4sql = mysqlTask->get_resp();
            if(resp4sql->get_packet_type() == MYSQL_PACKET_ERROR){
                fprintf(stderr,"ERROR, error_code = %d, error_msg = %s\n",
                    resp4sql->get_error_code(),
                    resp4sql->get_error_msg().c_str()
                    );
                //resp->set_status_code("500");
                resp->String("FAIL");
                return;
            }  

            protocol::MySQLResultCursor cursor(resp4sql);
            if(cursor.get_affected_rows() == 1){
                //成功
                resp->String("SUCCESS");
                auto mysqlTask2 = WFTaskFactory::create_mysql_task(uri, 0, nullptr);
                std::string sql = "INSERT INTO cloudisk.tbl_user_token (user_name, user_token) VALUES ('" + username + "', '-');";
                fprintf(stderr, "sql = %s\n", sql.c_str());
                mysqlTask2->get_req()->set_query(sql);
                series_of(mysqlTask)->push_back(mysqlTask2);
            }
            else{
                //失败
                resp->String("FAIL");
            }
        });
        
        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);
    });
}