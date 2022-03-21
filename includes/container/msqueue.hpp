// The Romi Project
//
// @ File   :  msqueue.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-21 15:44:26
//

#pragma once

#include <cds/intrusive/msqueue.h>
#include <cds/gc/hp.h>

namespace ci = cds::intrusive;
typedef cds::gc::HP hp_gc;

// MSQueue with Hazard Pointer garbage collector, base hook + item disposer:
struct Foo: public ci::msqueue::node<hp_gc>
{
};

// Disposer for Foo struct just deletes the object passed in
template<typename T>
struct MyDefaultDisposer {
    void operator()(T* p)
    {
        delete p;
    }
};

// Declare traits for the queue
template<typename T, typename Disposer = MyDefaultDisposer<T> >
struct myTraits: public ci::msqueue::traits {
    typedef ci::opt::hook<
        ci::msqueue::base_hook<ci::opt::gc<hp_gc> >
    > hook;
    typedef ci::opt::disposer<Disposer> disposer;
};

// At least, declare the queue type
template<typename T>
typedef ci::MSQueue< hp_gc, T, myTraits<T> > fooQueue;
// Example 2:
//  MSQueue with Hazard Pointer garbage collector,
//  member hook + item disposer + item counter,
//  without padding of internal queue data
//  Use msqueue::make_traits
struct Bar
{
    // Your data
    ...
    ci::msqueue::node< hp_gc > hMember;
};
typedef ci::MSQueue< hp_gc,
    Foo,
    typename ci::msqueue::make_traits<
        ci::opt::hook<
            ci::msqueue::member_hook<
                offsetof(Bar, hMember)
                ,ci::opt::gc<hp_gc>
            >
        >
        ,ci::opt::disposer< fooDisposer >
        ,cds::opt::item_counter< cds::atomicity::item_counter >
        ,cds::opt::padding< cds::opt::no_special_padding >
    >::type
> barQueue;
