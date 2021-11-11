#ifndef IMPLEMENTATIONS_H
#define IMPLEMENTATIONS_H

#include<string>
#include<vector>
#include<functional>

#define LOWER_STR(a) std::wstring(_wcslwr(const_cast<wchar_t*>(a.c_str())))

//returns string without ' ' ,'\n' outside string literals
std::wstring MinimizeJsonStr(std::wstring json);

//findStartBlockHandler procs for each char before it returns true and after findEndBlockHandler == true
//findEndBlockHandler procs for each other characters
//if any handler returns true or index out of copying block, than charackter ignores
std::vector<std::wstring> GetStringBlocks(
    std::wstring string,
    std::function<bool(int& index, std::wstring* str)> findStartBlockHandler,
    std::function<bool(int& index, std::wstring* str)> findEndBlockHandler);

//json - minimized string "[data]"
std::vector<std::wstring> GetStrArrays(std::wstring json);


bool _find_object_pair_end(int index, std::wstring* string);
bool _find_object_pair_start(int index, std::wstring* string);

#endif