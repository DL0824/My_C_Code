#pragma once
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include "tools.hpp"

typedef struct Question
{
    std::string id_;    //试题编号
    std::string name_;  //试题名称
    std::string path_;  //试题保存的路径
    std::string star_;   //试题难度
}TESTQUESTION;

class OjModel
{
    public:
        OjModel()
        {
            //将试题信息放到unordered_map中去
            LoadQuestions("./config_oj.cfg");
        }
        //用来获取所有试题的信息
        bool GetAllQuestions(std::vector<Question>* ques)
        {
            for(const auto& e : model_map_)
            {
                //将map中的试题信息给ques，做出参返回
                ques->push_back(e.second);
            }

            //可以进行排序，让试题列表有序
            std::sort(ques->begin(), ques->end(), [](Question& left, Question& right)
                    {
                    return std::atoi(left.id_.c_str()) < std::atoi(right.id_.c_str());
                    });
            return true;
        }

        bool GetOneQuestion(const std::string& id, std::string* desc, std::string* header, Question* ques)
        {
            //1.根据id找到对应的题目信息
            auto it = model_map_.find(id);
            if(it == model_map_.end())
            {
                LOG(ERROR, "Not Found Question:") << id <<std::endl;
                return false;
            }

            //拿到试题信息返回给上层用于渲染html页面的时候用
            *ques = it->second;

            //从具体的题目文件当中去获取两部分信息，desc，header头
            int ret = FileOper::ReadDataFromFile(DescPath(it->second.path_), desc);
            if(ret == -1)
            {
                LOG(ERROR, "Read desc failed") << std::endl;
                return false;
            }

            ret = FileOper::ReadDataFromFile(HeaderPath(it->second.path_), header);
            if(ret == -1)
            {
                LOG(ERROR, "Read desc failed") << std::endl;
                return false;
            }

            return true;
        }


        bool SplicingCode(std::string user_code, const std::string& ques_id, std::string* code)
        {
            //1.查找下对应id的题目是否存在
            auto iter = model_map_.find(ques_id);
            if(iter == model_map_.end())
            {
                LOG(ERROR, "can not find question id is ") << ques_id << std::endl;
                return false;
            }

            //2.读取tail
            std::string tail_code;
            int ret = FileOper::ReadDataFromFile(TailPath(iter->second.path_), &tail_code);
            if(ret < 0)
            {
                LOG(ERROR, "Open tail.cpp failed");
                return false;
            }

            //3.合并
            *code = user_code + tail_code;
            return true;
        }
    private:
        bool LoadQuestions(const std::string configfile_path)
        {
            std::ifstream file(configfile_path.c_str());
            if(!file.is_open())
                return false;

            std::string line;
            while(std::getline(file, line))
            {
                //1 二叉树 ./xxx 中等
                //1.需要切割字符串
                //2.切割后的内容保存到unordered_map中
                std::vector<std::string> vec;
                StringTool::Split(line, " ", &vec);
                if(vec.size() != 4)
                    continue;
                Question ques;
                ques.id_ = vec[0];
                ques.name_ = vec[1];
                ques.path_ = vec[2];
                ques.star_ = vec[3];
                model_map_[ques.id_] = ques;
            }
        }

        std::string DescPath(const std::string& ques_path)
        {
            return ques_path + "desc.txt";

        }
        std::string HeaderPath(const std::string& ques_path)
        {
            return ques_path + "header.cpp";

        }
        std::string TailPath(const std::string& ques_path)
        {
            return ques_path + "tail.cpp";

        }

    private:
        std::unordered_map<std::string, Question> model_map_;

};
