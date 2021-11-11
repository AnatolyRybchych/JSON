#ifndef JSON_H
#define JSON_H



#include<memory>
#include<algorithm>
#include<fstream>
#include"JSONValueType.h"
#include"implementations.h"

namespace JSON
{
    class JSONObj
    {
    public:
        JSONObj(std::wstring content, std::wstring key = L"__root__");
        bool IsArray();
        bool IsObject();
        bool IsNumber();
        bool IsBool();
        bool IsString();
        std::vector<JSONObj> GetInnerObjects();
        std::wstring GetKey();
        std::wstring GetJsonContent();
        std::wstring GetString();
        int GetInt();
        bool GetBool();
        double GetDouble();
        float GetFloat();
        bool CanAdd();
        void AddInt(std::wstring key,int value);
        void AddDouble(std::wstring key,double value);
        void AddFloat(std::wstring key,float value);
        void AddString(std::wstring key,std::wstring value);
        void AddObject(JSONObj obj);
        void AddBool(std::wstring key,bool value);
        std::wstring ToString();
        bool WriteToFile(std::wstring path);
        JSONObj operator[](int index);
        operator int();
        operator double();
        operator float();
        operator bool();
        operator std::wstring();
    protected:
        JSONValueType _type;
        std::wstring _content;
        std::wstring _key;
        std::vector<JSONObj> _inner;

        //after _type definition
        void Init();

        //json - minimized json string
        static JSONValueType GetType(std::wstring json);
        //json - minimized json string
        static std::vector<JSONObj> GetArray(std::wstring json);
        //json - minimized json string
        static std::vector<JSONObj> GetInnerObjectPairs(std::wstring json);
    };
}

#endif