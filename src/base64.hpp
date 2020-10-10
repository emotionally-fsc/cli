/**
 * \file base64.hpp
 * \brief Encode and decode base64 string.
 * \version 1.01.00
 * \authors [René Nyffenegger](https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp),
 * modified and documented by Andrea Esposito.
 */

#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <string>

/**
 * \brief Namespace for dealing with base64 strings.
 *
 * This namespace contains utilities to deal with base64 strings.
 */
namespace base64
{
    /**
     * \brief Encode a string to base64.
     *
     * This function encodes a string to a base64 string.
     *
     * \param s The string to be encoded.
     * \return The encoded string.
     */
    std::string encode(const std::string &s);

    /**
     * \brief Encode a string to base64.
     *
     * This function encodes a string to a base64 string.
     *
     * \param s The string to be encoded.
     * \param len The length of the string `s`.
     * \return The encoded string.
     */
    std::string encode(unsigned char const *s, unsigned int len);

    /**
     * \brief Decode a base64 string.
     *
     * This function decodes a base64 string to a binary string.
     *
     * \param s The string to be decoded.
     * \return The decoded string.
     */
    std::string decode(std::string const &s);
}
#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */