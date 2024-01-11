
#include <iostream>
#include <vector>
#include <fstream>
class Observer {
public:
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& file, const std::string& message) {}
    virtual void onFatalError(const std::string& file, const std::string& message) {}
    virtual void update(const std::string& message) {}
    virtual void printout(const std::string& file, const std::string& message){}
};

class Observed {
private:
    std::vector<Observer*> observer_;
public:
    void AddObserver(Observer* observer) {
        observer_.push_back(observer);
    }
    void RemoveObserver(Observer* observer) {
        auto it = std::remove(observer_.begin(), observer_.end(), observer);
        observer_.erase(it, end(observer_));
    }
    void Notify(const std::string& message) {
        for (auto& observer : observer_) {
            observer->update(message);
        }
    };
};


 class ObserverClass : public Observer{
 private:
     std::ofstream out;
 public:
      void onWarning(const std::string& message) override {
          update("Warning" + message);
      }
      void onError(const std::string& file, const std::string& message) override {
          printout(file, "Error" + message);
      }
      void onFatalError(const std::string& file, const std::string& message) override {
          update("Fatal Error:" + message);
          printout(file, "Fatal Error:" + message);
      }

      void printout(const std::string& file, const std::string& message) override {
          out.open(file);
          if (out.is_open()) {
              out << message +"\n";

          }
          out.close();
      }

      void update(const std::string& message) override {
          std::cout << message << std::endl;
      }
 
 };



int main()
{
    Observed observed;
    ObserverClass observer_class;

    observed.AddObserver(&observer_class);
   
    observer_class.onWarning("......");
    observer_class.onError("out.txt", "......");
    observer_class.onFatalError("out.txt", "............");
}

