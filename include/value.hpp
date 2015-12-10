#pragma once
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace conflang {

class value {
public:
	using ptr  = std::unique_ptr<value>;
	using cptr = std::unique_ptr<value const>;

	virtual ~value();

	virtual char const * type() const = 0;

	virtual value::ptr clone() const = 0;

	virtual bool callable(std::vector<value::cptr> const &) { return false; }

	virtual value::ptr call(std::vector<value::cptr> const &) { return nullptr; }
};

class null : public value {
	char const * type() const override { return "null"; }

	value::ptr clone() const override {
		return value::ptr(std::make_unique<null>());
	}
};

template<typename T>
class literal : public value {
public:
	T value;

	literal(T const &  value) : value(value) {};
	literal(T       && value) : value(std::move(value)) {};

	char const * type() const override { return "literal"; }

	value::ptr clone() const override {
		return value::ptr(std::make_unique<literal<T>>(value));
	}

	operator T       &  ()       &  { return value; }
	operator T const &  () const &  { return value; }
	operator T       && ()       && { return std::move(value); }
};


class list : public value, public std::vector<value::ptr> {
public:
	using std::vector<value::ptr>::vector;

	char const * type() const override { return "list"; }

	value::ptr clone() const override {
		auto result = std::make_unique<list>();
		result->reserve(size());
		for (auto const & child : *this) result->emplace_back(child->clone());
		return std::move(result);
	}
};

class map : public value, public std::map<std::string, value::ptr> {
public:
	char const * type() const override { return "map"; }

	value::ptr clone() const override {
		auto result = std::make_unique<map>();
		for (auto const & entry : *this) result->emplace(entry.first, entry.second->clone());
		return std::move(result);
	}
};


}
