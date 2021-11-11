#include"../include/JSON.h"
#include<iostream>

#define LOWER_STR(a) std::wstring(_wcslwr(const_cast<wchar_t*>(a.c_str())))

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

//findStartBlockHandler procs for each char before it returns true and after findEndBlockHandler == true
//findEndBlockHandler procs for each other characters
//if any handler returns true or index out of copying block, than charackter ignores
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

//json - minimized string "[data]"
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

namespace JSON
{
    JSONObj::JSONObj(std::wstring content, std::wstring key)
    {
        _content = MinimizeJsonStr(content);
        _key = key;
        std::wcout<<content<<"\n";
        _type = GetType(_content);
        Init();

    }

    void JSONObj::Init()
    {
        if(_type == JSONValueType::Array)
        {
            _inner = GetArray(_content);
        }
        if(_type == JSONValueType::Object)
        {
            _inner = GetInnerObjectPairs(_content);
        }
    }

    std::vector<JSONObj> JSONObj::GetArray(std::wstring json)
    {
        std::vector<JSONObj> res;
        int index = 0;
        for(auto str:GetStrArrays(json))
            res.push_back(JSONObj(str,std::to_wstring(index++)));
        return res;
    }

    std::vector<JSONObj> JSONObj::GetInnerObjectPairs(std::wstring json)
    {
        std::vector<JSONObj> res;


        auto findObjectPairEnd = [&](int& index, std::wstring* string) -> bool
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
            else if(complexity == 0) return true;
            else return false;
        };

        auto findObjectPairStart = [&](int& index, std::wstring* string) -> bool
        {
            index--;
            return true;
        };

        for(auto str:GetStringBlocks(json,findObjectPairStart,findObjectPairEnd))
        {
            int i = 0;
            std::wstring key;
            while(i < str.length())
            {
                if(str[i] == L':') break;
                if(str[i] != L'\"') key.push_back(str[i]);
                i++;
            }
            res.push_back(JSONObj(str.substr(i+1),key));
        }
        return res;
    }


    JSONValueType JSONObj::GetType(std::wstring json)
    {
        if(json.empty()) return JSONValueType::None;

        switch (json[0])
        {
        case L'{':
            return JSONValueType::Object;
        case L'[':
            return JSONValueType::Array;
        case L'\"':
            return JSONValueType::ValueString;
        default:
            if(iswdigit(json[0])) 
                return JSONValueType::ValueInt;
            else
                return JSONValueType::ValueBool;
        }
    }
}