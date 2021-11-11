#ifndef JSONValueType_H
#define JSONValueType_H

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
}
#endif