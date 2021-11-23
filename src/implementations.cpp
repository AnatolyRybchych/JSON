#include "../include/implementations.h"
#include<iostream>

std::wstring MinimizeJsonStr(std::wstring json)
{
    std::wstring res;
    bool isInnerStr = false;
    wchar_t prev;
    for(auto ch:json) 
    {
        if(!isInnerStr && ch == L'\"') isInnerStr = true;
        else if(isInnerStr && ch ==L'\"' && prev != L'\\') isInnerStr = false;
        if((ch != L' ' && (ch & ~(wchar_t)31) && ch != L'\n' && ch != L'\t') || isInnerStr)
            res+= ch;
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
        if(!writing && findStartBlockHandler(i,&string))//serching start of block
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
            curr+=string[i];
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
        static bool first = true;
        return GetStringBlocks(json, 
            [&](int& index,std::wstring* string)->bool
                {
                    if(string->at(index) == L',' || string->at(index) == L']')
                    {
                        index-= 2;
                        return true;
                    }
                    else return false;
                },
            [&](int& index,std::wstring* string) -> bool
                {return string->at(index) == L','|| string->at(index) == L']';}
        );
    }
}

bool _find_object_pair_end(int& index, std::wstring* string)
{
    static int complexity = 1;
    static bool isInStr = false;

    if(index == 1)
    {
        isInStr = false;
        complexity = 1;
    } 

    switch (string->at(index))
    {
    case L'\"':
        if(isInStr)
        {
            isInStr = false;
            if(index > 0 && string->at(index-1) == L'\\') isInStr = true;
        }
        break;
    case L'[':
    case L'{':
        complexity ++;
        break;
    case L']':
    case L'}':
        complexity--;
        break;
    }

    if(complexity == 1 && isInStr == false && (string->at(index) == L',')) return true;
    if(complexity == 0) return true;
    else return false;
}

bool _find_object_pair_start(int& index, std::wstring* string)
{
    if(index!=0)index--; 
    return true; 
}