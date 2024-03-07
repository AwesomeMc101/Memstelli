#include "con_vm.hpp"


std::string VM::decode_instruction(INST instruction)
{
	switch (instruction)
	{
	case OP_ADD: return "ADD"; break;
	case OP_SUB: return "SUB";
	case OP_MUL: return "MUL";
	case OP_DIV: return "DIV";
	case OP_MOD: return "MOD";
	case OP_STRING: return "STRING";
	case OP_NEWMEM: return "NEWMEM";
	case OP_CALL: return "CALL";
	case OP_UNKNOWN: return "UNKNOWN";
	case OP_ENDLINE: return "ENDLINE";
	case OP_INT: return "INT";
	case OP_EDITMEM: return "EDITMEM";
	case OP_EQU: return "EQU";
	case OP_JMP: return "JMP";
	case OP_MEMREF: return "MEMREF";
	case OP_GETA: return "GETA";
	case OP_LETA: return "LETA";
	case OP_ENDIF: return "ENDIF";
	case OP_IF: return "IF";
	case OP_OPENPARA: return "OPENPARA";
	case OP_CLOSEPARA: return "CLOSEPARA";
	case OP_JMPLOC: return "JMPLOC";
	case OP_NOINSTRUCTION: return "NOINSTRUCTION";
	case OP_PUSHCALLSTACK: return "PUSHCALLSTACK";
	case OP_ARRAYIDX: return "ARRAYIDX";
	
	}
	return "OTHER";
}

void VM::execute(Instruction_Set* in_St, Stack::Stack* heap)
{
	static bool is_jumploc = false;
	static std::string jumploc_iden = "";
	bool jumploc_already_set = false;


	//static std::vector<bool> if_heap;

	/*if (in_St->instructions[0].first == NOTECALL)
	{
		return;
	}*/

	std::reverse(in_St->instructions.begin(), in_St->instructions.end()); //reverse interpretation

	Stack::Stack* stack = new Stack::Stack;
	for (int i = 0; i < in_St->instructions.size(); i++)
	{
		INST instr = in_St->instructions[i].first;

		INST old_instr = OP_NOINSTRUCTION;
		if (i > 0) { old_instr = in_St->instructions[i - 1].first; } //old data is popped to the stack...
		std::string data = in_St->instructions[i].second;
		bool run = true;

		//decided to do if..else instead of switch as switches seem just too
		//wild for this LOL
		//	std::cout << "Decoded Instruction: " << decode_instruction(instr) << " & Data: " << data << " & Old Instr: " << decode_instruction(old_instr) << "\n";
		
		/* IF Statements */
		if (instr == OP_JUMPLOCEND)
		{
			is_jumploc = false;
		}
		else if (is_jumploc)
		{
			//std::cout << "Writing jmp.\n";
			//std::cout << "Push instr: " << decode_instruction(instr) << "\n";
			if (!jumploc_already_set) {
				heap->push_jump(jumploc_iden, in_St);
				jumploc_already_set = true;
			}
			if (instr == OP_JUMPLOCEND)
			{
				is_jumploc = false;
			}
			run = false;
		}

		if (heap->ifheap_size()) {
			if (heap->pop_if() == false)
			{
				
				//do not run
				//std::cout << "\nHeap pop is false\n";
				run = false;
			
					if(instr == OP_ENDIF)
					{
						heap->popback_if();
						continue;
					}
			}
		}
		if (!run) { continue; }

		if (instr == OP_OPENPROC)
		{
			printf("OPENPROC");
			std::cout << "POPPED: " << stack->pop() << "\n";
			int pid = CMemory::Functions::get_pid_by_name(stack->pop().c_str());
			if (!pid)
			{
				stack->push("PID FAILURE.");
				continue;
			}
			HANDLE proc = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, pid);
			if (!proc || proc == INVALID_HANDLE_VALUE)
			{
				stack->push("HANDLE FAILURE.");
				continue;
			}
			else
			{
				printf("\n-----Process opened.\n");
				CloseHandle(proc); //never said it was fast.
				heap->current_process = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, pid);
			}
			
		}
		else if (instr == OP_READMEMORY)
		{
			std::string addr = stack->pop();
			std::string val = CMemory::MemLib::readMemory(heap->current_process, (void*)std::stoi(addr, 0, 16));
			stack->push(val);
			std::cout << "\nMemory read: " << val << "\n";
		}
		else if (instr == OP_CLOSEPROC)
		{
			stack->push(std::to_string(CloseHandle(heap->current_process)));
		}
		else if (instr == OP_WRITEMEMORY)
		{
			//writememory 0x5555 50;
			std::cout << "write memory;";
			std::string addr = stack->pop();
			DWORD value = std::stoul(stack->pop_index(1));

			CMemory::MemLib::writeMemory(heap->current_process, value, (void*)std::stoi(addr, 0, 16));

		}
		else if (instr == OP_CALL)
		{
			//call cfunction w/ stack
			stack->push(CFunction::do_cfunction(data, stack->pop(), stack->pop_index(1))); //data is like "print"
		}
		else if (instr == OP_NEWARR)
		{
			//array varname 5 = 
			stack->dbg_printstack();
			std::string arrayName = stack->pop();
			std::string size = stack->pop_index(1);
			int arr_size = 0;
			if (!Arith::is_num(size))
			{
				stack->push("Array size not a valid integer.");
			}
			else {
				arr_size = ceil(std::stod(size));
			}
			std::vector<std::string> array_values{};
			for (int ff = 1; ff < (arr_size+1); ff++)
			{
				int to_index = ff + 1;//idx 1 is arr size
				array_values.push_back(stack->pop_index(to_index));
			}
			heap->push_array(arrayName, array_values);
		}
		else if (instr == OP_EXCLA)
		{
			if (old_instr == OP_EQU)
			{
				in_St->instructions[i].first = OP_NEQU;
			}
		}
		else if (instr == OP_MEMDEL)
		{
			heap->delete_var(stack->pop());
		}
		else if (instr == OP_PUSHCALLSTACK)
		{
			stack->push(stack->get_callstack_return());
		}
		else if (instr == OP_JMP) //function call (defined function, jump to call)
		{
			std::string iden = stack->pop();
			std::vector<Instruction_Set*> in_St_T = heap->return_jump_instructions(iden);

			stack->pop_back(); //get rid of identifier from stack

			heap->push_var("arg1", stack->pop());
			heap->push_var("arg2", stack->pop_index(1));


			/* push the return value to the stack at the end */
			Instruction_Set* callstack = new Instruction_Set;
			callstack->instructions.push_back(std::make_pair(OP_PUSHCALLSTACK, ""));
			in_St_T.push_back(callstack);

			std::reverse(in_St_T.begin(), in_St_T.end());
		//	in_St_T.push_back(std::make_pair(OP_PUSHCALLSTACK, ""));


			for (int z = 0; z < in_St_T.size(); z++)
			{
				//printf("\n---NEWZ\n");
				std::reverse(in_St_T[z]->instructions.begin(), in_St_T[z]->instructions.end());
				for (int x = 0; x < in_St_T[z]->instructions.size(); x++)
				{
					in_St->instructions.insert(in_St->instructions.begin() + i+1, in_St_T[z]->instructions[x]);
				}
				//in_St->instructions
			}
		}
		else if (instr == OP_PUSHSTACK) //"return x", get_callstack_return() is called at end of JMP and pushes return value to stack
		{
			//stack->push(stack->pop()); //doesnt make much sense now... :D
			stack->set_callstack_return(stack->pop()); //gets called o
		}
		else if (instr == OP_JMPLOC)
		{
			//new jump
			heap->new_jump(stack->pop());
			jumploc_iden = stack->pop();
			is_jumploc = true;
		}

		else if (instr == OP_NEWMEM)
		{
			if (stack->stacksize() >= 1)
			{
				//std::string dbgstr = stack->pop();
				heap->push_var(stack->pop(), stack->pop_index(1));

				//std::cout << "new var " << " valued at " << heap->index_var(dbgstr) << "\n";
				heap->dbg_printvars();
			}
			else
			{
				//printf("var cannot be called");
			}
		}
		else if (instr == OP_INT)
		{
			stack->push(data);
			bool do_math = false;
			switch (old_instr) //check if we need to do a math call
			{
			case OP_ADD: do_math = true; break;
			case OP_SUB: do_math = true; break;
			case OP_MUL: do_math = true; break;
			case OP_DIV: do_math = true; break;
			case OP_MOD: do_math = true; break;
			}
			//if (!do_math) //no math, push int to stack
			//{
			//	stack->push(data);
			//	continue;
			//}
			// math
			if (do_math) {
				stack->push(Arith::arith(stack->pop(), stack->pop_index(1), old_instr)); //push arith val to stack
			}
			else if ((old_instr == OP_DEQU) || /* Compare strings. */
				(old_instr == OP_LETA) ||
				(old_instr == OP_NEQU) ||
				(old_instr == OP_GETA))
			{
				stack->push(Compare::comp_integers(stack->pop(), stack->pop_index(1), old_instr));
			}
		}

		else if ((instr == OP_ADD) || (instr == OP_SUB) || (instr == OP_MUL) || (instr == OP_DIV) || (instr == OP_MOD))
		{
			if (!stack->stacksize()) //verify we have a stack value to index
			{
				if (!heap->stacksize())
				{
					stack->push("0");
				}
				else
				{
					stack->push(heap->pop());
				}
			}
		}
		else if (instr == OP_STRING)
		{
			stack->push(data);
			if (old_instr == OP_ADD)
			{
				stack->push(Concat::concat_str(stack->pop(), stack->pop_index(1))); 
			}

			else if ((old_instr == OP_DEQU) || /* Compare strings. */
				(old_instr == OP_LETA)		||
				(old_instr == OP_NEQU) ||
				(old_instr == OP_GETA)		)
			{
				stack->push(Compare::comp_strings(stack->pop(), stack->pop_index(1), old_instr));
			}
		}
		else if (instr == OP_EQU)
		{
	//	std::cout << "op equ\n";
			if (old_instr == OP_EQU)
			{
				//std::cout << "PUSH DEQU\n\n";
				in_St->instructions[i].first = OP_DEQU; //push double equal value
			}
		}
		else if (instr == OP_ARRAYIDX)
		{
			stack->push(data);
		}
		else if (instr == OP_MEMREF) //VAR CALLED print([varname]); would trigger this OPCODE
		{
			if (old_instr == OP_EQU)
			{
				heap->push_var(data, stack->pop());
			}
			else if (old_instr == OP_ARRAYIDX) //array
			{
				//printf("arr call");
				//in_St->instructions.erase(in_St->instructions.begin() + i);
				//old_instr = OP_NOINSTRUCTION;
				if (i > 1) { old_instr = in_St->instructions[i - 2].first; } //reset old instruction to be before ARRAYIDX
				int idx = std::stoi(stack->pop());


				stack->push(stack->pop_index(1));
				//why are we doing ^^? stack.pop is now the index value, we are bringing the old val back.
				stack->push(heap->index_array(data, idx));
				//std::cout << "index " << data << " @ " << idx << "\n";
			}
			else {
				stack->push(heap->index_var(data));
			}

		//	std::cout << "memref: " << heap->index_var(data) << "\n";

			bool do_math = false;
			switch (old_instr) //check if we need to do a math call
			{
			case OP_ADD: do_math = true; break;
			case OP_SUB: do_math = true; break;
			case OP_MUL: do_math = true; break;
			case OP_DIV: do_math = true; break;
			case OP_MOD: do_math = true; break;
			}
			if (do_math) {
				//std::cout << "DOMATH";
				if (!Arith::is_num(stack->pop()))
				{

					stack->push(Concat::concat_str(stack->pop(), stack->pop_index(1)));
				}
				else
				{
					//std::cout << "arith " << stack->pop() << " && " << stack->pop_index(1);
					stack->push(Arith::arith(stack->pop(), stack->pop_index(1), old_instr));
				}
			}
			else if ((old_instr == OP_DEQU) || /* Compare strings. */
				(old_instr == OP_LETA) ||
				(old_instr == OP_NEQU) ||
				(old_instr == OP_GETA))
			{
				if (!Arith::is_num(stack->pop()))
				{

					stack->push(Compare::comp_strings(stack->pop(), stack->pop_index(1), old_instr));
				}
				else
				{

					stack->push(Compare::comp_integers(stack->pop(), stack->pop_index(1), old_instr));
				}
				//stack->push(Compare::comp_integers(stack->pop(), stack->pop_index(1), old_instr));
			}
		}
		else if (instr == OP_IF)
		{
		
			std::cout << "OP_IF STACKPOP: " << stack->pop() << "\n";
			if (stack->pop().find("0") != std::string::npos)
			{
				//if_heap.push_back(false);
			//	printf("Pushed a false value.");
				heap->push_if(false);
			}
		}
		else if (instr == OP_UNKNOWN) //this is just a backup for a lot of the time now... most "fixes" in here are now fixed in parser 
		{
			//chekc for empty whitespace, if its just OP_UNKNOWN with a bunch of spaces we can disregard and set next loops old_instr to our old_instr (i-2 for them)
			bool not_empty = false;
			for (int i = 0; i < data.size(); i++)
			{
				if (data[i] != ' ') { not_empty = true; }
			}
			if (!not_empty)
			{
				in_St->instructions[i].first = old_instr;
				/* Why are we doing this? This code's data is a bunch of spaces. We need to be able to access the REAL old instruction,
				example:
				'Hello' + 'World'
				.......^This whitespace could cause us problems.*/
			}
			else
			{
				if (old_instr == OP_STRING) //meaning that on the old instruction, it had passed a value stating string was opened. may need
				//to verify that it wasnt a close string
				{
					stack->push(data);
				}
				else if (old_instr == OP_EQU)
				{
					stack->push(data); //var name
				}
			}
		}

	}
}