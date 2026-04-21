#include <algorithm>

#include "gabdebug.h"

static bool test = false;

int main()
{
    GABLOG_TRACE("Test");
    GABLOG_INFO("Test");
    GABLOG_WARN("Test");
    GABLOG_ERROR("Test");
    //GABLOG_ASSERT(test, "Test");

    for (int frame = 0; frame < 100; frame++)
    {
        GABPROFILER_CLEAR();

        GABPROFILE_SCOPE("BIG FRAME")
        {
            GABPROFILE_SCOPE("Frame")
            {
                volatile double result = 0.0;

                for (uint32_t i = 1; i < 10000; i++)
                {
                    GABPROFILE_SCOPE("TEST A")
                    {
                        for (uint32_t j = 1; j < 10000; j++)
                            result += (i * 0.5) / (j + 1.0);
                    }
                    GABPROFILE_SCOPE("TEST B")
                    {
                        for (uint32_t j = 1; j < 10000; j++)
                            result += (i * 0.5) * (j + 1.0);
                    }
                }
            }

            GABPROFILE_SCOPE("Frame2")
            {
                volatile double result = 0.0;

                for (uint32_t i = 1; i < 10000; i++)
                {
                    GABPROFILE_SCOPE("TEST A1")
                    {
                        for (uint32_t j = 1; j < 10000; j++)
                            result += (i * 0.5) / (j + 1.0);
                    }
                    GABPROFILE_SCOPE("TEST B2")
                    {
                        for (uint32_t j = 1; j < 10000; j++)
                            result += (i * 0.5) * (j + 1.0);
                    }
                }
            }
        }

        GABPROFILER_PRINT();
    }
}