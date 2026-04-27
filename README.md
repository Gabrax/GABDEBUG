<div align="center">

# Logging

</div>

```c
GABLOG_TRACE("Trace message");
GABLOG_INFO("Hello %s", "world");
GABLOG_WARN("Warning!");
GABLOG_ERROR("Error occurred");
GABLOG_ASSERT(!x, "Assertion failed: %d is false, x);
```

Example Output:

<img width="393" height="109" alt="{E9B8F1CD-D0E2-435B-BC18-C7BF35311187}" src="https://github.com/user-attachments/assets/b63ad1b6-5fb8-4e16-881a-b009d401c3fe" />

</br>

To disable logging less than specified alert level, use this function:

```c
void gablog_set_level(LogLevel level);
```

<div align="center">

# Profiling

</div>

```c
for (;;)
{
    GABPROFILER_CLEAR();

    GABPROFILE_SCOPE("Big frame")
    {
        GABPROFILE_SCOPE("Small Frame")
        {
            GABPROFILE_SCOPE("Test A") {}
            GABPROFILE_SCOPE("Test B") {}
        }

        GABPROFILE_SCOPE("Small Frame2")
        {
            GABPROFILE_SCOPE("Test A1") {}
            GABPROFILE_SCOPE("Test B2") {}
        }
    }

    GABPROFILER_PRINT();
}
```

Example Output:

<img width="390" height="175" alt="{E44A902D-94E7-493D-87C8-CC8BA6AB2774}" src="https://github.com/user-attachments/assets/5030f46d-5500-4f12-bc47-7466272d4132" />

</br>

The profiler is frame-based, which avoids GPU stalls and keeps performance consistent.

```text
Frame N     → record timings
Frame N+1   → GPU processing
Frame N+2   → resolve results
```

Adjust these macros to your needs:

```c
#define GABMAX_QUERIES_PER_FRAME 256
#define GABQUERY_LATENCY 3
```

<div align="center">

# Installation

</div>

1. Copy `gabdebug.h` into your project

2. In **one** source file:

```c
#define GABDEBUG_IMPLEMENTATION
#define GABPROFILER_IMPLEMENTATION
#include "gabdebug.h"
```

3. In all other files:

```c
#include "gabdebug.h"
```

Libraries are enabled by default. You can enable or disable them using the following macros.

```c
#define GABDEBUG_ENABLE false
#define GABPROFILER_ENABLE false
```

To enable GPU profiling, select the graphics API you are using:

```c
#define GAB_GL
```

Stripped-prefix versions are enabled by default. In case of naming conflicts, use the following defines:

```c
#define GABDEBUG_UNSTRIP_PREFIX
#define GABPROFILER_UNSTRIP_PREFIX
```


