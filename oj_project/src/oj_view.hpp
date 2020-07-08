#pragma once
#include <vector>
#include <string>
#include "oj_model.hpp"
#include <ctemplate/template.h>


class OjRender
{
    public:
        //用模板技术扩展html页面，并将页面返回给上层调用
        static void RenderAllQuestionshtml(std::string* html, std::vector<Question>& ques)
        {
            //1.获取数据字典-->将拿到的试题数据按照一定的顺序保存到内存当中
            ctemplate::TemplateDictionary dict("all_questions");
            for(const auto& que : ques)
            {
                //创建子字典
                ctemplate::TemplateDictionary* section_dict = dict.AddSectionDictionary("question");

                section_dict->SetValue("id", que.id_);//超链接使用
                section_dict->SetValue("id", que.id_);
                section_dict->SetValue("name", que.name_);
                section_dict->SetValue("star", que.star_);
            }
            //2获取类模板指针，加载预定义的html页面到内存中去
            ctemplate::Template* tl =  ctemplate::Template::GetTemplate("./template/all_questions.html", ctemplate::DO_NOT_STRIP);
            //3.渲染 拿着模板类的指针，将数据字典当中的数据更新到html页面的内存中
            tl->Expand(html, &dict);
        }

        static void RenderOneQuestionhtml(const Question& ques, std::string& desc, std::string& header, std::string* html)
        {
            ctemplate::TemplateDictionary dict("question");
            dict.SetValue("id", ques.id_);
            dict.SetValue("name", ques.name_);
            dict.SetValue("star", ques.star_);
            dict.SetValue("desc", desc);
            dict.SetValue("header", header);

            ctemplate::Template* tpl = ctemplate::Template::GetTemplate("./template/question.html", ctemplate::DO_NOT_STRIP);
            tpl->Expand(html, &dict);
        }

        static void RenderReasonhtml(const std::string& errorno, const std::string& reason, const std::string& stdout_reason, std::string* html)
        {
            ctemplate::TemplateDictionary dict("reason");
            dict.SetValue("errorno", errorno);
            dict.SetValue("reason", reason);
            dict.SetValue("stdout", stdout_reason);

            ctemplate::Template* tpl = ctemplate::Template::GetTemplate("./template/reason.html", ctemplate::DO_NOT_STRIP);
            tpl->Expand(html, &dict);
        }
};

