#include "con_cfunction.hpp"

void CFunction::print(std::string value)
{
	std::cout << "\nCONSTELLI || PRINTING: " << value << "\n";
}

int CFunction::strlen(std::string value)
{
	return (value.length());
}

double CFunction::con_pow(std::string A, std::string B)
{
	//std::cout << "POW " << A << "^" << B << "\n";
	return (pow(std::stod(A), std::stod(B)));
}
std::string CFunction::bool_parse(std::string boolean)
{
	if (boolean == "1") { return "true"; }
	return "false";
}

std::string CFunction::c_sqrt(std::string num)
{
	if (!Arith::is_num(num)) { return "0"; }
	return std::to_string(sqrt(std::stod(num)));
}
std::string CFunction::c_cbrt(std::string num)
{
	if (!Arith::is_num(num)) { return "0"; }
	return std::to_string(cbrt(std::stod(num)));
}

std::string CFunction::get_input()
{
	//printf("calling getinput");
	std::cout << std::endl;
	std::string value = "";
	putchar('>');
	std::getline(std::cin, value);
	return value;
}

std::string CFunction::randint(std::string MAX)
{
	if (!Arith::is_num(MAX)) { return "0"; }

	int max_val = std::stoi(MAX);
	
	int to_return = rand() % max_val;
	return std::to_string(to_return);

}

std::string CFunction::isadmin()
{
	HANDLE token;
	OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token);
	if (token != INVALID_HANDLE_VALUE)
	{
		TOKEN_ELEVATION tok_ele;
		DWORD returned_size = sizeof(tok_ele);
		if (GetTokenInformation(token, TokenElevation, &tok_ele, sizeof(tok_ele), &returned_size))
		{
			if (tok_ele.TokenIsElevated)
			{
				return "1";
			}
		}
	}
	return "0";
}
std::string CFunction::do_cfunction(std::string caller, std::string arg1, std::string arg2)
{
	if (caller == "print") { CFunction::print(arg1); return "1"; }
	if (caller == "strlen") { return std::to_string(CFunction::strlen(arg1)); }
	if (caller == "pow") { return std::to_string(CFunction::con_pow(arg1, arg2)); }
	if (caller == "bool_parse") { return CFunction::bool_parse(arg1); }
	if (caller == "get_input") { return CFunction::get_input(); }
	if (caller == "abort") { abort(); }
	if (caller == "randint") { return CFunction::randint(arg1); }

	if (caller == "sqrt") { return CFunction::c_sqrt(arg1); }
	if (caller == "cbrt") { return CFunction::c_cbrt(arg1); }
	if (caller == "isadmin") { return CFunction::isadmin(); }

	//if(caller == "mem.deletevar") 
	//if(caller == ""){}
}