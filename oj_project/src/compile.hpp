#pragma once
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

#include "tools.hpp"

//定义错误码
enum ErrorNo
{
    OK = 0,          //没有错误
    COMPILE_ERROR,   //编译错误
    RUN_ERROR,       //运行错误
    PRAM_ERROR,      //参数错误  
    INTERNAL_ERROR   //内存错误 
};

class Compiler
{
    public:
        static void CompileAndRun(Json::Value req, Json::Value* resp)
        {
            //1.判空
            //{"code":"xxx", "stdin":"xxx"}
            if(req["code"].empty())
            {
                (*resp)["errorno"] = PRAM_ERROR;
                (*resp)["reason"] = "Pram error";
                LOG(ERROR, "Request Commande is Empty") << std::endl;
                return;
            }

            //2.将代码写入临时文件，可能有多个提交
            std::string code = req["code"].asString();
            //文件名称进行约定 tmp_时间戳.cpp
            std::string tmp_filename = WriteTmpFile(code);
            if(tmp_filename == "")
            {
                (*resp)["errorno"] = INTERNAL_ERROR;
                (*resp)["reason"] = "Create file failed";
                LOG(ERROR, "Write Source failed");
                return;
            }

            //3.编译
            if(!Compile(tmp_filename))
            {
                (*resp)["errorno"] = COMPILE_ERROR;
                std::string reason;
                FileOper::ReadDataFromFile(ErrorPath(tmp_filename), &reason);
                (*resp)["reason"] = reason; 
                LOG(ERROR, "Compile Error") << std::endl;
                Clean(tmp_filename);
                return;
            }

            //4.运行
            int sig = Run(tmp_filename);
            if(sig != 0) 
            {
                (*resp)["errorno"] = RUN_ERROR;
                (*resp)["reason"] = "Program exit by sig " + std::to_string(sig);
                LOG(ERROR, "Run Error") << std::endl;
                Clean(tmp_filename);
                return;

            }

            //5.构造响应
            (*resp)["errorno"] = OK;
            (*resp)["reason"] = "Compile and run is okey!";
            //标准输出
            std::string stdout_reason;
            FileOper::ReadDataFromFile(StdoutPath(tmp_filename), &stdout_reason);
            (*resp)["stdout"] = stdout_reason;
            //标准错误
            std::string stderr_reason;
            FileOper::ReadDataFromFile(StderrPath(tmp_filename), &stderr_reason);
            (*resp)["stderr"] = stderr_reason;

            //6.清理临时文件
            Clean(tmp_filename);
            return;

        }
    private:
        static std::string WriteTmpFile(const std::string& code)
        {
            //组织文件名称，组织文件的前缀名称，用来区分源码文件，可执行文件是同一组数据
            std::string tmp_filename = "/tmp_" + std::to_string(LogTime::GetTimeStamp());
            //写文件
            int ret = FileOper::WriteDataToFile(SrcPath(tmp_filename), code); 
            if(ret < 0)
            {
                LOG(ERROR, "Write code to source failed");
                return "";
            }
            return tmp_filename;
        }

        static std::string SrcPath(const std::string& filename)
        {
            return "./tmp_files" + filename + ".cpp";
        }

        static std::string ErrorPath(const std::string& filename)
        {
            return "./tmp_files" + filename + ".err";
        }

        static std::string ExePath(const std::string& filename)
        {
            return "./tmp_files" + filename + ".executable";
        }

        static std::string StdoutPath(const std::string& filename)
        {
            return "./tmp_files" + filename + ".stdout";
        }
        static std::string StderrPath(const std::string& filename)
        {
            return "./tmp_files" + filename + ".stderr";
        }

        static bool Compile(const std::string& filename)
        {
            //1.构造编译命令
            //g++ src -o [exec] -std=c++11 ...
            const int commandcount = 20;
            char buf[commandcount][50] = {{0}};
            char* Command[commandcount] = {0}; 
            for(int i = 0; i < commandcount; i++)
            {
                Command[i] = buf[i];

            }
            snprintf(Command[0], 49, "%s", "g++");
            snprintf(Command[1], 49, "%s", SrcPath(filename).c_str());
            snprintf(Command[2], 49, "%s", "-o");
            snprintf(Command[3], 49, "%s", ExePath(filename).c_str());
            snprintf(Command[4], 49, "%s", "-std=c++11");
            snprintf(Command[5], 49, "%s", "-D");
            snprintf(Command[6], 49, "%s", "CompileOnline");
            Command[7] = NULL;

            //2.创建子进程，进行程序替换
            int pid = fork();
            if(pid < 0)
            {
                LOG(ERROR, "Create child process failed");
                return false;

            }
            else if(pid == 0)
            {
                //打开编译错误文件，用于保存编译错误信息
                int fd = open(ErrorPath(filename).c_str(), O_CREAT|O_RDWR, 0664);
                if(fd < 0)
                {
                    LOG(ERROR, "open compile errorfile failed") << ErrorPath(filename) << std::endl;
                }

                //重定向
                dup2(fd, 2);
                //程序替换
                execvp(Command[0], Command);
                perror("execvp");
                LOG(ERROR, "execvp failed") << std::endl;
                exit(0);
            }
            else
            {
                waitpid(pid, NULL, 0);
            }

            //3.验证是否产生可执行程序
            struct stat st;
            int ret = stat(ExePath(filename).c_str(), &st);//获取文件信息
            if(ret < 0)
            {
                LOG(ERROR, "Compile ERROR! Exe filename is ") << ExePath(filename) << std::endl;
                return false;
            }
            return true;
        }

        static int Run(std::string& filename)
        {
            //也是创建子进程来进行程序替换，然后执行程序
            int pid = fork();
            if(pid < 0)
            {
                LOG(ERROR, "Exec pragma failed! Create child process failed") << std::endl;
                return -1;

            }
            else if(pid == 0)
            {
                //可以对程序执行进行限制
                //1.时间限制
                //alarm
                alarm(1);
                //2.内存大小限制
                struct rlimit rl;
                rl.rlim_cur = 1024 * 30000;
                rl.rlim_max = RLIM_INFINITY;//无限制
                setrlimit(RLIMIT_AS, &rl);
                //  获取 ：标准输出-->重定向到文件
                int stdout_fd = open(StdoutPath(filename).c_str(), O_CREAT | O_RDWR, 0664);
                if(stdout_fd < 0)
                {
                    LOG(ERROR, "Open stdout file failed") << StdoutPath(filename) << std::endl;
                    return -1;

                }
                dup2(stdout_fd, 1);
                //  标准错误-->重定向到文件
                int stderr_fd = open(StderrPath(filename).c_str(), O_CREAT | O_RDWR, 0664);
                if(stderr_fd < 0)
                {
                    LOG(ERROR, "Open stderr file failed") << StderrPath(filename) << std::endl;
                    return -1;

                }
                dup2(stderr_fd, 2);
                //执行程序
                execl(ExePath(filename).c_str(), ExePath(filename).c_str(), NULL);
                exit(1);
            }
            int Status = -1;
            waitpid(pid, &Status, 0);
            //将是否收到信号的信息返回给调用者，如果调用者判断是0， 则正常运行完毕，否则收到看了某个信号异常结束的
            return Status & 0x7f;
        }

        static void Clean(std::string filename)
        {
            unlink(SrcPath(filename).c_str());
            unlink(ExePath(filename).c_str());
            unlink(ErrorPath(filename).c_str());
            unlink(StdoutPath(filename).c_str());
            unlink(StderrPath(filename).c_str());

        }
};
