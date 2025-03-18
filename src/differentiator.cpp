#include "expressions/expressions.hpp"

#include <iostream>
#include <regex>
#include <stdexcept>
#include <unordered_map>

using VariableType = std::unordered_map<std::string, long double>;
using ComplexVariableType =
	std::unordered_map<std::string, std::complex<long double>>;

template <typename T, typename VarMap>
std::string run_task(
	Expression<T> expr, bool to_diff, bool to_eval,
	const std::string &diff_by, VarMap &values
) {
	std::stringstream oss;
	if (to_diff) {
		Expression<T> diff_expr = expr.diff(diff_by);
		oss << "Differentiated: " << diff_expr.to_string();
	}

	if (to_eval) oss << "Evaluated: " << expr.resolve_with(values);
	return oss.str();
}
int main(int argc, char* argv[]) {
	std::string expression_string, diff_by;
	bool eval_expr = false, diff_expr = false, use_complex = false;
	VariableType variables;
	ComplexVariableType complex_variables;

	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "--eval" || arg == "--diff") {
			if (++i >= argc)
				throw std::invalid_argument("No value specified for " + arg);
			expression_string = argv[i];
			diff_expr |= (arg == "--diff");
			eval_expr |= (arg == "--eval");
		} else if (arg == "--by") {
			if (++i >= argc)
				throw std::invalid_argument("No value specified for --by");
			diff_by = argv[i];
		} else if (arg.find("=") != std::string::npos) {
			auto pos = arg.find("=");
			std::string var_name = arg.substr(0, pos),
						val_str = arg.substr(pos + 1);
			variables[var_name] = std::stold(val_str);
		} else {
			throw std::invalid_argument("Unknown argument: " + arg);
		}
	}

	auto expression = Expression<>::from_string(expression_string);
	std::cout << run_task(
	    expression, diff_expr, eval_expr, diff_by, variables
	) << "\n";
	return 0;
}
