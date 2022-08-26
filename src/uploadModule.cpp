#include "../include/uploadModule.h"
#include "../include/hashValue.h"
#include "../include/backup.h"
#include "../include/configure.h"

#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLUtil.h>

#include <string>

using std::string;

void UploadModule::addModule(wfrest::HttpServer& server, Config& config)
{
    server.GET("/file/upload",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("static/view/index.html");
    });

    server.POST("/file/upload",[config](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        // 1 解析用户的username和token
        // 校验token
        string username = req->query("username");
        string token = req->query("token");

        std::string uri = "mysql://root:123456@localhost";
        std::string sql = "SELECT user_token FROM cloudisk.tbl_user_token WHERE user_name = '" + username + "';";
        auto mysqlTask = WFTaskFactory::create_mysql_task(uri, 0, [config, req, resp, username, token](WFMySQLTask *mysqlTask){
            auto resp4mysql = mysqlTask->get_resp();
            protocol::MySQLResultCursor cursor(resp4mysql);
            std::vector<std::vector<protocol::MySQLCell>> rows;
            cursor.fetch_all(rows);

            // 2 token校验成功，根据用户名去查询数据库，得到单个用户的文件列表
            if(rows[0][0].as_string() == token)
            {
                //3 解析用户的post form-data请求
                using Form = std::map<string, std::pair<string, string>>;
                Form form = req->form();
                //4 取出文件的名字和内容 保存到服务端文件系统中
                auto filename = form["file"].first;
                string filepath = "uploadfiles/" + filename;
                auto filecontent = form["file"].second; 
                //fprintf(stderr,"filepath = %s\n, filecontent = %s\n", filepath.c_str(), filecontent.c_str());
                int fd = open(filepath.c_str(), O_RDWR|O_CREAT, 0666);
                write(fd, filecontent.c_str(), filecontent.size());
                close(fd);

                //5 将文件的元信息和文件用户的关系写入数据库
                HashValue filehashcalc(filepath);
                string filehash = filehashcalc.getSha1sum();
                string filesizestr = std::to_string(filecontent.size());

                char timestamp[40];
                time_t now = time(nullptr);
                struct tm *ptm = localtime(&now);
                sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", 
                        ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
                        ptm->tm_hour, ptm->tm_min ,ptm->tm_sec);
                string sql = "INSERT INTO cloudisk.tbl_file (file_sha1, file_name, file_size, file_addr) VALUES ('"
                    + filehash + "','" + filename + "'," + filesizestr + ", '" + filepath + "');";
                sql += "INSERT IGNORE INTO cloudisk.tbl_user_file (user_name, file_sha1, file_name, file_size, upload_at) VALUES('";
                sql += username + "','" + filehash + "','" +filename + "'," + filesizestr + ",'" + timestamp + "')";
                fprintf(stderr,"sql = %s\n", sql.c_str());

                string uri = "mysql://root:123456@localhost";
                auto mysqlTask2 = WFTaskFactory::create_mysql_task(uri,0,nullptr);
                mysqlTask2->get_req()->set_query(sql);
                series_of(mysqlTask)->push_back(mysqlTask2);                    

                // 可以将文件备份到OSS中
                Backup(filepath, filehash, config);

                //6 响应客户端一个上传完成的页面 重定向
                resp->set_status_code("301");
                resp->headers["Location"] = "/file/upload/success";
            }
            
            // 7 token校验失败
            else
            {
                resp->String("invalid token");
            }
        });

        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);
    });

    server.GET("/file/upload/success",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        //resp->String("Upload success");
        resp->set_status_code("301");
        resp->headers["Location"] = "/static/view/home.html";
    });
}