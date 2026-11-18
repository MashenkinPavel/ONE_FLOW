#pragma once
#include <string>
#include <fstream>

enum class SolverType{
    SIMPLE,
};

struct CGeneralSettings{
    std::string foam_dir;
    std::string work_dir;
    SolverType slv_type = SolverType::SIMPLE;
};


class Logger{
    public:
    Logger(std::string filename){
        out.open(filename);
        
    }
    ~Logger(){
        out.close();
    }
    void print(std::string str){
        out<<str;
    }
    private:
    std::ofstream out;
};

using LoggerPtr = std::shared_ptr<Logger>;

LoggerPtr& operator<<(LoggerPtr& log, const std::string& str){
    log->print(str);
    return log;
}

LoggerPtr& operator<<(LoggerPtr& log, const char* str){
    std::string strout(str);
    log->print(strout);
    std::cout<<str;
    return log;
}