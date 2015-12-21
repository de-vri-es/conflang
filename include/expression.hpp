#pragma once
#include "forward.hpp"
#include "value.hpp"

#include <utility>
#include <string>
#include <vector>
#include <map>

namespace conflang {

class expression {
public:
	using ptr  = std::unique_ptr<expression>;
	using cptr = std::unique_ptr<expression const>;

	virtual ~expression();

	virtual value::ptr evaluate(engine const &, scope const &) = 0;
};

class literal_expression : public expression {
public:
	value::ptr literal;

	value::ptr evaluate(engine const &, scope const &) override {
		return literal->clone();
	}
};

class list_expression : public expression {
public:
	std::vector<expression::ptr> children;

	value::ptr evaluate(engine const & engine, scope const & scope) override;
};

class map_expression : public expression {
public:
	std::map<std::string, expression::ptr> elements;

	value::ptr evaluate(engine const & engine, scope const & scope) override;
};

class reference_expression : public expression {
public:
	std::string identifier;

	value::ptr evaluate(engine const & engine, scope const & scope) override;
};

class function_call_expression : public expression {
public:
	expression::ptr functor;
	std::vector<expression::ptr> arguments;

	value::ptr evaluate(engine const & engine, scope const & scope) override;
};

class unary_operator_expression : public expression {
public:
	enum class operator_t {
		logical_not,
		bitwise_complement,
		negate,
		nop,
	};

	static constexpr std::size_t type_count = std::size_t(operator_t::nop);

	operator_t type;

	expression::ptr operand;

	value::ptr evaluate(engine const &, scope const &) override;
};

class binary_operator_expression : public expression {
public:
	enum class operator_t {
		plus,
		minus,
		multiply,
		divide,
		remainder,
		compare_equal,
		compare_unequal,
		compare_less,
		compare_greater,
		compare_less_equal,
		compare_greater_equal,
		logical_and,
		logical_or,
		bitwise_and,
		bitwise_or,
		bitwise_xor,
		shift_left,
		shift_right,
		subscript,
	};

	static constexpr std::size_t type_count = std::size_t(operator_t::subscript);

	operator_t type;

	expression::ptr lhs;

	expression::ptr rhs;

	value::ptr evaluate(engine const &, scope const &) override;
};

}
