#include "../include/implementations.h"


std::wstring MinimizeJsonStr(std::wstring json)
{
    std::wstring res;
    bool isInnerStr = false;
    wchar_t prev;
    for(auto ch:json) 
    {
        if(!isInnerStr && ch == L'\"') isInnerStr = true;
        else if(isInnerStr && ch ==L'\"' && prev != L'\\') isInnerStr = false;
        if((ch != L' ' && ch != L'\n' && ch != L'\t') || isInnerStr)
            res.push_back(ch);
        prev = ch;
    }
    return res;
}

std::vector<std::wstring> GetStringBlocks(
    std::wstring string,
    std::function<bool(int& index, std::wstring* str)> findStartBlockHandler,
    std::function<bool(int& index, std::wstring* str)> findEndBlockHandler)
{
    std::vector<std::wstring> res;
    std::wstring curr;
    bool writing = false;
    for (int i = 0; i < string.length(); i++)
    {
        if(!writing && findStartBlockHandler(i,&string))//serchig start of block
        {
            writing = true;
            continue;
        }
        if(writing && findEndBlockHandler(i,&string))//searching end of block
        {
            writing = false;
            res.push_back(curr);
            curr.clear();
            continue;
        }
        if(writing)
        {
            curr.push_back(string[i]);
        }
    }
    return res;
}


std::vector<std::wstring> GetStrArrays(std::wstring json)
{
    if(json.size() < 3)         return std::vector<std::wstring>();
    else if(json[0] != L'[')    return std::vector<std::wstring>();

    if(json[1] == L'\"')
    {
        return GetStringBlocks(json, 
            [&](int& index,std::wstring* string)->bool
                {return string->at(index) == L'\"';},
            [&](int& index,std::wstring* string) -> bool
                {return string->at(index) == L'\"' && string->at(index) != L'\\';}
        );
    }
    else if(json[1] == L'[')
    {
        return GetStringBlocks(json, 
            [&](int& index,std::wstring* string)->bool
                {return string->at(index) == L'[' || string->at(index) == L'[';},
            [&](int& index,std::wstring* string) -> bool
                {return string->at(index) == L']';}
        );
    }
    else
    {
        return GetStringBlocks(json, 
            [&](int& index,std::wstring* string)->bool
                {return string->at(index) == L',';},
            [&](int& index,std::wstring* string) -> bool
                {return string->at(index) == L',';}
        );
    }
}

bool _find_object_pair_end(int index, std::wstring* string)
{
    static int complexity = 0;
    bool isInStr = false;
    if(string->at(index) == L'\"')
    {
        if(isInStr)
        {
            isInStr = true;
        }
        else
        {
            if(string->at(index-1) == L'\\') 
            {
                isInStr = false;
                if(index > 0)if(string->at(index-1) == L'\\') isInStr = true;
            }
        }
    }
    else if(string->at(index) == L'{') complexity ++;
    else if(string->at(index) == L'}') complexity--;
    if(complexity == 1 && isInStr == false && (string->at(index) == L',')) return true;
    else return false;
}

bool _find_object_pair_start(int index, std::wstring* string)
{
    index--;
    return true;
}