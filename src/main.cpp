#include "gablog.h"


int main()
{
    GABLOG_TRACE("Test");
    GABLOG_INFO("Test");
    GABLOG_WARN("Test");
    GABLOG_ERROR("Test");
    GABLOG_CRITICAL("Test");
    bool test = false;
    //GABLOG_ASSERT(test, "Test");

    TRACE("Test");
    ASSERT(test, "Test");
}