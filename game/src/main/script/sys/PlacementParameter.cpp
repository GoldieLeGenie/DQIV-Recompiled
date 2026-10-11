#include "main/script/sys/PlacementParameter.hpp"

int (*PlacementParameter::startFunction_)(void*);

ARM PlacementParameter::PlacementParameter()
{
}

ARM PlacementParameter::~PlacementParameter()
{
}

ARM void PlacementParameter::setup(void* addr)
{
    dataObject_.setup(addr);
}

ARM void PlacementParameter::cleanup()
{
    dataObject_.cleanup();
}

ARM int PlacementParameter::execute()
{
    return startFunction_(dataObject_.getAddr());
}

ARM void PlacementParameter::setStartFunction(int (*fc)(void*))
{
    startFunction_ = fc;
}
