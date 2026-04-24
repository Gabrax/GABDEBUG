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

<div align="center">

### Output

</div>

```text
[INFO] Hello world (main.c:10)
[WARN] Warning! (main.c:11)
[ERROR] Error occurred (main.c:12)
```

<div align="center">

# Profiling

</div>

```c
for (;;)
{
    GABPROFILER_CLEAR();

    GABPROFILE_SCOPE("Frame")
    {
        // your code here
    }

    GABPROFILER_PRINT();
}
```

Enable OpenGL support:

```c
#define GAB_GL
#include <glad/glad.h>
```

Requirements:

* OpenGL context must be initialized
* Uses `GL_TIME_ELAPSED` queries
* Results appear after a few frames (latency buffering)


Example Output

```text
Frame: CPU 16.42 ms | GPU 14.87 ms
Update: CPU 5.10 ms | GPU 0.00 ms
Render: CPU 11.20 ms | GPU 14.80 ms
```


The profiler is **frame-based**:

```text
Frame N     → record timings
Frame N+1   → GPU processing
Frame N+2   → resolve results
```

This avoids GPU stalls and keeps performance consistent.

```c
#define gabMAX_QUERIES_PER_FRAME 256
#define gabQUERY_LATENCY 3
```

<div align="center">

# Installation

</div>

1. Copy `GABDEBUG.h` into your project

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

<div align="center">

### Enable / Disable systems

</div>

```c
#define GABDEBUG_ENABLE 0
#define GABPROFILER_ENABLE 0
```

* Completely removes code at compile time
* Zero runtime cost

<div align="center">

### Prefix Stripping (Optional)

</div>

Enable shorter macros:

```c
#define GABDEBUG_STRIP_PREFIX
#define GABPROFILER_STRIP_PREFIX
```


```c
GABDEBUG_set_level(LOG_WARN);
```