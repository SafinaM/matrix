//
// Created by mar on 02.10.2022.
//
#pragma once

#include <cstdint>

enum class Errors: uint32_t {
	InvalidSizes = 0,
	Ok
};

enum class DirectionOfConcatenation: uint32_t {
	Right = 0,
	Bottom
};
