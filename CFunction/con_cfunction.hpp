#include "include.h"
#include "con_math.hpp"
namespace CFunction
{

	void print(std::string value);
	
	int strlen(std::string value);
	double con_pow(std::string A, std::string B);
	std::string bool_parse(std::string boolean);
	std::string get_input();
	std::string randint(std::string MAX);

	std::string c_sqrt(std::string num);
	std::string c_cbrt(std::string num);

	std::string isadmin();

	std::string do_cfunction(std::string caller, std::string arg1, std::string arg2);
	

}