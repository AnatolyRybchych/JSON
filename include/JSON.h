#include<string>
#include<vector>
#include<functional>
#include<exception>
#include<assert.h>
#include<memory>
#include<algorithm>

namespace JSON
{
    enum JSONValueType
    {
        ValueInt = 0x0100,
        ValueString = 0x1000,
        ValueBool = 0x1100,

        Array = 0x0001,
        Object = 0x0010,
        None = 0x0000
    };

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
        static std::vector<JSONObj> GetArray(std::wstring json);
        static std::vector<JSONObj> GetInnerObjectPairs(std::wstring json);
    };
}