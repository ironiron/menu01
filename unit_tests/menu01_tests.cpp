/*
 * menu01_tests.cpp
 *
 *  Created on: 15 sty 2024
 *      Author: Rafał
 */

#include <iostream>
#include <string>
#include <array>

//#define CATCH_CONFIG_NO_CPP11_TO_STRING
//#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_RUNNER
//#include "catch.hpp"
//#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cstdlib>
#include <ctime>

 class menupage
 {

 };

struct item
{
    item(const std::string_view (&n)) : name(n)
    {}
    const std::string_view name;
    int * child;
};

namespace internal
{

template<std::size_t N, typename F, std::size_t... I>
const std::array<item, N> construct(F&& func, std::index_sequence<I...>)
{
    return std::array<item, N>{ { (static_cast<void>(I), func(I))... } };
}

template< std::size_t N, typename F>
const std::array<item, N> construct(F&& func)
{
    return construct<N>(std::forward<F>(func), std::make_index_sequence<N>());
}

}

template<auto M, typename  T_Par = const std::nullptr_t>
struct menu_list1
{
//    menu_list1( const menu_list1&& ) {std::cout<< "menulist Moved \n";};
//    menu_list1( const menu_list1& x )
//    {
////        _items(x._items);
//        std::cout<< "menulist copied \n";
//        std::cout<< "random_value ---"<<random_value<<std::endl;
//    };
//     ~menu_list1( ) {
//        std::cout<< "menulist deleted \n";
//        std::cout<< "random_value ---"<<random_value<<std::endl;
//    };
//    menu_list1( const menu_list1& ) = delete;
//    ~menu_list1()= delete;
    menu_list1(const std::string_view (&name), const std::string_view (&s)[M], T_Par &upper_level = nullptr) :
            _name(name),
                    _items(
                    { internal::construct<M>(
                    [s](int i) -> const item
                    {
                        return item(s[i]);
                    }) }),
            parent(upper_level)
    {
//        random_value= std::rand();
        std::cout<< "menulist Created \n";
//        std::cout<< "random_value ---"<<random_value<<std::endl;

    }
    const std::string_view _name;
    std::array<item, (M)> _items;
    const T_Par* parent;
//    const  std::string_view  operator [] (int i) const {return _items[i].name;}
//    const  std::string_view & operator [] (int i) {return _items[i].name;}
};

template <class T>
class Menu01
{
public:
    virtual ~Menu01( ) {std::cout<< "Menu01 deleted \n";};
    Menu01( T &root_page) : _list(root_page)
//    Menu01( T &root_page) : _list(std::move(root_page))
//    Menu01( T &root_page) : _list(std::forward<T>(&root_page))
    {
        std::cout<< "Menu01 Created \n";
//        std::cout<< "_list.random_value ---"<<_list.random_value<<std::endl;
     //   _current_index=0;
    }
//    virtual ~Menu01(){}
    void MoveDown(void)
    {
        _current_index++;
    }
    const std::string_view CurrentItemStr(void)
    {
        return _list._items[_current_index].name;
    }
    unsigned int  CurrentItemInd(void)
    {
        return _current_index;
    }

//    bool GoIn(void)
//    {
//        if(_list._items[_current_index].child != nullptr)
//            _current_index=0;
//
//        return _current_index;
//    }

private:
     T _list;
    unsigned int _current_index=0;
    //unsigned int parent_index=0;
};

//TODO

TEST_CASE( "After initialization active item should be 0 (the first in list)")
{
    std::srand(std::time(nullptr)); // use current time as seed for random generator
    menu_list1 main_page("main page",{"item1","second item"});
//    Menu01 menu=Menu01(std::move(main_page));
    Menu01 menu=Menu01(main_page);
    CHECK(menu.CurrentItemStr()==std::string("item1"));
    CHECK(menu.CurrentItemInd()==0);
}

//TEST_CASE( "After initialization callback should be called with index of active item")
//{
//    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
//    Menu01 menu=Menu01(main_page);
//    CHECK(menu.CurrentItemStr()==std::string("item1"));
//    CHECK(menu.CurrentItemInd()==0);
//}

//TEST_CASE( "Moves down the list")
//{
//    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
//    Menu01 menu=Menu01(main_page);
//    menu.MoveDown();
//    CHECK(menu.CurrentItemStr()==std::string("second item"));
//    CHECK(menu.CurrentItemInd()==1);
//}

//TEST_CASE( "goes into sublist")
//{
//    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
//    Menu01 menu=Menu01(main_page);
//    menu.MoveDown();
//    CHECK(menu.CurrentItemStr()==std::string("second item"));
//    CHECK(menu.CurrentItemInd()==1);
//}
