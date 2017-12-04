# Plusone

My personal utility library used in my proprietary projects

# Requirements

* **C++14**: Robust support for most language features
* **fmtlib**: Great formatting library (already included as submodule)
* **doctest**: Unit testing library (already included as downloadable target)
* **openssl**: SSL library (optional)
* CMake 3.0 or later

# Building

Plusone is header-only and depends only on fmtlib which also header-only. To use Plusone in your project, simply copy the Plusone and fmtlib sources to your project's source tree. Then, edit your build scripts to add `include` directory to the list of paths checked by the C++ compiler when searching for includes.
Alternatively, bring Plusone into your Git repository using the `git submodule` command.

Plusone `#include` lines will look like this:
```cpp
#include <plusone/compiler.hpp>
#include <plusone/static_vector.hpp>
```

For the examples, Plusone provides build scripts for CMake:
```sh
mkdir build
cd build
cmake ..
make
```

For using Plusone in scope a CMake project:
```cmake
add_subdirectory(external/plusone EXCLUDE_FROM_ALL)
...
add_executable(app main.cpp)
target_link_libraries(app plusone)
```

# License

Distributed under the MIT License. (See accompanying file [LICENSE](LICENSE))
