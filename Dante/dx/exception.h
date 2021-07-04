#pragma once
#include <exception>
#include <winerror.h>
#include <iomanip>
#include <sstream>
#include <system_error>

namespace dante::dx
{
    class COMException : public std::exception
    {
    public:
        COMException(HRESULT hr)
        {
            std::stringstream ss;
            ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << hr;
            message = ss.str() + " - " + std::system_category().message(hr);
        }

        const char* what() const override
        {
            return message.c_str();
        }

    private:
        std::string message;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
            throw COMException{ hr };
    }
}