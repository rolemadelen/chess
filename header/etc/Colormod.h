/*
    Author: This piece of code does not belong to myself.	

    Purpose:  change text color in console.
          usage of "\033[": handle the console cursor            
*/

#ifndef COLORMOD_H
#define COLORMOD_H

#include <ostream>
namespace Color {
	enum Code {
		FG_RED     = 31,
		FG_GREEN   = 32,
		FG_BLUE    = 34,
		FG_DEFAULT = 39,
		BG_RED     = 41,
		BG_GREEN   = 42,
		BG_BLUE    = 44,
		BG_DEFAULT = 49
	};
	class Modifier {
	Code code;
	public:
		Modifier(Code pCode):code(pCode){}
		friend std::ostream&
		operator<<(std::ostream& os, const Modifier& mod) {
			return os << "\033["<<mod.code<<"m";
		}
	}; // end class
} // end namespace

#endif
