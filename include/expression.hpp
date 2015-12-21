#pragma once
#include "types.hpp"
#include "value.hpp"

#include <utility>
#include <vector>
#include <map>

namespace conflang {

expression * lookup_reference(scope const & scope, string_view identifier);
expression * lookup_loose_reference(scope const & original_scope, map_expression * local_scope, string_view identifier);
expression * lookup_unlinked_reference(map_expression * local_scope, string_view identifier);

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

class reference_expression : public expression {
public:
	/**
	 * The different types of references:
	 *
	 * normal:
	 *   A normal reference is bound tightly to a name from it's own (or parent) scope.
	 *   It will never bind to a name from another scope in inheritance expressions.
	 *
	 * loose:
	 *   Loose references are bound to a name from their own (or parent) scope,
	 *   but they can be rebound to another scope using inheritance expressions.
	 *
	 * unlinked:
	 *   Unlinked references do not refer to a name in their own (or parent) scope and
	 *   must always be rebound to a new scope using inheritance expressions.
	 *   Using unlinked references in anything but abstract maps is not permitted.
	 */
	enum class reference_t {
		normal,
		loose,
		unlinked,
	};

	string_view identifier;

	value::ptr evaluate(engine const & engine, scope const & scope) override;
};

class list_expression : public expression {
public:
	std::vector<expression::ptr> children;

	value::ptr evaluate(engine const & engine, scope const & scope) override;
};

class map_expression : public expression {
public:
	std::map<string_view, expression::ptr> elements;

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
