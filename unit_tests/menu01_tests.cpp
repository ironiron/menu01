/*
 * menu01_tests.cpp
 *
 *  Created on: 15 sty 2024
 *      Author: Rafał
 */


#define CATCH_CONFIG_NO_CPP11_TO_STRING
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include <array>
#include <iostream>


template<auto M>
struct menu_list1
{
    menu_list1(const std::string (&s)[M]) :
            items(
            { [s]() -> std::array<std::string, (M)>
            {
                return std::to_array<const std::string>(s);
            }() })
    {
        //const std::array<int,(sizeof...(nodes))> items={names...};
        //std::cout<<"aaaaaaa ="<<items[0]<<std::endl;
        std::cout << "aaaaaaa =" << items[0] << "  ads " << items[1]
                << std::endl;
    }
//    const std::array<int,(sizeof...(nodes))> items;
//    item* parent = nullptr;
    //const std::array<item,sizeof(nodes)> items={nodes...};
    const std::array<std::string, (M)> items;

    const std::string name;
};

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

TEST_CASE( "After initialization callback should be called with index of active item")
{
//    menu.item["main"]["sublist settings"].setCalback(foo);
//    Menu01::Create_Lis t<5>={"test1","t2","aaa","i4","5"};
//    Menu01 menu(menu_list);
//    Create_List<5> base={"i1","i2","i3","i4","i5"};
//    Menu01 menu(menu_list);

//    Create_List<base,nullptr,nullptr> up1={"gotobase1","hahah","eh"};

//    menu.Register_Callback();

//    std::string e="gasfd";
//    std::string e2="dsagasfd";
//    const std::array<std::string,2> a={"dada","dasd"};
//    menu_list("adad","das","ehh");//function initialization
//    menu_list eh={45,55,21};//struct initialization
//    menu_list eh={e,e2};//struct initialization
//    menu_list eh={";aa;",";bb;"};//struct initialization

    menu_list1 eh=menu_list1({"1111111","2222222"});

////    menu_list(134,1,5);
////    std::cout<<"aaaaaaa"<<lala.items[0]<<std::endl;
//    std::cout<<" sds "<<a[0]<<std::endl;

//  REQUIRE(testing::menu01::index==3);
}
