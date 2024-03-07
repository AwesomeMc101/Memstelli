#include "con_stack.hpp"

std::string Stack::Stack::pop()
{
	//std::cout << "OLDMAX: " << Stack::stack.size() << "\n";
	if (!stack.size())
	{
		return "nil";
	}
	std::string val = Stack::stack[Stack::stack.size() - 1];
	//Stack::stack.erase(Stack::stack.begin() + (Stack::stack.size() - 1));
	//Stack::stack.pop_back();
	//std::cout << "NEWMAX: " << Stack::stack.size() << "\n";
	return val; 
}

std::string Stack::Stack::pop_index(unsigned int idx) //top value is size()-1, - idx from this
{
	if (idx >= Stack::stack.size())
	{
		std::cout << "\n\nERROR: ATTEMPT AT INDEXING STACK OUT OF RANGE.\n\n";
		return "0";
	}
	std::string val = Stack::stack[Stack::stack.size() - 1 - idx];
	//Stack::stack.erase(Stack::stack.begin() + (Stack::stack.size() - 1 - idx));
	return val; //5 - 1 - 5
}

void Stack::Stack::dbg_printstack()
{
	for (std::string item : stack)
	{
		std::cout << "Stack Item: " << item << "\n";
	}
}

void Stack::Stack::push_array(std::string iden, std::vector<std::string> initial_values)
{
	std::cout << "NEw array " << iden << "\n";
	for (std::string cec : initial_values)
	{
		std::cout << "member: " << cec << "\n";
	}
	Stack::array_stack[iden] = initial_values;
}
std::string Stack::Stack::index_array(std::string iden, int idx)
{
	if((idx < 0) || (idx > Stack::array_stack[iden].size())) {return "0"; }
	return Stack::array_stack[iden][idx];
}

void Stack::Stack::push(std::string data)
{
	//std::cout << "pushstack: " << data << "\n";
	Stack::stack.push_back(data);
}

void Stack::Stack::push_var(std::string iden, std::string value)
{
	Stack::var_stack[iden] = value;
}


void Stack::Stack::pop_back()
{
	Stack::stack.pop_back();
}

size_t Stack::Stack::stacksize()
{
	return Stack::stack.size();
}

std::string Stack::Stack::index_var(std::string iden)
{
	return Stack::var_stack[iden];
}

void Stack::Stack::delete_var(std::string iden)
{
	Stack::var_stack.erase(iden);
}

void Stack::Stack::new_jump(std::string iden)
{
	Stack::jump_stack[iden] = {};
}
void Stack::Stack::push_jump(std::string iden, Instruction_Set* in_St)
{
	Stack::jump_stack[iden].push_back(in_St);
}

void Stack::Stack::set_callstack_return(std::string val)
{
	Stack::callstack_return_value = val;
}
std::string Stack::Stack::get_callstack_return()
{
	return Stack::callstack_return_value;
}

std::vector<Instruction_Set*> Stack::Stack::return_jump_instructions(std::string iden)
{
	return Stack::jump_stack[iden];
}

void Stack::Stack::push_if(bool ifval)
{
	Stack::if_heap.push_back(ifval);
}
bool Stack::Stack::pop_if()
{
	bool val = Stack::if_heap[Stack::if_heap.size() - 1];
	//Stack::stack.erase(Stack::stack.begin() + (Stack::stack.size() - 1 - idx));
	return val; //5 - 1 - 5
}
void Stack::Stack::popback_if()
{
	if (Stack::ifheap_size()) {
		Stack::if_heap.pop_back();
	}
}
size_t Stack::Stack::ifheap_size()
{
	return Stack::if_heap.size();
}


void Stack::Stack::dbg_printvars()
{/*
	for (auto a : Stack::var_stack)
	{
		std::cout << "Var " << a.first << " at " << a.second() << "\n";
	}*/
	for (std::map<std::string, std::string>::const_iterator it = Stack::var_stack.begin();
		it != Stack::var_stack.end(); ++it)
	{
		std::cout << it->first << " @ " << it->second << "\n";
	}
}