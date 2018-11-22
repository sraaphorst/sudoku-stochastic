/**
 * DefaultMethods.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

/**
 * I probably shouldn't be doing this, but due to the depth of the class hierarchy, typing the same constructos
 * over and over again is getting severely tedious.
 */
#define CONSTRUCTORS_WITH_INIT(classname, superclassname, initializer) \
    using data_type = GenSudokuBoard<N>; \
    using pointer_type = std::unique_ptr<data_type> \
    classname() = delete; \
    classname(const classname&) = default; \
    classname(classname&&) = default; \
    classname &operator=(const classname&) = default; \
    explicit classname(const data_type &partial_board): superclassname{partial_board} { initializer(); } \
    explicit classname(data_type &&partial_board): superclassname{partial_board} { initializer(); } \
    virtual ~##classname() = default;

#define CONSTRUCTORS(classname, superclassname) \
    using data_type = GenSudokuBoard<N>; \
    using pointer_type = std::unique_ptr<data_type> \
    classname() = delete; \
    classname(const classname&) = default; \
    classname(classname&&) = default; \
    classname &operator=(const classname&) = default; \
    explicit classname(const data_type &partial_board): superclassname{partial_board} {} \
    explicit classname(data_type &&partial_board): superclassname{partial_board} {} \
    virtual ~##classname() = default;
