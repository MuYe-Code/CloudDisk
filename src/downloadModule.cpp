#include "../include/downloadModule.h"

#include <workflow/WFFacilities.h>
#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLUtil.h>

void DownloadModule::addModule(wfrest::HttpServer& server, Config& config)
{
    server.POST("/file/downloadurl",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        // 1 解析用户请求 hash user token
        auto queryInfo = req->query_list();
        std::string filesha1 = queryInfo["filehash"];
        std::string filename = queryInfo["filename"];
        // 2 校验token
        // 3 根据hash查mysql，得到文件名
        std::string uri = "mysql://root:123456@localhost";
        std::string sql = "SELECT file_name FROM cloudisk.tbl_file WHERE file_sha1='" + filesha1 + "';";
        auto mysqlTask = WFTaskFactory::create_mysql_task(uri,0,[resp, filename](WFMySQLTask *mysqlTask){
            // 4 根据文件名，构造一个url去访问nginx
            auto resp4mysql = mysqlTask->get_resp();
            protocol::MySQLResultCursor cursor(resp4mysql);
            std::vector<std::vector<protocol::MySQLCell>> rows;
            cursor.fetch_all(rows);
            if(1 == cursor.get_rows_count() && rows[0][0].as_string() == filename)
            {
                //生成一个访问Nginx的连接用于下载
                std::string url = "http://192.168.150.142:1235/" + filename;
                resp->String(url);
            }
            else { resp->String("No such file"); }
        });
        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);
    });
}