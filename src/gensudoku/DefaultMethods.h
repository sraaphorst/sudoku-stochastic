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

/// DO NOT USE DIRECTLY: standard class aliases
#ifndef __GENSUDOKUBOARD_POPULATOR_TYPE_ALIASES
#define __GENSUDOKUBOARD_POPULATOR_TYPE_ALIASES \
    using data_type = GenSudokuBoard<N>; \
    using pointer_type = std::unique_ptr<data_type>;
#endif

/// Standard operations with default constructor deleted
#ifndef __GENSUDOKUBOARD_POPULATOR_STANDARD_CONSTRUCTORS
#define __GENSUDOKUBOARD_POPULATOR_STANDARD_CONSTRUCTORS(classname) \
    classname() = delete; \
    classname(const classname&) = default; \
    classname(classname&&) = default; \
    classname &operator=(const classname&) = default; \
    classname &operator=(classname&&) = default; \
    virtual ~classname() = default;
#endif

/// DO NOT USE DIRECTLY: Standard constructors that accept a partial_board.
#ifndef __GENSUDOKUBOARD_POPULATOR_CONTENTS_CONSTRUCTORS
#define __GENSUDOKUBOARD_POPULATOR_CONTENTS_CONSTRUCTORS(classname, superclassname) \
    explicit classname(const data_type &partial_board): superclassname<N>{partial_board} {} \
    explicit classname(data_type &&partial_board): superclassname<N>{partial_board} {}
#endif

/// DO NOT USE DIRECTLY: Standard constructors that accept a partial_board and then invoke an initialize method.
#ifndef __GENSUDOKUBOARD_POPULATOR_CONTENTS_CONSTRUCTORS_WITH_INIT
#define __GENSUDOKUBOARD_POPULATOR_CONTENTS_CONSTRUCTORS_WITH_INIT(classname, superclassname) \
    explicit classname(const data_type &partial_board): superclassname<N>{partial_board} { initialize(); } \
    explicit classname(data_type &&partial_board): superclassname<N>{partial_board} { initialize(); }
#endif

/**
 * TO BE USED DIRECTLY.
 * This covers the constructors and basic methods that are needed by GenSudokuBoardPopulator subclassses that
 * initialize data structures.
 */
#define __GENSUDOKUBOARD_POPULATOR_CONSTRUCTORS_WITH_INIT(classname, superclassname) \
    __GENSUDOKUBOARD_POPULATOR_TYPE_ALIASES \
    __GENSUDOKUBOARD_POPULATOR_STANDARD_CONSTRUCTORS(classname) \
    __GENSUDOKUBOARD_POPULATOR_CONTENTS_CONSTRUCTORS_WITH_INIT(classname, superclassname)


/**
 * TO BE USED DIRECTLY.
 * This covers the constructors and basic methods that are needed by GenSudokuBoardPopulator subclassses that
 * do not initialize data structures.
 */
#define __GENSUDOKUBOARD_POPULATOR_CONSTRUCTORS(classname, superclassname) \
    __GENSUDOKUBOARD_POPULATOR_TYPE_ALIASES \
    __GENSUDOKUBOARD_POPULATOR_STANDARD_CONSTRUCTORS(classname) \
    __GENSUDOKUBOARD_POPULATOR_CONTENTS_CONSTRUCTORS(classname, superclassname)

/**
 * TO BE USED DIRECTLY.
 * Thhese omit the contents constructors, as they need unique parameters.
 */
#define __GENSUDOKUBOARD_POPULATOR_GENERIC_CONSTRUCTORS(classname) \
    __GENSUDOKUBOARD_POPULATOR_TYPE_ALIASES \
    __GENSUDOKUBOARD_POPULATOR_STANDARD_CONSTRUCTORS(classname)
