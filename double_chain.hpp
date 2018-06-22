//
// Created by dy2018 on 18-6-22.
//

#ifndef UNTITLED43_DOUBLE_CHAIN_HPP
#define UNTITLED43_DOUBLE_CHAIN_HPP
#include <memory>
template<typename T>
class double_chain;
template<typename T>
class double_chain_iterator;
template<typename T>
class _data
{
    friend class double_chain<T>;
    friend  class double_chain_iterator<T>;
public:
    template<typename U>
    _data(U&& value):save_data(std::forward<U>(value))
    {

    }
private:
    T save_data;
    std::shared_ptr<_data> before;
    std::shared_ptr<_data> after;
};
template<typename T>
class double_chain_iterator
{
public:
    double_chain_iterator(std::weak_ptr<_data<T>> point_pos):_begin(point_pos),_now_pos(point_pos)
    {

    }
    double_chain_iterator& operator++()
    {
        if(_now_pos.lock()!= nullptr)
        {
            _now_pos = _now_pos.lock()->after;
        }
        return *this;
    }
    double_chain_iterator& operator++(int)
    {
        if(_now_pos.lock()!= nullptr)
        {
            _now_pos = _now_pos.lock()->after;
        }
        return *this;
    }
    double_chain_iterator& operator--()
    {
        if(_now_pos.lock()!= nullptr)
        {
            _now_pos = _now_pos.lock()->before;
        }
        return *this;
    }
    T& operator*()
    {
        return _now_pos.lock()->save_data;
    }
    bool operator!=(const double_chain_iterator& iter)
    {
        return (_now_pos.lock()!=iter._now_pos.lock());
    }
private:
    std::weak_ptr<_data<T>> _begin;
    std::weak_ptr<_data<T>> _now_pos;
};
template<typename T>
class double_chain
{
public:
    double_chain() = default;
    ~double_chain()
    {
        std::weak_ptr<_data<T>> weak_end = _head->before;
        std::weak_ptr<_data<T>> _before_chain = weak_end.lock()->before;
        _head->before.reset();
        _before_chain.lock()->after.reset();
        while(_before_chain.lock()!=_head)
        {
            _before_chain.lock()->after.reset();
            std::weak_ptr<_data<T>> tmp = _before_chain.lock()->before;
            tmp.lock()->after.reset();
            _before_chain = tmp;
        }
        _head.reset();
    }
public:
    void push_back(const T& value)
    {
        if(_head== nullptr)
        {
            _head = std::make_shared<_data<T>>(value);
            _now_weak = _head;
        }else{
            auto tmp_ptr = std::make_shared<_data<T>>(value);
            auto now_ptr = _now_weak.lock();
            if(now_ptr!= nullptr)
            {
                now_ptr->after = tmp_ptr;
                tmp_ptr->before = now_ptr;
                _now_weak = tmp_ptr;
            }
            _head->before = tmp_ptr;
        }
        ++_size;
    }
    std::size_t size() const
    {
        return _size;
    }
    T& next()
    {
        if(_read_weak.lock()==nullptr)
        {
            _read_weak = _head;
        }
        auto share_point =  _read_weak.lock();
        if(share_point!= nullptr)
        {
            _read_weak = share_point->after;
            return share_point->save_data;
        }
        return _out_of_range_data;
    }
    T& before()
    {
        if(_read_weak.lock()== nullptr)
        {
            _read_weak = _head;
        }
        if(_read_weak.lock()==_head)
        {
            return _out_of_range_data;
        }else{
            _read_weak = _read_weak.lock()->before;
            return _read_weak.lock()->save_data;
        }
    }
    double_chain_iterator<T> begin()
    {
        return double_chain_iterator<T>(_head);
    }
    double_chain_iterator<T> end()
    {
        return double_chain_iterator<T>(_head->before->after);
    }
private:
    T _out_of_range_data;
    std::size_t  _size = 0;
private:
    std::shared_ptr<_data<T>> _head;
    std::weak_ptr<_data<T>> _now_weak;
    std::weak_ptr<_data<T>> _read_weak;
};
#endif //UNTITLED43_DOUBLE_CHAIN_HPP
