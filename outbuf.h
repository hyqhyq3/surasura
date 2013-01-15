/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  hyq <i@hyq.me>

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


#ifndef OUTBUF_H
#define OUTBUF_H

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/coroutine/all.hpp>
#include <boost/bind.hpp>

class outbuf : private boost::noncopyable
{
public:
    typedef boost::tuple< boost::system::error_code, std::size_t >                      tuple_t;
    typedef boost::coroutines::coroutine< void( boost::system::error_code, std::size_t) >     coro_t;
private:
    boost::asio::ip::tcp::socket      &   s_;
    coro_t                            &   coro_;
    coro_t::caller_type                  &   ca_;
public:
    outbuf(
        boost::asio::ip::tcp::socket & s,
        coro_t & coro,
        coro_t::caller_type & ca) :
        s_( s), coro_( coro), ca_( ca)
        { }
        
        template<typename MutableBufferSequence>
        std::size_t write(
            const MutableBufferSequence & buffers) 
        {
            boost::system::error_code ec;
            std::size_t n;
            n = write(buffers, ec);
            if(ec) {
                throw ec;
            }
            return n;
        }
        
        template<typename MutableBufferSequence>
        std::size_t write(
            const MutableBufferSequence & buffers,
            boost::system::error_code & ec) 
        {
            boost::asio::async_write(s_, buffers, boost::bind(&coro_t::operator(), &coro_, _1, _2));
            ca_();
            
            std::size_t n = 0;
            boost::tie( ec, n) = ca_.get();
            return n;
        }
};

#endif // OUTBUF_H
