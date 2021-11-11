#ifndef JSONValueType_H
#define JSONValueType_H

namespace JSON
{
    enum JSONValueType
    {
        ValueNumber,
        ValueString,
        ValueBool,

        Array,
        Object,
        None
    };
}
#endif