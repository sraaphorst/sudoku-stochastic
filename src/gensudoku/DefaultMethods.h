/**
 * DefaultMethods.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

/**
 * This is not exactly an idea will probably be met with much approval, but I am tired of writing the exact
 * same set of constructors and basic operators over and over again.
 */
#define __CONSTRUCTORS_WITH_INIT(classname, superclassname) \
public:\
    using data_type = GenSudokuBoard<N>; \
    using pointer_type = std::unique_ptr<data_type>; \
    classname() = delete; \
    classname(const classname&) = default; \
    classname(classname&&) noexcept = default; \
    classname &operator=(const classname&) = default; \
    explicit classname(const data_type &partial_board): superclassname{partial_board} { initialize(); } \
    explicit classname(data_type &&partial_board): superclassname{partial_board} { initialize(); } \
    virtual ~classname() = default;

#define CONSTRUCTORS(classname, superclassname) \
public:\
    using data_type = GenSudokuBoard<N>; \
    using pointer_type = std::unique_ptr<data_type> \
    classname() = delete; \
    classname(const classname&) = default; \
    classname(classname&&) = default; \
    classname &operator=(const classname&) = default; \
    explicit classname(const data_type &partial_board): superclassname{partial_board} {} \
    explicit classname(data_type &&partial_board): superclassname{partial_board} {} \
    virtual ~##classname() = default;
