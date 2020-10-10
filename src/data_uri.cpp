/*
 * The tool for the emotion analysis created for Andrea Esposito's Bachelor's Thesis.
 * Copyright (C) 2020 Andrea Esposito <a.esposito39@studenti.uniba.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file data_uri.cpp
 * \brief Implementation of data_uri.hpp
 *
 * \author Andrea Esposito
 * \date April 9, 2020
 */

#include "data_uri.hpp"

const std::string PROTOCOL = "data:", BASE64 = ";base64,";

bool data_uri::is_data_uri(const std::string &s)
{
    return s.find(PROTOCOL) == 0 && s.find(BASE64) != std::string::npos;
}

data_uri::data_uri(const std::string &s)
{
    if (!data_uri::is_data_uri(s)) throw data_uri::string_not_uri();

    std::size_t base64_position = s.find(BASE64);

    m_type = s.substr(PROTOCOL.size(), base64_position - PROTOCOL.size());
    m_data = s.substr(PROTOCOL.size() + m_type.size() + BASE64.size());
}

std::string data_uri::get_type() const
{
    return m_type;
}

std::string data_uri::get_data() const
{
    return m_data;
}

std::string data_uri::get_uri() const
{
    return PROTOCOL + m_type + BASE64 + m_data;
}

const char *data_uri::string_not_uri::what() const noexcept
{
    return "The given string isn't a Data URI";
}
