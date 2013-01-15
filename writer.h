/*
 *    <one line to give the program's name and a brief idea of what it does.>
 *    Copyright (C) 2013  hyq <i@hyq.me>
 * 
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef WRITER_H
#define WRITER_H
#include <boost/integer.hpp>
#include <boost/asio/buffer.hpp>

template <class OutBuffer>
void write_uint8(OutBuffer& buf, int value) {
    char val[1];
    val[0] = static_cast<boost::uint8_t>(value); 
    buf.write(boost::asio::buffer(val,1));
}

template <class OutBuffer>
void write_uint16(OutBuffer& buf, int value) {
    char val[2];
    val[0] = static_cast<boost::uint8_t>(value >> 8); 
    val[1] = static_cast<boost::uint8_t>(value);
    buf.write(boost::asio::buffer(val,2));
}

template <class OutBuffer>
void write_uint24(OutBuffer& buf, int value) {
    char val[3];
    val[0] = static_cast<boost::uint8_t>(value >> 16); 
    val[1] = static_cast<boost::uint8_t>(value >> 8);
    val[2] = static_cast<boost::uint8_t>(value);
    buf.write(boost::asio::buffer(val,3));
}

template <class OutBuffer>
void write_uint32(OutBuffer& buf, int value) {
    char val[4];
    val[0] = static_cast<boost::uint8_t>(value >> 24); 
    val[1] = static_cast<boost::uint8_t>(value >> 16);
    val[2] = static_cast<boost::uint8_t>(value >> 8);
    val[3] = static_cast<boost::uint8_t>(value);
    buf.write(boost::asio::buffer(val,4));
}
#endif