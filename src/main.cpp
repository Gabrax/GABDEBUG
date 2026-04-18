#include <algorithm>

#include "gabdebug.h"

int main()
{
    GABLOG_TRACE("Test");
    GABLOG_INFO("Test");
    GABLOG_WARN("Test");
    GABLOG_ERROR("Test");
    bool test = false;
    //GABLOG_ASSERT(test, "Test");

    TRACE("Test");
    //ASSERT(test, "Test");

    for (int frame = 0; frame < 100; frame++)
    {
        GABPROFILER_CLEAR_RESULTS();

        GABPROFILE_SCOPE("Frame")
        {
            volatile double result = 0.0;

            for (uint32_t i = 1; i < 10000; i++)
                for (uint32_t j = 1; j < 10000; j++)
                    result += (i * 0.5) / (j + 1.0);
        }

        GABPROFILER_PRINT_RESULTS();
    }
}