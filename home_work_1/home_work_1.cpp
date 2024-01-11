
#include <iostream>
#include <fstream>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLog : public LogCommand {
public:

    void print (const std::string& message) override {
        std::cout << message << std::endl;
    };
};

class PrintFileLog : public LogCommand {
private:
    std::string file_;
    std::ofstream out;

public:
    PrintFileLog (const std::string& file) : file_(file) {};
    void print( const std::string& message) override {
        out.open(file_);
        if (out.is_open())
        {
            out << message << std::endl;
        }
        out.close();
    };

};
void print(LogCommand& logcommand, const std::string message) {
    logcommand.print(message);
}

int main()
{
    ConsoleLog conlog;
    PrintFileLog filelog("out.txt");
    print(filelog, "wtf");
    print(conlog, "wtf");

}

