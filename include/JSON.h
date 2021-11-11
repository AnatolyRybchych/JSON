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
        //if IsArray or IsObject
        std::vector<JSONObj> GetInnerObjects();
        std::wstring GetKey();
        //Json String but if you add new lines,you shoud call CommitContentChanges to makeNewResult
        std::wstring GetJsonContent();
        //returns string value of content
        std::wstring GetString();
        //returns int value of content
        int GetInt();
        //returns bool value of content
        bool GetBool();
        //returns double value of content
        double GetDouble();
        //returns float value of content
        float GetFloat();
        bool CanAdd();
        void AddInt(std::wstring key,int value);
        void AddDouble(std::wstring key,double value);
        void AddFloat(std::wstring key,float value);
        void AddString(std::wstring key,std::wstring value);
        void AddObject(JSONObj obj);
        void AddBool(std::wstring key,bool value);
        //to apply changes for content
        void CommitContentChanges();
        //builds json using all data
        std::wstring ToString();
        //builds json using all data and writes into file
        bool WriteToFile(std::wstring path);
        int GetLengh();
        JSONObj operator[](int index);
        JSONObj operator[](std::wstring key);
        JSONObj operator[](const wchar_t* key);
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