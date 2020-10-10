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
 * \file exit_codes.hpp
 * \brief An header to define the exit codes of the tool.
 *
 * This header contains the definition of all the exit codes of the tool
 *
 * \author Andrea Esposito
 * \date April 10, 2020
 */

#ifndef EMOTIONS_EXIT_CODES_HPP
#define EMOTIONS_EXIT_CODES_HPP

/**
 * \brief A collection of all the exit codes of the tool.
 *
 * This enum contains all the (expected) exit codes of the tool.
 */
enum class exit_codes : int
{
    OK = 0, ///< The tool exited with no error completing its tasks.
    HALT = 1, ///< The tool exited with no error, but without completing its tasks.
    ARGUMENT_ERROR = 2, ///< The tool exited due to errors in the given arguments.
    UNKNOWN_ARGUMENT_ERROR = 3 ///< The tool exited due to unknown errors while parsing the arguments.
};

#endif //EMOTIONS_EXIT_CODES_HPP
