#JSON

::JSON namespace -> lib namespace

    JSON::JSONObj -> json parser/editor class
    JSON::JSONObj(wstring data, wstring key = L"__root__")

    JSONObj["key"]          -> returns child element in first order with equal name
    GetLengh                -> returns count of child nodes
    JSONObj[index]          -> returns child element by order index
    JSONObj.IsArray         -> returns true if this element is json array : ["qwe","zxc"]   [1, 2 , 3]  [true, false, false]
    JSONObj.IsObject        -> returns true if this element is JsonObject : {"key1":"value1", "key2":2, "key3":false }
    JSONObj.IsString        -> returns true if this element is JsonString : "string"
    JSONObj.IsNumber /
    JsonObj.IsBool          -> returns true if can be casted to this type

    JSONObj.GetString       -> if IsString returns JsonString without "" (only content)
                                else     returns Json content as wstring
    
    (Type)JSONObj/
    JSONObj.Get##Type       -> if Is##Type returns content as Type type
                                else     returns {number -> 0, bool -> false}

    GetInnerObjects         -> returns std::vector of child objects , empty vector is number/bool/string
    GetKey                  -> returns key of current node
    
    CommitContentChanges    -> rewrites Json, with all applyed changes

    AddObject/
    Add[Type]               -> appends Json node by value  // CommitContentChanges to apply changes
                                    if node is not array/object -> does not make sense
    
    ToString                -> retunrs json content 
    WriteToFile             -> writes json content to file

                                
                                EXAMPLES

#file.json

    {
    "key1":"val1",
    "key2":1,
    "key3":{
        "key3_1":"val3",
        "key3_2":"val4"
        },
    "key4":false,
    "key5":[1,2,3],
    "key6":[true,false],
    "key7":["val","val","val2"]
    }

#main.cpp

    int main(int argc, char* atgv[])
    {
    std::wstring sval;
    int ival;
    bool bval;

    JSON::JSONObj obj(read_all_file(L"./file.json"));

    sval = (std::wstring)obj[L"key1"];                   //sval:L"val1"
    sval = (std::wstring)obj[L"key3"][L"key3_1"];        //sval:L"val3"
    
    ival = (int)obj[L"key2"];                            //ival:1
    ival = (int)obj[L"key5"][1];                         //ival:2

    bval = (bool)obj[L"key6"][0];                        //bval:true

    obj.AddObject(JSON::JSONObj(L"key8", L"{\"key8_1\":1}"))
    obj.AddFloat(L"key9",2.5);

    obj.CommitContentChanges();
    obj.WriteToFile(L"file.json");

    return 0;
    }

#file.json

    {
    "key1":"val1",
    "key2":1,
    "key3":{
        "key3_1":"val3",
        "key3_2":"val4"
        },
    "key4":false,
    "key5":[1,2,3],
    "key6":[true,false],
    "key7":["val","val","val2"],
    "key8":{
        "key8_1":1
    },
    "key9":2.5
    }

