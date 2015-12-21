#pragma once
#include "types.hpp"
#include "expression.hpp"
#include "value.hpp"

#include <array>
#include <functional>
#include <vector>

namespace conflang {

struct binary_overload {
	virtual ~binary_overload();

	virtual bool applicable(value * lhs, value * rhs) const = 0;

	virtual value::ptr call(value * lhs, value * rhs) const = 0;
};

struct unary_overload {
	virtual ~unary_overload();

	virtual bool applicable(value *) const = 0;

	virtual value::ptr call(value *) const = 0;
};

class engine {
private:
	std::array<std::vector<binary_overload>, binary_operator_expression::type_count> binary_overloads_;
	std::array<std::vector<unary_overload>,  unary_operator_expression::type_count>  unary_overloads_;

public:
	std::vector<unary_overload> const & unary_overloads(unary_operator_expression::operator_t type) const {
		return unary_overloads_[std::size_t(type)];
	}

	std::vector<unary_overload> & unary_overloads(unary_operator_expression::operator_t type) {
		return unary_overloads_[std::size_t(type)];
	}

	std::vector<binary_overload> const & binary_overloads(binary_operator_expression::operator_t type) const {
		return binary_overloads_[std::size_t(type)];
	}

	std::vector<binary_overload> & binary_overloads(binary_operator_expression::operator_t type) {
		return binary_overloads_[std::size_t(type)];
	}
};

}
