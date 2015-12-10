#pragma once

namespace conflang {

class engine;
class scope;

class value;
class null_value;
class map_value;
class list_value;
template<typename T> class literal_value;

class expression;
class literal_expression;
class map_expression;
class list_expression;
class reference_expression;
class function_call_expression;
class unary_operator_expression;
class binary_operator_expression;

}
