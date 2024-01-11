#include <iostream>
#include <fstream>
#include <stdexcept>

class LogMessage {
public:
    enum class Type {
        Warning,
        Error,
        FatalError,
        Unknow
    };
    LogMessage(Type messageType, const std::string& message) : messageType_(messageType), message_(message) {}
    Type type() const {
        return messageType_;
    };
    const std::string& message() const {
        return message_;
    };
private:
    Type messageType_;
    std::string message_;
};

class AbstractHandler {
protected:
    std::unique_ptr<AbstractHandler> nextHandler_;
    std::ofstream file;
    std::string filename_;
public:
    virtual ~AbstractHandler() = default;
    virtual void handler(const LogMessage& logmessage) = 0;

    void setNextHandler(std::unique_ptr<AbstractHandler> nextHandler) {
        nextHandler_ = std::move(nextHandler);
    }
    void setFileName(std::string filename) {
        filename_ = filename;
    }
    void outFileMessage(const std::string& filename, const std::string& message) {
        file.open(filename);
        if (file.is_open()) {
            file << message + "\n";
            file.close();
        }
    }
};

class FatalErrorHandler : public AbstractHandler {
public:
    void handler(const LogMessage& logmessage) override {
        if (logmessage.type() == LogMessage::Type::FatalError) {
            throw std::runtime_error(logmessage.message());
        }
        else {
            nextHandler_->handler(logmessage);
        }
    }
};
class ErrorHandler : public AbstractHandler {
public:
    ErrorHandler(std::string filename) {
        setFileName(filename);
    }

    void handler(const LogMessage& logmessage) override {
        if (logmessage.type() == LogMessage::Type::Error) {
            outFileMessage(filename_, logmessage.message());
          
        }
        else {
            nextHandler_->handler(logmessage);
        }
    }
};
class WarningHandler : public AbstractHandler {
public:
    void handler(const LogMessage& logmessage) override {
        if (logmessage.type() == LogMessage::Type::Warning) {
            std::cout << logmessage.message() << std::endl;
        }
        else {
            nextHandler_->handler(logmessage);
        }
    }
};
class UnknowHandler : public AbstractHandler {
public:
    void handler(const LogMessage& logmessage) override {
        if (logmessage.type() == LogMessage::Type::Unknow) {
            throw std::runtime_error("Unknow message!!!");
        }
        else {
            nextHandler_->handler(logmessage);
        }
    }
};

int main()
{
    LogMessage fatalMessage(LogMessage::Type::FatalError, "Is Fatal Error!!!");
    LogMessage errorMessage(LogMessage::Type::Error, "Is Error!!!");
    LogMessage warningMessage(LogMessage::Type::Warning, "//////Warning//////");
    LogMessage unknowMessage(LogMessage::Type::Unknow, "alalal");

    auto fatalHandler = std::make_unique<FatalErrorHandler>();
    auto errorHandler = std::make_unique<ErrorHandler>("out.txt");
    auto warningHandler = std::make_unique<WarningHandler>();
    auto unknowHandler = std::make_unique<UnknowHandler>();

    //цепочка обработки

    //fatalHandler->setNextHandler(std::move(errorHandler));
    errorHandler->setNextHandler(std::move(warningHandler));
    //warningHandler->setNextHandler(std::move(unknowHandler));


    //Каждый обработчик обрабатывает только свое сообщение, перемещал указатели, но понимаю уже что так, как деляю я нельзя,
    // пробывал создавать vector<unique_ptr<AbstractHandler>> и итерироваться, если не проходил по условиям, стало только хуже.



    try {
       // fatalHandler->handler(fatalMessage);
        errorHandler->handler(warningMessage);
        //warningHandler->handler(warningMessage);
        //unknowHandler->handler(unknowMessage);
    }
    catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}