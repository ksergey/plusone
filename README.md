# Plusone

My personal utility library used in my proprietary projects

# Requirements

* **C++14**: Robust support for most language features
* CMake 3.0 or later

# Building

Plusone is header-only so there are no libraries to build or link with. To use Plusone in your project, simply copy the Plusone sources to your project's source tree. Then, edit your build scripts to add `include` directory to the list of paths checked by the C++ compiler when searching for includes.
Alternatively, bring Plusone into your Git repository using the `git submodule` command.

Plusone `#include` lines will look like this:
```
#include <plusone/compiler.hpp>
#include <plusone/static_vector.hpp>
```

For the examples, Plusone provides build scripts for CMake.
```
mkdir build
cd build
cmake ..
make
```

# License

Distributed under the MIT License. (See accompanying file [LICENSE](LICENSE))
