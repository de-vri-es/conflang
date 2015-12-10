#include "expression.hpp"

namespace conflang {

expression::~expression() {}

value::ptr list_expression::evaluate(engine const & engine, scope const & scope) {
	auto result = std::make_unique<list>();
	result->reserve(children.size());
	for (auto const & child : children) {
		result->emplace_back(child->evaluate(engine, scope));
	}
	return std::move(result);
}

value::ptr map_expression::evaluate(engine const & engine, scope const & scope) {
	auto result = std::make_unique<map>();
	for (auto const & entry : elements) {
		result->emplace(entry.first, entry.second->evaluate(engine, scope));
	}
	return std::move(result);
}

value::ptr function_call_expression::evaluate(engine const & engine, scope const & scope) {
	value::ptr functor = this->functor->evaluate(engine, scope);
	std::vector<value::cptr> arguments;
	arguments.reserve(this->arguments.size());
	for (auto const & argument : this->arguments) {
		arguments.emplace_back(argument->evaluate(engine, scope));
	}

	if (!functor->callable(arguments)) throw std::runtime_error("invalid function call");
	return functor->call(arguments);

}

}
