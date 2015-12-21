#include "expression.hpp"
#include "engine.hpp"

#include <string>

namespace conflang {

expression * lookup_reference(scope const & scope, string_view identifier) {
	for (auto i = scope.rbegin(); i != scope.rend(); ++i) {
		map_expression const & current = **i;
		auto hit = current.elements.find(identifier);
		if (hit != current.elements.end()) return hit->second.get();
	}

	return nullptr;
}

expression * lookup_loose_reference(scope const & original_scope, map_expression * local_scope, string_view identifier) {
	expression * local = lookup_unlinked_reference(local_scope, identifier);
	if (local) return local;
	return lookup_reference(original_scope, identifier);
}

expression * lookup_unlinked_reference(map_expression * local_scope, string_view identifier) {
	auto hit = local_scope->elements.find(identifier);
	if (hit == local_scope->elements.end()) return nullptr;
	return hit->second.get();
}

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

value::ptr reference_expression::evaluate(engine const & engine, scope const & scope) {
	expression * expression = lookup_reference(scope, identifier);
	if (!expression) throw std::runtime_error("undefined reference: " + std::string(identifier));
	return expression->evaluate(engine, scope);
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

value::ptr unary_operator_expression::evaluate(engine const & engine, scope const & scope) {
	value::ptr operand = this->operand->evaluate(engine, scope);

	std::vector<unary_overload const *> matches;
	for (unary_overload const & overload : engine.unary_overloads(type)) {
		if (overload.applicable(operand.get())) matches.push_back(&overload);
	}

	if (matches.empty())    throw std::runtime_error("no match for unary operator "      + std::to_string(int(type)) + " with type " + operand->type());
	if (matches.size() > 1) throw std::runtime_error("ambiguous call to unary operator " + std::to_string(int(type)) + " with type " + operand->type());

	return matches[0]->call(operand.get());
}

value::ptr binary_operator_expression::evaluate(engine const & engine, scope const & scope) {
	value::ptr lhs = this->lhs->evaluate(engine, scope);
	value::ptr rhs = this->rhs->evaluate(engine, scope);

	std::vector<binary_overload const *> matches;
	for (binary_overload const & overload : engine.binary_overloads(type)) {
		if (overload.applicable(lhs.get(), rhs.get())) matches.push_back(&overload);
	}

	if (matches.empty())    throw std::runtime_error("no match for binary operator "      + std::to_string(int(type)) + " with types " + lhs->type() + " and " + rhs->type());
	if (matches.size() > 1) throw std::runtime_error("ambiguous call to binary operator " + std::to_string(int(type)) + " with types " + lhs->type() + " and " + rhs->type());

	return matches[0]->call(lhs.get(), rhs.get());
}

}
