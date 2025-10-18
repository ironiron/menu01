/*
 * menu01_tests.cpp
 *
 *  Created on: 15 sty 2024
 *      Author: Rafał
 */

//#include <catch2/catch_test_macros.hpp> // catch2 v3
#include <catch.hpp> // use this for single header catch2
#include <type_traits>
#include <typeinfo>
#include <functional>

#include "../src/Menu01.hpp"




TEST_CASE( "After initialization active item should be 0 (the first in list)")
{
    Menu01_base::menu_list main_page("main page",{"item1","second item"});
    Menu01 menu=Menu01(&main_page);

    CHECK(menu.CurrentItemStr()==std::string("item1"));
    CHECK(menu.CurrentItemInd()==0);
    CHECK(menu.PageName()==std::string("main page"));
}


TEST_CASE( "Moves down the list")
{
    Menu01_base::menu_list main_page=Menu01_base::menu_list("main page",{"item1","second item"});
    Menu01 menu=Menu01(&main_page);
    menu.MoveDown();
    CHECK(menu.CurrentItemStr()==std::string("second item"));
    CHECK(menu.CurrentItemInd()==1);
}

TEST_CASE( "goes into sublist")
{
    Menu01_base::menu_list main_page=Menu01_base::menu_list("main page",{"item1","second item"});
    [[maybe_unused]]   Menu01_base::menu_list subpage=Menu01_base::menu_list("child",{"some1","second", "3"},&main_page,1);
    Menu01 menu=Menu01(&main_page);
    menu.MoveDown();
    auto ret=menu.GoIn();
    CHECK(ret== true);
    menu.MoveDown();
    menu.MoveDown();
    CHECK(menu.CurrentItemStr()==std::string("3"));
    CHECK(menu.CurrentItemInd()==2);
    CHECK(menu.PageName()==std::string("child"));
}

TEST_CASE( "reports an error on attemt to go into non-existing sublist")
{
    Menu01_base::menu_list main_page=Menu01_base::menu_list("main page",{"item1","second item"});
    Menu01 menu=Menu01(&main_page);
    menu.MoveDown();
    auto ret= menu.GoIn();

    CHECK(ret== false);
    CHECK(menu.CurrentItemStr()==std::string("second item"));
    CHECK(menu.CurrentItemInd()==1);
}

TEST_CASE( "Keeps at boundary index when trying to access more items than exists")
{
    Menu01_base::menu_list main_page = Menu01_base::menu_list("main page",
    { "item1", "second item", "stay_here" });
    Menu01 menu = Menu01(&main_page);
    menu.MoveDown();
    menu.MoveDown();
    CHECK(menu.CurrentItemStr() == std::string("stay_here"));
    menu.MoveDown();
    CHECK(menu.CurrentItemStr() == std::string("stay_here"));

    menu.MoveUp();
    menu.MoveUp();
    menu.MoveUp();
    CHECK(menu.CurrentItemStr() == std::string("item1"));
    menu.MoveUp();
    CHECK(menu.CurrentItemStr() == std::string("item1"));


}

TEST_CASE( "Goes out of the sub list")
{
    Menu01_base::menu_list main_page=Menu01_base::menu_list("main page",{"item1","second item"});
    [[maybe_unused]]  Menu01_base::menu_list subpage=Menu01_base::menu_list("child",{"some1","second", "3"},&main_page,1);
    Menu01 menu=Menu01(&main_page);

    menu.MoveDown();

    auto ret=menu.GoIn();
    CHECK(ret== true);

    menu.MoveDown();//some dummy action
    menu.MoveDown();//some dummy action
    menu.GoOut();
    CHECK(menu.CurrentItemInd()==1);
    CHECK(menu.PageName()==std::string("main page"));
}

TEST_CASE( "Goes out of the sub list, and reports error if not possible")
{
    Menu01_base::menu_list main_page=Menu01_base::menu_list("main page",{"item1","second item"});
    [[maybe_unused]]  Menu01_base::menu_list subpage=Menu01_base::menu_list("child",{"some1","second", "3"},&main_page,1);
    Menu01 menu=Menu01(&main_page);

    menu.MoveDown();

    auto ret=menu.GoIn();
    CHECK(ret== true);

    menu.MoveDown();//some dummy action
    menu.MoveDown();//some dummy action
     menu.GoOut();
    ret = menu.GoOut();
    CHECK(ret== false);
    CHECK(menu.CurrentItemInd()==1);
    CHECK(menu.PageName()==std::string("main page"));
    menu.MoveUp();
    CHECK(menu.CurrentItemInd()==0);
}

TEST_CASE( "Goes in and out of 3 tier sublists")
{
    Menu01_base::menu_list main_page=Menu01_base::menu_list("main page",{"item1","second item"});
    Menu01_base::menu_list subpage=Menu01_base::menu_list("child",{"some1","second", "3"},&main_page,1);
    [[maybe_unused]]  Menu01_base::menu_list detPage=Menu01_base::menu_list("child22",{"some1","second", "3"},&subpage,2);
    Menu01 menu=Menu01(&main_page);

    menu.MoveDown();

    auto ret=menu.GoIn();
    CHECK(ret== true);

    menu.MoveDown();//some dummy action
    menu.MoveDown();//some dummy action
    CHECK(menu.CurrentItemInd()==2);

    ret=menu.GoIn();
    CHECK(ret== true);
    CHECK(menu.PageName()==std::string("child22"));


    ret =menu.GoOut();
    CHECK(ret== true);
    ret = menu.GoOut();
    CHECK(ret== true);
    CHECK(menu.CurrentItemInd()==1);
    CHECK(menu.PageName()==std::string("main page"));
}

TEST_CASE( "Go to specific item")
{
    Menu01_base::menu_list main_page=Menu01_base::menu_list("main page",{"item1","second item", "eh", "lala"});
    Menu01 menu=Menu01(&main_page);

    CHECK(menu.CurrentItemInd()==0);
    menu.GoTo(3);
    CHECK(menu.CurrentItemInd()==3);
}

TEST_CASE( "When moving/stepping on Item perform callback funtion")
{
    Menu01_base::menu_list main_page = Menu01_base::menu_list("main page",
    { "item1", "second item" });
    Menu01_base::menu_list subpage = Menu01_base::menu_list("child",
    { "some1", "second", "3" }, &main_page, 1);
    Menu01 menu = Menu01(&main_page);

    unsigned int _in_clb_test = 0;
    auto _test_calback = [&](unsigned int i) noexcept
    {
        _in_clb_test = i;
    };

    main_page.Register_callback(1, _test_calback);
    subpage.Register_callback(0, _test_calback);
    subpage.Register_callback(2, _test_calback);

    CHECK(_in_clb_test == 0);
    menu.MoveDown();
    CHECK(_in_clb_test == 1);

    menu.GoIn();
    CHECK(_in_clb_test == 0);

    menu.MoveDown();
    CHECK(_in_clb_test== 0);
    menu.MoveDown();
    CHECK(_in_clb_test== 2);

    menu.GoOut();
    CHECK(_in_clb_test== 1);
}

TEST_CASE( "Current list can be accessed")
{
    Menu01_base::menu_list main_page=Menu01_base::menu_list("main page",{"item1","second item", "3rd"});
    Menu01_base::menu_list subpage=Menu01_base::menu_list("child",{"inner1"},&main_page,0);
    Menu01 menu=Menu01(&main_page);

    CHECK(menu.GetListItem(0)==std::string("item1"));
    CHECK(menu.GetListItem(1)==std::string("second item"));
    CHECK(menu.GetListItem(2)==std::string("3rd"));

    CHECK(menu.GetMenuLength()==3);

    menu.GoIn();
    CHECK(menu.GetMenuLength()==1);
}

TEST_CASE( "If has parent list can return pointer to it")
{
    Menu01_base::menu_list main_page=Menu01_base::menu_list("main page",{"item1","second item"});
    Menu01_base::menu_list subpage=Menu01_base::menu_list("child",{"inner1","inner2", "inner3"},&main_page,1);
    Menu01_base::menu_list subsubpage=Menu01_base::menu_list("subchild",{"1","2", "3"},&subpage,0);
    Menu01 menu=Menu01(&main_page);

    menu.MoveDown();

    CHECK(menu.HasParent() == false);
    auto ret=menu.GoIn();
    CHECK(menu.HasParent() == true);

    CHECK(menu.GetParent()->Get_item(0) == std::string("item1"));
    CHECK(menu.GetParent()->Get_item(1) == std::string("second item"));

    menu.GoIn();
    CHECK(menu.HasParent() == true);

    CHECK(menu.GetParent()->Get_item(0) == std::string("inner1"));
    CHECK(menu.GetParent()->Get_item(1) == std::string("inner2"));
    CHECK(menu.GetParent()->Get_item(2) == std::string("inner3"));
}

