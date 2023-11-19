#ifndef H__EXCEPTION
#define H__EXCEPTION

#include <exception>

class SocketException : public std::exception {
public:
        SocketException() : std::exception() {}
        inline virtual char const* what() const noexcept {
                return why;
        }

private:
        char const* why = "Socket Exception";
};

class BindException : public SocketException {
public:
        BindException() : SocketException() {}

private:
        char const* why = "Bind Exception";
};

class ListenException : public SocketException {
public:
        ListenException() : SocketException() {}

private:
        char const* why = "Listen Exception";
};

class AcceptException : public SocketException {
public:
        AcceptException() : SocketException() {};

private:
        char const* why = "Accept Exception";
};



#endif

