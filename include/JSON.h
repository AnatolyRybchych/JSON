#ifndef JSON_H
#define JSON_H



#include<memory>
#include<algorithm>

#include"JSONValueType.h"
#include"implementations.h"

namespace JSON
{
    class JSONObj
    {
    public:
        JSONObj(std::wstring content, std::wstring key = L"root");
        JSONValueType GetType();
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