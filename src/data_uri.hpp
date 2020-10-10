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
 * \file data_uri.hpp
 * \brief An header to manage data URIs.
 *
 * This header contains useful declaration to manage data URIs.
 *
 * \author Andrea Esposito
 * \date April 9, 2020
 */

#ifndef EMOTIONS_DATA_URI_HPP
#define EMOTIONS_DATA_URI_HPP

#include <string>

/**
 * \brief A utility class to handle data URIs.
 *
 * This class represents a data URI. A data URI is defined by [MDN](https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/Data_URIs)
 * as a string with the followind sintax: `data:[<mediatype>][;base64],<data>`.
 */
class data_uri
{
private:
    std::string m_type;
    std::string m_data;

public:
    /**
     * \brief Check if a string is a data URI.
     *
     * The function checks if a string is in the format `data:<mediatype>;base64,<data>`.
     *
     * \param s The string to be checked
     * \return True if `s` is a data URI, false otherwise.
     */
    static bool is_data_uri(const std::string &s);

    /**
     * \brief The class constructor.
     *
     * This constructor creates a data_uri from a string.
     *
     * \param s The string representing the data uri.
     *
     * \throws data_uri::string_not_uri if `s` is not a valid data URI.
     */
    explicit data_uri(const std::string &s);

    /**
     * \brief Get the media type.
     *
     * This function returns the media type of the data URI.
     *
     * @return The media type (`<mediatype>` in `data:<mediatype>;base64,<data>`).
     */
    std::string get_type() const;

    /**
     * \brief Get the data.
     *
     * This function returns the data contained in the data URI.
     *
     * \return The data (`<data>` in `data:<mediatype>;base64,<data>`).
     */
    std::string get_data() const;

    /**
     * \brief Get the URI.
     *
     * This function returns the entire URI as a string.
     *
     * @return The URI as a string.
     */
    std::string get_uri() const;

    /**
     * \brief An exception raised if a string is not an URI.
     *
     * This exception is thrown if a string, assumed to be one, is not a data URI.
     */
    class string_not_uri : public std::exception
    {
        const char *what() const noexcept override;
    };
};


#endif //EMOTIONS_DATA_URI_HPP
