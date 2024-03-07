#include "con_compare.hpp"

std::string Compare::comp_strings(std::string str1, std::string str2, INST comptype)
{
	std::cout << "strcompare: " << str1 << " to " << str2 << "\n";
	switch (comptype)
	{
	case OP_DEQU:
		if (str1 == str2) { return "1"; }
		else { return "0"; }
		break;
	case OP_NEQU:
		if ((str1) != (str2)) { return "1"; }
		else { return "0"; }
		break;
	case OP_GETA:
		if (str1.length() > str2.length()) { return "1"; }
		else { return "0"; }
		break;
	case OP_LETA:
		if (str1.length() < str2.length()) { return "1"; }
		else { return "0"; }
		break;
	}
	return "0";
}

std::string Compare::comp_integers(std::string str1, std::string str2, INST comptype)
{
	std::cout << str1 << " comp to " << str2 << "\n";



	switch (comptype)
	{
	case OP_DEQU:
		//std::cout << "DEQU COMP";
		if (std::stod(str1) == std::stod(str2)) { return "1"; }
		else { return "0"; }
		break;
	case OP_NEQU:
		if (std::stod(str1) != std::stod(str2)) { printf("nequ 1"); return "1"; }
		else { printf("nequ 0"); return "0"; }
		break;
	case OP_GETA:
		//std::cout << "GETA COMP";
		if (std::stod(str1) > std::stod(str2)) { return "1"; }
		else { return "0"; }
		break;
	case OP_LETA:
		//std::cout << "LETA COMP";
		if (std::stod(str1) < std::stod(str2)) { return "1"; }
		else { printf("\nreturn 0;\n"); return "0"; }
		break;
	}
	return "0";
}