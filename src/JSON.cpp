#include"../include/JSON.h"

namespace JSON
{
    JSONObj::JSONObj(std::wstring content, std::wstring key)
    {
        _content = MinimizeJsonStr(content);
        _key = key;
        _type = GetType(_content);
        Init();
    }

    bool JSONObj::IsArray()
    {
        return _type == JSONValueType::Array;
    }

    bool JSONObj::IsObject()
    {
        return _type == JSONValueType::Object;
    }

    bool JSONObj::IsNumber()
    {
        return _type == JSONValueType::ValueNumber;
    }

    bool JSONObj::IsBool()
    {
        return _type == JSONValueType::ValueBool;
    }

    bool JSONObj::IsString()
    {
        return _type == JSONValueType::ValueString;
    }

    std::vector<JSONObj> JSONObj::GetInnerObjects()
    {
        return _inner;
    }

    std::wstring JSONObj::GetKey()
    {
        return _key;
    }

    std::wstring JSONObj::GetJsonContent()
    {
        return _content;
    }

    std::wstring JSONObj::GetString()
    {
        if(!_content.size() > 2 && _content[0] == L'\"')
        {
            return _content.substr(1,_content.size()-1);
        }
        return _content;
    }

    int JSONObj::GetInt()
    {
        if(IsNumber() == false)
        {
            return 0;
        }
        else 
        {
            return std::stoi(GetString());
        }
    }

    bool JSONObj::GetBool()
    {
        return LOWER_STR(MinimizeJsonStr(GetString())).find(L"true") != -1;
    }

    bool JSONObj::CanAdd()
    {
        return IsObject() || IsArray();
    }

    void JSONObj::AddInt(std::wstring key,int value)
    {
        if(CanAdd())
        {
            _inner.push_back(JSONObj(std::to_wstring(value),key));
        }
    }

    void JSONObj::AddDouble(std::wstring key,double value)
    {
        if(CanAdd())
        {
            _inner.push_back(JSONObj(std::to_wstring(value),key));
        }
    }

    void JSONObj::AddFloat(std::wstring key,float value)
    {
        if(CanAdd())
        {
            _inner.push_back(JSONObj(std::to_wstring(value),key));
        }
    }

    void JSONObj::AddString(std::wstring key,std::wstring value)
    {
        if(CanAdd())
        {
            _inner.push_back(JSONObj(L"\""+value+L"\"",key));
        }
    }

    void JSONObj::AddObject(JSONObj obj)
    {
        _inner.push_back(obj);
    }

    void JSONObj::AddBool(std::wstring key,bool value)
    {
        if(CanAdd())
        {
            _inner.push_back(JSONObj(std::to_wstring(value),key));
        }
    }
    void JSONObj::CommitContentChanges()
    {
        _content = ToString();
    }

    std::wstring JSONObj::ToString()
    {
        std::wstring res = L"";
        int counter = 0;
        if(IsObject())
        {
            res+=L"{";
            for(auto in:_inner)
            {
                if(counter++) res+= L',';
                std::wstring innerStr;
                for(auto ch:in.ToString())
                {
                    innerStr+= ch;
                    innerStr += ch==L'\n'?L"\t":L"";
                }
                res+= std::wstring(L"\n\"") + in.GetKey() + L"\":" + innerStr;
            }
            res+=L"\n}";
        }
        else if(IsArray())
        {
            res+=L"[";
            for(auto in:_inner)
            {
                if(counter++) res+= L',';
                res += std::wstring(L"\n\t") + in.GetString();
            }
            res += L"\n]";

        }
        else if(IsString())
        {
            res += GetString();
        }
        else
        {
            res += GetString();
        }
        return res;
    }

    bool JSONObj::WriteToFile(std::wstring path)
    {
        std::wofstream fs(path.c_str(),std::ios::out);
        if(fs.is_open())
        {
            std::wstring out = ToString();
            fs.write(out.c_str(),out.length());
            fs.close();
            return true;
        }
        else
        {
            return false;
        }
        
    }

    double JSONObj::GetDouble()
    {
        if(IsNumber())
        {
            std::wstring str = GetString();
            std::replace(str.begin(),str.end(),L',',L'.');
            return std::stod(GetString());
        }
        else return 0;
    }

    float JSONObj::GetFloat()
    {
        if(IsNumber())
        {
            std::wstring str = GetString();
            std::replace(str.begin(),str.end(),L',',L'.');
            return std::stof(str);
        }
        else return 0;
    }

    JSONObj JSONObj::operator[](int index)
    {
        return _inner[index];
    }

    JSONObj::operator int()
    {
        return GetInt();
    }

    JSONObj::operator double()
    {
        return GetDouble();
    }

    JSONObj::operator float()
    {
        return GetFloat();
    }

    JSONObj::operator bool()
    {
        return GetBool();
    }

    JSONObj::operator std::wstring()
    {
        return GetString();
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
                if(str[i] != L'\"') key+=str[i];
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
                return JSONValueType::ValueNumber;
            else
                return JSONValueType::ValueBool;
        }
    }
}