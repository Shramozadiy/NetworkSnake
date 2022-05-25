#include "readText.h"

ReadText& ReadText::getInstance() {
    static ReadText  instance;
    return instance;
}

void ReadText::set(WPARAM wP)
{
    std::lock_guard<std::mutex> lg(_lock);
    wParam.push_back(wP);
}

WPARAM ReadText::get(int id)
{
    std::lock_guard<std::mutex> lg(_lock);
    return wParam[id];
}

long long ReadText::getNumber()
{
    std::lock_guard<std::mutex> lg(_lock);
    return wParam.size();
}