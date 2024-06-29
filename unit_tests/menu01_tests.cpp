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
#include <type_traits>
#include <typeinfo>
#include <functional>

 struct list1
{
//     std::array<list1*, 1> child;
    virtual list1* Get_child(unsigned int index)
    {
        return nullptr;
    }
    virtual list1* Get_parent(void)
    {
        return nullptr;
    }
    const std::string_view _name;
    const std::array<std::string_view, 1> _items;


    virtual const std::string_view Get_name(void)
    {
        return "bb";
    }

    virtual const std::string_view Get_item(unsigned int index)
    {
        return "aa";
    }
    virtual const unsigned int Get_max(void)
    {
        return 0;
    }
    virtual const unsigned int Set_callback(void)//todo what about pure virtual function?
    {
        return 0;
    }

    virtual const unsigned int Get_parent_index(void) {return 0;};

    virtual  void Perform_clb(unsigned int index){}


 protected:
//    unsigned int max;
};

// concept  Fooable = ! std::is_same_v<const std::nullptr_t, T_Par>;


//template<auto M, typename  T_Par = const std::nullptr_t>
template<auto M, typename  T_Par = const std::nullptr_t>
 struct menu_list1 : list1
{
    constexpr  menu_list1(const std::string_view (&name), const std::string_view (&s)[M],T_Par * parent_menu=nullptr,unsigned int index=0) :
            _name(name),
            _items(
                        { [s]() -> std::array<std::string_view, (M)>//todo check const here
                        {
                            return std::to_array<const std::string_view>(s);
                        }() }),
                        parent((std::is_same_v<const std::nullptr_t, T_Par>) ? nullptr : parent_menu ),
                        _index(index)
    {
        if constexpr(std::is_same_v<const std::nullptr_t, T_Par> == false)
        {
            parent->child[index]=this;
        }
    };
    const  std::string_view _name;
    const  std::array<std::string_view, (M)> _items;
    std::array< void(*)(unsigned int) , (M)> _clb = {nullptr};
    T_Par *parent;
    unsigned int _index;

    std::array<list1*, (M)> child{};

    list1* Get_child(unsigned int index)
    {
        return child[index];
    }
    const  std::string_view Get_name(void)
    {
        return _name;
    }
    const  std::string_view Get_item(unsigned int index)
    {
        return _items[index];
    }
    virtual const unsigned int Get_max(void)
    {
        return _items.size();
    }
    virtual const unsigned int Get_parent_index(void)
    {
        return _index;
    }
    list1* Get_parent(void)
    {
        if constexpr (std::is_same_v<const std::nullptr_t, T_Par> == false)
        {
            return static_cast<list1*>(parent);
        }
        else
        {
            return nullptr;
        }
    }
    void Perform_clb(unsigned int index)
    {
        if(_clb[index] != nullptr)
        {
            _clb[index](index);
        }
    }

    void Register_callback(unsigned int index,void(*f)(unsigned int) )
    {
        _clb[index]=f;
    }

};

template <class T>
class Menu01
{
public:
    Menu01( T &&root_page) : _list(root_page),_current_list(_list)
    {
//        max_index
    }
    virtual ~Menu01()
    {
    }
    void MoveDown(void)
    {
        if (_current_index + 1 < _current_list->Get_max())
        {
            _current_index++;
            _current_list->Perform_clb(_current_index);
        }
    }
    void MoveUp(void)
    {
        if (_current_index > 0)
        {
            _current_index--;
            _current_list->Perform_clb(_current_index);
        }
    }
    const std::string_view CurrentItemStr(void) const
    {
        return _current_list->Get_item(_current_index); //_items[_current_index];
//        return _current_list->Get_Item(_current_index);
    }

    const std::string_view PageName(void) const
    {
        return _current_list->Get_name();
    }
    unsigned int  CurrentItemInd(void) const
    {
        return _current_index;
    }

    bool GoIn(void)
    {
        if(_current_list->Get_child(_current_index) != nullptr)
        {
            parent_index=_current_index;

            _parent_list=_current_list;
            _current_list=_current_list->Get_child(_current_index) ;

            _current_index=0;
            _current_list->Perform_clb(_current_index);
            return true;
        }
        return false;
    }
    bool GoOut(void)
    {
        if (_current_list->Get_parent() != nullptr)
        {
            _current_list = _parent_list;
            _parent_list = _current_list->Get_parent();
            _current_index = parent_index;
            parent_index = _current_list->Get_parent_index();
            _current_list->Perform_clb(_current_index);
            return true;
        }
        else
        {
            _parent_list = nullptr;
            return false;
        }
    }
    void GoTo(unsigned int index)
    {
        if (index < _current_list->Get_max())
        {
            _current_index = index;
            _current_list->Perform_clb(_current_index);
        }
    }

//    int item[100];
private:
     T _list;
     list1 *_current_list;
     list1 *_parent_list;

    unsigned int _current_index=0;
    unsigned int parent_index=0;
};


TEST_CASE( "After initialization active item should be 0 (the first in list)")
{
    std::srand(std::time(nullptr)); // use current time as seed for random generator
    menu_list1 main_page("main page",{"item1","second item"});
//    Menu01 menu=Menu01(std::move(main_page));
    Menu01 menu=Menu01(&main_page);

    CHECK(menu.CurrentItemStr()==std::string("item1"));
    CHECK(menu.CurrentItemInd()==0);
    CHECK(menu.PageName()==std::string("main page"));
}


TEST_CASE( "Moves down the list")
{
    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
    Menu01 menu=Menu01(&main_page);
    menu.MoveDown();
    CHECK(menu.CurrentItemStr()==std::string("second item"));
    CHECK(menu.CurrentItemInd()==1);
}

TEST_CASE( "goes into sublist")
{
    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
    [[maybe_unused]]   menu_list1 subpage=menu_list1("child",{"some1","second", "3"},&main_page,1);
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
    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
    Menu01 menu=Menu01(&main_page);
    menu.MoveDown();
    auto ret= menu.GoIn();

    CHECK(ret== false);
    CHECK(menu.CurrentItemStr()==std::string("second item"));
    CHECK(menu.CurrentItemInd()==1);
}

TEST_CASE( "Keeps at boundary index when trying to access more items than exists")
{
    menu_list1 main_page = menu_list1("main page",
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
    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
    [[maybe_unused]]  menu_list1 subpage=menu_list1("child",{"some1","second", "3"},&main_page,1);
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
    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
    [[maybe_unused]]  menu_list1 subpage=menu_list1("child",{"some1","second", "3"},&main_page,1);
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
    menu_list1 main_page=menu_list1("main page",{"item1","second item"});
    menu_list1 subpage=menu_list1("child",{"some1","second", "3"},&main_page,1);
    [[maybe_unused]]  menu_list1 detPage=menu_list1("child22",{"some1","second", "3"},&subpage,2);
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
    menu_list1 main_page=menu_list1("main page",{"item1","second item", "eh", "lala"});
    Menu01 menu=Menu01(&main_page);

    CHECK(menu.CurrentItemInd()==0);
    menu.GoTo(3);
    CHECK(menu.CurrentItemInd()==3);
}

unsigned int _in_clb_test=0;

void _test_calback(unsigned int i)
{
    _in_clb_test = i;
}

TEST_CASE( "When moving/stepping on Item perform callback funtion")
{
    menu_list1 main_page = menu_list1("main page",
    { "item1", "second item" });
    menu_list1 subpage = menu_list1("child",
    { "some1", "second", "3" }, &main_page, 1);
    Menu01 menu = Menu01(&main_page);

    main_page.Register_callback(1, &_test_calback);
    subpage.Register_callback(0, &_test_calback);
//    main_page.Register_callback(1,&_test_calback);
    subpage.Register_callback(2, &_test_calback);

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



//TODO clean namespace!!!
