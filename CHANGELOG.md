# Version 20

* Added plusone::ignore_unused
* Replaced macroses \_\_likely -> LIKELY, becase UB (compilers reserve variable names started from double underscore)
* Refactored static_vector

# Version 19

* Added fixed_object_pool
* Added type traits
* Removed process
* Removed object_pool

# Version 18

* Clear repository

# Version 17

* Removed `commit` from plusone::net::mmap_rx::frame

    Now it called in `frame` destructor

# Version 16

* plusone::serialization refactoring
* plusone::serialization added ability to read/write std::array

# Version 15

* Updated signal interface
* Increased pow10 table till 20

# Version 14

* Added static string
* Added string_view

# Version 13

* Added static_buffer_base
* Fixed SSL tests build

# Version 12

* Fixed CMake build
* Fixed SSL stream

# Version 11

* Changed signal interface
* Added process management interface

# Version 10

* Added initial SSL stream support
* Code cleanup
* Changed CMakeLists.txt option names

# Version 9

* Added swap method to net::socket class
* Added noexcept keyword to move constructor and operator of class net::socket
* Fixed README.md

# Version 8

* Introduced tcp socket unit test
* Added CHANGELOG.md
