#pragma once


#include <Windows.h>
#include <vector>
#include <mutex>

class ReadText
{
    std::mutex _lock;

    ReadText() {}
    ReadText(const ReadText&);
    ReadText& operator=(ReadText&);


    std::vector <WPARAM> wParam;
public:

    static ReadText& getInstance();

    void set(WPARAM wP);

    WPARAM get(int id);

    long long getNumber();

    
};