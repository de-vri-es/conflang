#pragma once

#include <engine.hpp>

namespace conflang {

template<typename T1, typename T2>
struct binary_overload_wrapper : binary_overload {
	std::function<value::ptr(T1 *, T2 *)> functor;

	bool applicable(value * left, value * right) override {
		return dynamic_cast<T1 *>(left) != nullptr && dynamic_cast<T2 *>(right) != nullptr;
	}

	value::ptr call(value * left, value * right) override {
		return functor(static_cast<T1 *>(left), static_cast<T2 *>(right));
	}
};

template<typename T1, typename T2>
std::unique_ptr<binary_overload> wrap_binary_operator(std::function<value::ptr (T1 *, T2 *)> functor) {
	return std::make_unique<binary_overload_wrapper<T1, T2>>(std::move(functor));
}

template<typename T>
struct unary_overload_wrapper : unary_overload {
	std::function<value::ptr(T *)> functor;

	bool applicable(value * operand) override {
		return dynamic_cast<T *>(operand) != nullptr;
	}

	value::ptr call(value * operand) override {
		return functor(static_cast< T*>(operand));
	}
};

template<typename T>
std::unique_ptr<binary_overload> wrap_unary_operator(std::function<value::ptr (T *)> functor) {
	return std::make_unique<unary_overload_wrapper<T>>(std::move(functor));
}

}
