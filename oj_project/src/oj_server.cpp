#include "httplib.h"
#include "compile.hpp"
#include "oj_view.hpp"
#include "oj_model.hpp"
#include <jsoncpp/json/json.h>

int main()
{
    //在使用httplib的时候，需要使用httplib提供的命名空间
    using namespace httplib;
    Server svr;
    OjModel ojmodel;
    //1.要获取试题信息（试题的信息来源于文件）
    //通过ojmodel模块来获取试题信息
    svr.Get("/all_questions", [&ojmodel](const Request& req, Response& resp){
            //获取试题信息
            std::vector<Question> vec;
            ojmodel.GetAllQuestions(&vec);

            //把获取到的试题信息返回给浏览器
            //可以返回一个html页面
            //先用模板技术来填充一下html页面
            std::string html;
            OjRender::RenderAllQuestionshtml(&html, vec);
            resp.set_content(html,"text/html; charset=UTF-8");
            });

    //2.获取具体题目信息
    //利用源码转义
    svr.Get(R"(/question/(\d+))", [&ojmodel](const Request& req, Response& resp){
            //1.去model中获取对应题号题目的信息
            //      map当中存有题目的（编号、名字、保存路径、难度）
            //header：用户可以操作的代码
            //desc：题目的描述信息
            std::string header;
            std::string desc;
            //从querystr当中获取id
            LOG(INFO, "req.matches") << req.matches[0] << ":" << req.matches[1] << std::endl;
            //2.获取题目保存路径然后加载题目
            //      要先获取题号:req.matches[i]--->从querystr中获取id
            Question ques;
            ojmodel.GetOneQuestion(req.matches[1].str(), &desc, &header, &ques);

            //3.组织html页面，返回给浏览器
            std::string html;
            OjRender::RenderOneQuestionhtml(ques, desc, header, &html);
            resp.set_content(html,"text/html; charset=UTF-8");
            });

    //处理浏览器提交的代码
    svr.Post(R"(/question/(\d+))", [&ojmodel](const Request& req, Response& resp){
            //1.从正文当中提取出来提交的内容。主要是提取code字段所对应的内容
            //  提交的内容当中有url编码-->提交内容进行 解码
            //  提取完成后的数据放到 unordered_map<std::string, std::string>
            std::unordered_map<std::string, std::string> pram;
            UrlUtil::PraseBody(req.body, &pram);
            //需要给提交的代码增加头文件，测试用例，main函数，也就是合并header & tail
            //2.合并
            std::string code;
            ojmodel.SplicingCode(pram["code"], req.matches[1].str(), &code);
            //3.编译运行
            Json::Value req_json;
            req_json["code"] = code;
            Json::Value resp_json;
            Compiler::CompileAndRun(req_json, &resp_json);
            //4.构造响应
            const std::string errorno = resp_json["errorno"].asString();
            const std::string reason = resp_json["reason"].asString();
            const std::string stdout_reason = resp_json["stdout"].asString();
            std::string html;
            OjRender::RenderReasonhtml(errorno, reason, stdout_reason, &html);
            resp.set_content(html,"text/html; charset=UTF-8");
            });


    svr.listen("0.0.0.0", 54321);
    return 0;
}
