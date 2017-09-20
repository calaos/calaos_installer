#ifndef IENTRYHELPER_H
#define IENTRYHELPER_H

#include "Params.h"

class IEntryHelper
{
    public:
        virtual ~IEntryHelper() {}
        virtual int exec() = 0;
        virtual Params& getParams() = 0;
};

#endif // IENTRYHELPER_H
