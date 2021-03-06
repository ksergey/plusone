/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_string_150817174547
#define KSERGEY_static_string_150817174547

#include <string>
#include <plusone/string_view.hpp>

namespace plusone {

/** Simple fixed length string */
template< std::size_t N >
class static_string
{
private:
    char storage_[N + 1u]; /* Include null terminator */
    std::size_t size_;

public:
    /** Construct empty string */
    static_string();

    /** Construct from another `static_string` */
    template< std::size_t M >
    static_string(const static_string< M >& s);

    /** Construct from `const char*` and count */
    static_string(const char* s, std::size_t count);

    /** Construct from null terminated string */
    static_string(const char* s);

    /** Construct from array */
    template< std::size_t M >
    static_string(const char (&s)[M]) noexcept;

    /** Construct from `std::string` */
    static_string(const std::string& s);

    /** Construct from `string_view` */
    static_string(const string_view& s);

    /** Construct from `std::initializer_list` of chars */
    static_string(std::initializer_list< char > il);

    /** @return Pointer to a range of characters */
    const char* data() const noexcept;

    /** @return Pointer to a range of characters */
    char* data() noexcept;

    /** @return `data()` */
    const char* c_str() const noexcept;

    /** @return The number of chars in the string */
    std::size_t size() const noexcept;

    /** @return The number of chars in the string */
    std::size_t length() const noexcept;

    /** @return True if `size() == 0` */
    bool empty() const noexcept;

    /** @return `N` */
    static constexpr std::size_t capacity() noexcept;

    /** @return `N` */
    static constexpr std::size_t max_size() noexcept;

    /**
     * Set new size of data and set '\0' at end
     * @param[in] new_size New size of data
     * @pre `new_size <= size`
     */
    void set_size(std::size_t new_size);

    /**
     * @return `*(data() + i)`
     * @throw std::out_of_range when i > size()
     */
    const char& at(std::size_t i) const;

    /**
     * @return `*(data() + i)`
     * @throw std::out_of_range when i > size()
     */
    char& at(std::size_t i);

    /**
     * @pre `i <= size()`
     * @return `*(data() + i)`
     */
    const char& operator[](std::size_t i) const noexcept;

    /**
     * @pre `i <= size()`
     * @return `*(data() + i)`
     */
    char& operator[](std::size_t i) noexcept;

    /** @note Equivalent to `(*this)[0]` */
    const char& front() const noexcept;

    /** @note Equivalent to `(*this)[0]` */
    char& front() noexcept;

    /**
     * @note Equivalent to `at(*this)[size() - 1]`
     * @pre `!empty()`
     */
    const char& back() const noexcept;

    /**
     * @note Equivalent to `at(*this)[size() - 1]`
     * @pre `!empty()`
     */
    char& back() noexcept;

    /** Clears contents of this string */
    void clear() noexcept;

    /**
     * Append charater to the end of this string
     * @throw std::out_of_range exception if no capacity
     */
    void push_back(char ch);

    /**
     * Removes the last char from the string.
     * @pre `!empty()`
     * @throw std::out_of_range if `empty()`
     */
    void pop_back();

    /**
     * Appends `count` chars from the specified char array.
     * @throw std::out_of_range if `size() + count > N`
     */
    static_string& append(const char* s, std::size_t count);

    /** @note Equivalent to `append(s, std::strlen(s))` */
    static_string& append(const char* s);

    /** @note Equivalent to `append(s.data(), s.size())` */
    template< std::size_t M >
    static_string& append(const static_string< M >& s);

    /** @note Equivalent to `append(s.data(), s.size())` */
    static_string& append(const std::string& s);

    /** @note Equivalent to `append(s.data(), s.size())` */
    static_string& append(const string_view& s);

    /**
     * Assign `count` characters from the specified char array.
     * @throw std::out_of_range if `count > N`
     */
    static_string& assign(const char* s, std::size_t count);

    /** @note Equivalent to `assign(s, std::strlen(s))` */
    static_string& assign(const char* s);

    /** @note Equivalent to `assign(s.data(), s.size())` */
    template< std::size_t M >
    static_string& assign(const static_string< M >& s);

    /** @note Equivalent to `assign(s.data(), s.size())` */
    static_string& assign(const std::string& s);

    /** @note Equivalent to `assign(s.data(), s.size())` */
    static_string& assign(const string_view& s);

    /** Convert to `std::string` */
    std::string to_string() const;

    /** Convert to `string_view` */
    string_view to_string_view() const;

    /** Convert to `string_view` */
    operator string_view() const;
};

/** Compare two strings */
template< std::size_t N, std::size_t M >
bool operator==(const static_string< N >& left, const static_string< M >& right) noexcept;

/** Compare two strings */
template< std::size_t N >
bool operator==(const static_string< N >& left, const std::string& right) noexcept;

/** Compare two strings */
template< std::size_t N >
bool operator==(const std::string& left, const static_string< N >& right) noexcept;

/** Compare two strings */
template< std::size_t N >
bool operator==(const static_string< N >& left, const string_view& right) noexcept;

/** Compare two strings */
template< std::size_t N >
bool operator==(const string_view& left, const static_string< N >& right) noexcept;

/** Compare two strings */
template< std::size_t N >
bool operator==(const static_string< N >& left, const char* right) noexcept;

/** Compare two strings */
template< std::size_t N >
bool operator==(const char* left, const static_string< N >& right) noexcept;

} /* namespace plusone */

namespace std {

/** `std::hash` support. */
template< std::size_t N >
struct hash< plusone::static_string< N > >
{
    std::size_t operator()(const plusone::static_string< N >& s) const;
};

} /* namespace std */

#include <plusone/impl/static_string.ipp>

#endif /* KSERGEY_static_string_150817174547 */
