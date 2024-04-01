/*
 * menu01_tests.cpp
 *
 *  Created on: 15 sty 2024
 *      Author: Rafał
 */

#include <iostream>
#include <string>
#include <array>
#include <catch2/catch_test_macros.hpp>


 struct list1
{

};


template<auto M>
 struct menu_list1 : list1
{
    constexpr  menu_list1(const std::string_view (&name), const std::string_view (&s)[M]) :
            _name(name),
            _items(
                        { [s]() -> std::array<std::string_view, (M)>
                        {
                            return std::to_array<const std::string_view>(s);
                        }() })
    {
    }
    const  std::string_view _name;
    const  std::array<std::string_view, (M)> _items;
//    constexpr static std::string_view _name;
//    constexpr static std::array<std::string_view, (M)> _items;
};

template <class T>
class Menu01
{
public:
    Menu01( T &root_page) : _list(root_page)
    {

    }
    virtual ~Menu01(){}
    void MoveDown(void)
    {
        _current_index++;
    }
    const std::string_view CurrentItemStr(void)
    {
        return _list._items[_current_index];
    }

    const std::string_view PageName(void)
    {
        return _list._name;
    }
    unsigned int  CurrentItemInd(void)
    {
        return _current_index;
    }

    bool GoIn(void)
    {
//        if(_list._items[_current_index] != nullptr)
//            _current_index=0;
//
//        return _current_index;
        return true;
    }
    void Connect(int parent, list1 child)
    {

    }

    int item[100];
private:
     T _list;

    unsigned int _current_index=0;
    unsigned int parent_index=0;
};


TEST_CASE( "After initialization active item should be 0 (the first in list)")
{
    std::srand(std::time(nullptr)); // use current time as seed for random generator
    menu_list1 main_page("main page",{"item1","second item"});
//    Menu01 menu=Menu01(std::move(main_page));
    Menu01 menu=Menu01(main_page);

    CHECK(menu.CurrentItemStr()==std::string("item1"));
    CHECK(menu.CurrentItemInd()==0);
    CHECK(menu.PageName()==std::string("main page"));
}


TEST_CASE( "Moves down the list")
{
    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
    Menu01 menu=Menu01(main_page);
    menu.MoveDown();
    CHECK(menu.CurrentItemStr()==std::string("second item"));
    CHECK(menu.CurrentItemInd()==1);
}

TEST_CASE( "goes into sublist")
{
    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
    menu_list1 subpage=menu_list1("child",{"some1","second", "3"});
    Menu01 menu=Menu01(main_page);
    menu.Connect(menu.item[1],subpage);
    menu.MoveDown();
    menu.GoIn();
//    menu.MoveDown();
//    menu.MoveDown();
    CHECK(menu.CurrentItemStr()==std::string("3"));
    CHECK(menu.CurrentItemInd()==2);
    CHECK(menu.PageName()==std::string("child"));
}

//TEST_CASE( "reports an error on attemt to go into non-existingsublist")
//{
////    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
////    menu_list1 subpage=menu_list1("main page",{"some1","second", "3"});
////    Menu01 menu=Menu01(main_page);
////    menu.MoveDown();
////    menu.GoIn();
////    menu.MoveDown();
////    menu.MoveDown();
////    CHECK(menu.CurrentItemStr()==std::string("3"));
////    CHECK(menu.CurrentItemInd()==2);
////    CHECK(menu.CurrentItemInd()==2);
//}
