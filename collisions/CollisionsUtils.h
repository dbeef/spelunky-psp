//
// Created by xdbeef on 07.10.18.
//

#ifndef SPELUNKYDS_COLLISIONS_UTILS_H
#define SPELUNKYDS_COLLISIONS_UTILS_H

//TODO Use initializer_list instead of variadic templates, because we expect arguments of same type. C++ Primer p. 291.

//!>variadic template function that recursively sets priority of a set of sprites

//function to end the recursion and set the last element
template<typename T>
void set_existance(bool existance, T *i) {
    i->exists = existance;
}

//this function will be called for all but the last element in the pack
template<typename T, typename ... Param>
void set_priority(bool existance, T *i, Param *... params) {
    i->exists = existance;
    set_existance(existance, params ...);
}


#endif //SPELUNKYDS_COLLISIONS_UTILS_H
