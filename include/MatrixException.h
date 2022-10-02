//
// Created by mar on 02.10.2022.
//

#pragma once

#include <string>

struct MatrixException : std::exception {
	std::string m_message;

	MatrixException(const std::string& msg);

	const char* what() const noexcept override;
};