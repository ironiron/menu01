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
//#include "catch.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"


 class menupage
 {

 };

struct item
{

    item(const std::string_view (&n)) : name(n)
    {}
   const  std::string_view name;
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
    }
    const std::string_view _name;
    const std::array<item, (M)> _items;
    const T_Par* parent;
    const  std::string_view  operator [] (int i) const {return _items[i].name;}
    const  std::string_view & operator [] (int i) {return _items[i].name;}
};

template <class T>
class Menu01
{
public:
    Menu01(T &root_page) : _list(root_page)
    {
     //   _current_index=0;
    }
    virtual ~Menu01(){}
    void MoveDown(void)
    {
        _current_index++;
    }
    const std::string_view CurrentItemStr(void)
    {
//        return "";
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

TEST_CASE( "After initialization active item should be 0 (or the first in list)")
{
    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
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

//int main()
//{
//    menu_list1 main_page=menu_list1({"item1","second item"});
//    Menu01 menu=Menu01(main_page);
//    return 0;
//}

//template<unsigned int size> void Create_List()
//{
//    const std::array<item,size> list;
//
//}
//
//void Link_Lists(item parent_node, int list)
//{
//    parent_node.next=list[0];
//}
//
///*
// * ptr to upper layer
// *
// * list of string and ptr to lower layers
// *
// *
// */
//
////void clb(int index)
////{
////    testing::menu01::index=index;
////}

//TEST_CASE( "After initialization callback should be called with index of active item")
//{
////    menu.item["main"]["sublist settings"].setCalback(foo);
////    Menu01::Create_Lis t<5>={"test1","t2","aaa","i4","5"};
////    Menu01 menu(menu_list);
////    Create_List<5> base={"i1","i2","i3","i4","i5"};
////    Menu01 menu(menu_list);
//
////    Create_List<base,nullptr,nullptr> up1={"gotobase1","hahah","eh"};
//
////    menu.Register_Callback();
//
////    std::string e="gasfd";
////    std::string e2="dsagasfd";
////    const std::array<std::string,2> a={"dada","dasd"};
////    menu_list("adad","das","ehh");//function initialization
////    menu_list eh={45,55,21};//struct initialization
////    menu_list eh={e,e2};//struct initialization
////    menu_list eh={";aa;",";bb;"};//struct initialization
//
//    menu_list1 eh=menu_list1({"1111111","2222222"});
//
//////    menu_list(134,1,5);
//////    std::cout<<"aaaaaaa"<<lala.items[0]<<std::endl;
////    std::cout<<" sds "<<a[0]<<std::endl;
//
////  REQUIRE(testing::menu01::index==3);
//}
