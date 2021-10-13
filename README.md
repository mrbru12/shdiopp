# shdiopp
A minimal header-only shader loader for OpenGL

```cpp
// Be sure to include shdiopp.h after a gl declaration header, such as glew!
#include <GL/glew.h>

#define SHDIOPP_IMPLEMENTATION
#include "shdiopp.h"

int main()
{
    ...

    shdiopp::Program program = shdiopp::Program::loadFile("res/shader/vertex.vert", "res/shader/fragment.frag");

    glUseProgram(program.getId());

    ...
}
```
