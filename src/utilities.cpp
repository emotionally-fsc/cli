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
 * \file utilities.cpp
 * \brief Implementation of utilities.hpp.
 *
 * \author Andrea Esposito
 * \date April 8, 2020
 */

#include <iostream>
#include <regex>
#include <fstream>
#include <algorithm>

#include <boost/program_options.hpp>

#include "utilities.hpp"
#include "data_uri.hpp"

exit_codes
setup_options(int argc, char **argv, std::string &video, unsigned int &framerate, std::string &classifier_path)
{
    namespace po = boost::program_options;

    po::options_description options("Available options");
    options.add_options()
            ("help,h", "Display this help message")
            ("affdex,a", po::value<std::string>(&classifier_path), "The Affdex classifier path")
            ("framerate,f", po::value<unsigned int>(&framerate), "The analysis framerate");

    po::options_description hidden("Hidden options");
    hidden.add_options()("video", po::value<std::string>(&video), "The video to be analyzed");

    po::positional_options_description arguments;
    arguments.add("video", -1);

    po::options_description all_options("All options");
    all_options.add(options);
    all_options.add(hidden);

    po::variables_map args = nullptr;
    try
    {
        po::store(po::command_line_parser(argc, argv).options(all_options).positional(arguments).run(), args);
        po::notify(args);

        if (args.count("help"))
        {
            std::cout << "Usage: " << argv[0] << " [options] VIDEO" << std::endl
                      << "Analyze the emotions of a video using Affectiva." << std::endl
                      << std::endl
                      << options << std::endl
                      << "Copyright (C) 2020 Andrea Esposito. Released under the GNU GPL v3 License." << std::endl;
            return exit_codes::HALT;
        }
        else if (!args.count("video"))
        {
            throw po::error("You must specify a video!");
        }

    }
    catch (po::error &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl
                  << std::endl;
        std::cerr << "For help, use the -h option." << std::endl
                  << std::endl;
        return exit_codes::ARGUMENT_ERROR;
    }
    catch (...)
    {
        std::cerr << "Unknown error!\n";
        return exit_codes::UNKNOWN_ARGUMENT_ERROR;
    }

    return exit_codes::OK;
}
