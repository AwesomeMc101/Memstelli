#pragma once
#include "include.h"
#define _CRT_SECURE_NO_WARNINGS
namespace Stack
{
	class Stack {
		std::vector<std::string> stack;
		std::map<std::string, std::string> var_stack;
		std::map <std::string, std::vector<Instruction_Set*>> jump_stack;
		std::vector<bool> if_heap;
		std::string callstack_return_value = "0";
		std::map<std::string, std::vector<std::string>> array_stack;
		
	public:
		//std::map<INST, std::string> arith_callstack;
		//std::vector<std::pair<INST, std::string>> arith_callstack;
		HANDLE current_process;

		void push(std::string data);
		std::string pop();
		std::string pop_index(unsigned int idx); //pop with size()-idx index 
		void pop_back();
		size_t stacksize();
		void push_var(std::string iden, std::string value);
		std::string index_var(std::string iden);
		void delete_var(std::string iden);

		void push_if(bool ifval);
		bool pop_if();
		void popback_if();
		size_t ifheap_size();

		void push_array(std::string iden, std::vector<std::string> intial_values);
		std::string index_array(std::string iden, int idx);

		void new_jump(std::string iden);
		void push_jump(std::string iden, Instruction_Set* in_St);
		std::vector<Instruction_Set*> return_jump_instructions(std::string iden);
		void set_callstack_return(std::string val);
		std::string get_callstack_return();

		void dbg_printvars();
		void dbg_printstack();
	};
}