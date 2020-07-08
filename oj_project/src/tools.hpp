#pragma once 
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <boost/algorithm/string.hpp>


///////////////////////////////////////////////////////////////////////////////////
//实现一个切割字符串的工具函数
///////////////////////////////////////////////////////////////////////////////////
class StringTool
{
    public:
        //input:待切割的字符串 split_char:分隔字符 output:切割完成之后存放的地方
        static void Split(const std::string& input, const std::string& split_char, std::vector<std::string>* output)
        {
            boost::split(*output, input, boost::is_any_of(split_char), boost::token_compress_off);
        }
};



///////////////////////////////////////////////////////////////////////////////
//封装时间戳获取函数
///////////////////////////////////////////////////////////////////////////////
class LogTime
{
    public:
        static int64_t GetTimeStamp()
        {
            struct timeval tv;
            gettimeofday(&tv, NULL);
            return tv.tv_sec;

        }

        //返回 年-月-日 时:分:秒
        static void GetTimeStamp(std::string* TimeStamp)
        {
            time_t SysTime;
            time(&SysTime);//返回从1970.1.1 0:0:0到现在的一个秒数
            struct tm* st = localtime(&SysTime);
            char buf[30] = {'\0'};
            snprintf(buf, sizeof(buf) - 1, "%04d-%02d-%02d %02d:%02d:%02d", st->tm_year + 1900, st->tm_mon + 1, st->tm_mday, st->tm_hour, st->tm_min, st->tm_sec);
            TimeStamp->assign(buf, strlen(buf));
        }
};



///////////////////////////////////////////////////////////////////////////////////////
//封装一个简单的日志库
///////////////////////////////////////////////////////////////////////////////////////
//日志等级：INFO WARNING ERROR FATAL DEBUG
const char* Level[] = {
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL",
    "DEBUG"
};

enum LogLevel
{
    INFO = 0,
    WARNING,
    ERROR,
    FATAL,
    DEBUG
};

inline std::ostream& Log(LogLevel lev, const char* file, int line, const std::string& logmsg)
{
    std::string level_info = Level[lev];
    std::string TimeStamp;
    LogTime::GetTimeStamp(&TimeStamp);
    std::cout << "[" <<TimeStamp << " " << level_info << " " << file << line << "]" << logmsg;
    return std::cout;
}

#define LOG(lev, msg) Log(lev, __FILE__, __LINE__, msg)



/////////////////////////////////////////////////////////////////////
//实现文件操作的类
////////////////////////////////////////////////////////////////////
class FileOper
{
    public:
        static int ReadDataFromFile(const std::string& filename, std::string* content)
        {
            std::ifstream file(filename.c_str());
            if(!file.is_open())
            {
                LOG(ERROR, "Open file failed! filename is") << filename << std::endl;
                return -1;
            }

            std::string line;
            while(std::getline(file, line))
            {
                *content += line + "\n";
            }
            file.close();
            return 0;
        }

        static int WriteDataToFile(const std::string& filename, const std::string& Data)
        {
            std::ofstream file(filename.c_str());
            if(!file.is_open())
            {
                LOG(ERROR, "Open file failed") << filename << std::endl;
                return -1;
            }

            file.write(Data.data(), Data.size());
            file.close();
            return 0;
        }





};


//////////////////////////////////////////////
//封装url相关操作
/////////////////////////////////////////////

class UrlUtil
{
    public:
        static void PraseBody(const std::string& body, std::unordered_map<std::string, std::string>* pram)
        {
            //name=xxx&stdin=xxxx
            std::vector<std::string> tokens;
            StringTool::Split(body, "&", &tokens);
            for(const auto& token:tokens)
            {
                //name=xxxx
                std::vector<std::string> vec;
                StringTool::Split(token, "=", &vec);
                if(vec.size() != 2)
                {
                    continue;
                }
                (*pram)[vec[0]] = UrlDecode(vec[1]);
            }
        }
    private:
        static unsigned char ToHex(unsigned char x) 
        { 
            return  x > 9 ? x + 55 : x + 48; 
        }

        static unsigned char FromHex(unsigned char x) 
        { 
            unsigned char y;
            if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
            else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
            else if (x >= '0' && x <= '9') y = x - '0';
            else assert(0);
            return y;
        }


        static std::string UrlEncode(const std::string& str)
        {
            std::string strTemp = "";
            size_t length = str.length();
            for (size_t i = 0; i < length; i++)
            {
                if (isalnum((unsigned char)str[i]) || 
                        (str[i] == '-') ||
                        (str[i] == '_') || 
                        (str[i] == '.') || 
                        (str[i] == '~'))
                    strTemp += str[i];
                else if (str[i] == ' ')
                    strTemp += "+";
                else
                {
                    strTemp += '%';
                    strTemp += ToHex((unsigned char)str[i] >> 4);
                    strTemp += ToHex((unsigned char)str[i] % 16);

                }

            }
            return strTemp;

        }

        static std::string UrlDecode(const std::string& str)
        {
            std::string strTemp = "";
            size_t length = str.length();
            for (size_t i = 0; i < length; i++)
            {
                if (str[i] == '+') strTemp += ' ';
                else if (str[i] == '%')
                {
                    assert(i + 2 < length);
                    unsigned char high = FromHex((unsigned char)str[++i]);
                    unsigned char low = FromHex((unsigned char)str[++i]);
                    strTemp += high*16 + low;

                }
                else strTemp += str[i];
            }
            return strTemp;
        }
};

