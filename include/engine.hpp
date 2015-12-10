#pragma once
#include "scope.hpp"
#include "expression.hpp"
#include "value.hpp"

#include <array>
#include <functional>
#include <vector>

namespace conflang {

struct binary_overload {
	virtual ~binary_overload();

	virtual bool applicable(value * left, value * right) = 0;

	virtual value::ptr call(value * left, value * right) = 0;
};

struct unary_overload {
	virtual ~unary_overload();

	virtual bool applicable(value *) = 0;

	virtual value::ptr call(value *) = 0;
};

class engine {
private:
	using unary_overload  = std::function<value::cptr (value::cptr)>;

	std::array<std::vector<binary_overload>, binary_operator::type_count> binary_overloads_;
	std::array<std::vector<unary_overload>,  unary_operator::type_count>  unary_overloads_;

public:
	std::vector<binary_overload> const & binary_overloads(binary_operator::operator_t type) const {
		return binary_overloads_[std::size_t(type)];
	}

	std::vector<binary_overload> & binary_overloads(binary_operator::operator_t type) {
		return binary_overloads_[std::size_t(type)];
	}
};

}
