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

class unary_operator : public expression {
public:
	enum class operator_t {
		logical_not        = 0,
		bitwise_complement = 1,
	};

	static constexpr std::size_t type_count = std::size_t(operator_t::bitwise_complement);

	operator_t type;

	expression::ptr operand;

	value::ptr evaluate(engine const &, scope const &) override;
};

class binary_operator : public expression {
public:
	enum class operator_t {
		plus                  =  0,
		minus                 =  1,
		multiply              =  2,
		divide                =  3,
		remainder             =  4,
		compare_equal         =  5,
		compare_unequal       =  6,
		compare_less          =  7,
		compare_greater       =  8,
		compare_less_equal    =  9,
		compare_greater_equal = 10,
		logical_and           = 11,
		logical_or            = 12,
		bitwise_and           = 13,
		bitwise_or            = 14,
		bitwise_xor           = 15,
		shift_left            = 16,
		shift_right           = 17,
		subscript             = 18,
	};

	static constexpr std::size_t type_count = std::size_t(operator_t::subscript);

	operator_t type;

	expression::ptr left;

	expression::ptr right;

	value::ptr evaluate(engine const &, scope const &) override;
};

}
