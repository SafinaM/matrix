//
// Created by mar on 02.10.2022.
//
#include "../include/MatrixException.h"

MatrixException::MatrixException(const std::string& msg) : m_message(msg) {};

const char* MatrixException::what() const noexcept {
	return m_message.c_str();
}
