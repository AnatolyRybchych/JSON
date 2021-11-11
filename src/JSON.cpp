#include"../include/JSON.h"

#include<iostream>

namespace JSON
{
    JSONObj::JSONObj(std::wstring content, std::wstring key)
    {
        _content = MinimizeJsonStr(content);
        _key = key;
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

        for(auto str:GetStringBlocks(json,_find_object_pair_start,_find_object_pair_end))
        {
            int i = 0;
            std::wstring key;
            while(i < str.length())
            {
                if(str[i] == L':') break;
                if(str[i] != L'\"') key.push_back(str[i]);
                i++;
            }
            std::wcout<<str<<"\n";
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