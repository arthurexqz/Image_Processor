#pragma once

#include <exception>
#include <string>
#include <stdexcept>

class AppException : public std::runtime_error {
public:
    explicit AppException(const std::string& msg) : std::runtime_error(msg) {
    }
};

class NotEnoughArgumentsException : public AppException {
public:
    NotEnoughArgumentsException() : AppException("Недостаточно аргументов в CLI. Введите input и output файлы") {
    }
};

class CantOpenFile : public AppException {
public:
    explicit CantOpenFile(const std::string& msg)
        : AppException(static_cast<std::string>("не получается открыть файл ") + msg) {
    }
};

class NotBMP : public AppException {
public:
    explicit NotBMP(const std::string& msg) : AppException(static_cast<std::string>("Передаваемый файл не BMP") + msg) {
    }
};

class InvalidFilterNameException : public AppException {
public:
    explicit InvalidFilterNameException(const std::string& arg_name)
        : AppException("Некорректное имя фильтра: " + arg_name) {
    }
};