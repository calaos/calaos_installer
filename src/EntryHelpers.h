#ifndef ENTRYHELPERS_H
#define ENTRYHELPERS_H

#include "IEntryHelper.h"
#include "Params.h"
#include "DialogDetectxPL.h"
#include "DialogDetectMySensors.h"

class EntryHelpers
{
    public:
        static IEntryHelper* getEntryHelper(const Params &params, QWidget *parent)
        {
            if(params.get_param_const("type").substr(0,3) == "xPL")
                return new DialogDetectxPL(params, parent);
            if(params.get_param_const("type").substr(0,9) == "MySensors")
                return new DialogDetectMySensors(params, parent);

            return nullptr;
        }
};

#endif // ENTRYHELPERS_H
