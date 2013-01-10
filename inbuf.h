/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  <copyright holder> <email>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef INBUF_H
#define INBUF_H

#include <boost/asio.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/coroutine/all.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>


class inbuf : private boost::noncopyable
{
public:
    typedef boost::tuple< boost::system::error_code, std::size_t >                      tuple_t;
    typedef boost::coroutines::coroutine< void( boost::system::error_code, std::size_t) >     coro_t;
private:
    boost::asio::ip::tcp::socket      &   s_;
    coro_t                            &   coro_;
    coro_t::caller_type                  &   ca_;
public:
    inbuf(
        boost::asio::ip::tcp::socket & s,
        coro_t & coro,
        coro_t::caller_type & ca) :
        s_( s), coro_( coro), ca_( ca)
    { }
    
    template<typename MutableBufferSequence>
    std::size_t read_some(
        const MutableBufferSequence & buffers,
        boost::system::error_code & ec) 
    {
        s_.async_read_some(buffers, boost::bind(&coro_t::operator(), &coro_, _1, _2));
        ca_();
        
        std::size_t n = 0;
        boost::tie( ec, n) = ca_.get();
        return n;
    }
};


#endif // INBUF_H
