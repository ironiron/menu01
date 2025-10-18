/*
 * Menu01.hpp
 *
 *  Created on: 15 sty 2024
 *      Author: Rafał
 */
#ifndef MENU01_HPP_
#define MENU01_HPP_


#include <string>
#include <array>
#include <functional>

namespace Menu01_base
{

 struct list_base
{
    virtual list_base* Get_child(unsigned int index)= 0;
    virtual list_base* Get_parent(void)= 0;
    virtual const std::string_view Get_name(void)= 0;
    virtual const std::string_view Get_item(unsigned int index)= 0;
    virtual std::size_t Get_max(void) = 0;
    virtual unsigned int Get_parent_index(void)= 0;
    virtual  void Perform_clb(unsigned int index)= 0;

    virtual ~list_base() = default;
};

template<auto M, typename  T_Par = const std::nullptr_t>
 struct menu_list : list_base
{
     explicit constexpr  menu_list(const std::string_view (&name), const std::string_view (&s)[M],T_Par * parent_menu=nullptr,unsigned int index=0) :
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

    menu_list(const menu_list&) =delete;
    menu_list& operator=(const menu_list&) = delete;

    const  std::string_view _name;
    const  std::array<std::string_view, (M)> _items;
    std::array<std::function<void(unsigned int)>, (M)> _clb = {nullptr};
    T_Par *parent;
    unsigned int _index;

    std::array<list_base*, (M)> child{};

    list_base* Get_child(unsigned int index) override
    {
        return child[index];
    }
    const  std::string_view Get_name(void) override
    {
        return _name;
    }
    const  std::string_view Get_item(unsigned int index) override
    {
        return _items[index];
    }
    std::size_t Get_max(void) override
    {
        return _items.size();
    }
    unsigned int Get_parent_index(void) override
    {
        return _index;
    }
    list_base* Get_parent(void) override
    {
        if constexpr (std::is_same_v<const std::nullptr_t, T_Par> == false)
        {
            return static_cast<list_base*>(parent);
        }
        else
        {
            return nullptr;
        }
    }
    void Perform_clb(unsigned int index) override
    {
        if(_clb[index] != nullptr)
        {
            _clb[index](index);
        }
    }

    void Register_callback(unsigned int index,std::function<void(unsigned int)> f)
    {
        _clb[index]=std::move(f);
    }
};

 };

template <class T>
class Menu01
{
public:
    explicit Menu01( T &&root_page) : _list(root_page),_current_list(_list),_parent_list(nullptr)
    {
    }
    virtual ~Menu01()
    {
    }
    Menu01(const Menu01&) =delete;
    Menu01& operator=(const Menu01&) = delete;
    const std::string_view PageName(void) const
    {
        return _current_list->Get_name();
    }
    unsigned int  CurrentItemInd(void) const
    {
        return _current_index;
    }
    const std::string_view CurrentItemStr(void) const
    {
        return _current_list->Get_item(_current_index);
    }

    const std::string_view GetListItem(unsigned int index) const
    {
        return _current_list->Get_item(index);
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

    bool HasParent(void)
    {
        if (_current_list->Get_parent() != nullptr)
        {
            return true;
        }
        return false;
    }

    Menu01_base::list_base * GetParent(void)
    {
        return _current_list->Get_parent();
    }

    std::size_t GetMenuLength(void)
    {
        return _current_list->Get_max();
    }

private:
     T _list;
     Menu01_base::list_base *_current_list;
     Menu01_base::list_base *_parent_list;
    unsigned int _current_index=0;
    unsigned int parent_index=0;
};

#endif

