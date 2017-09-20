#ifndef ENTRYHELPERS_H
#define ENTRYHELPERS_H

#include "IEntryHelper.h"
#include "Params.h"
#include "DialogDetectxPL.h"

class EntryHelpers
{
    public:
        static IEntryHelper* getEntryHelper(const Params &params, QWidget *parent)
        {
            if(params.get_param_const("type").substr(0,3) == "xPL")
                return new DialogDetectxPL(parent);

            return nullptr;
        }
};

#endif // ENTRYHELPERS_H
