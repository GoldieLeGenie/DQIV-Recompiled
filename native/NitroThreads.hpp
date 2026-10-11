#pragma once
#include <functional>
#include <memory>
namespace pc {
struct ThreadRuntime;
class NitroThreads {
public:
    NitroThreads();
    ~NitroThreads();
    NitroThreads(const NitroThreads&)=delete;
    NitroThreads& operator=(const NitroThreads&)=delete;
    void checkErrors();
    void setIdleHandler(std::function<void()> handler);
private:
    std::unique_ptr<ThreadRuntime> impl_;
};
unsigned suspendNativeInterrupts();
void resumeNativeInterrupts(unsigned depth);
}
