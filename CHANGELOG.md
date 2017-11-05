# Version 20

* Added plusone::ignore_unused
* Changed static_vector

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

* Changed plusone::serialization
* Added serialization to std::array in plusone::serialization

# Version 15

* Updated signal interface
* Increased pow10 table till 20

# Version 14

* Added static_string
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
