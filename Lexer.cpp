/**
 * *****************************************************************************
 * file name : Lexer.cpp
 * author    : Hung Q. Ngo
 * description: implementation of Lexer interface
 * *****************************************************************************
 */
#include "Lexer.h"
#include <iostream>
using namespace std;

/**
 * -----------------------------------------------------------------------------
 *  scan and return the next token
 *  cur_pos then points to one position right past the token
 *  the token type is set to ERRTOK on error, at that point the global state
 *  variable err will be set to true
 * -----------------------------------------------------------------------------
 */
Token Lexer::next_token() {
    Token ret;
    size_t last;

    if (in_err) {
        ret.type = ERRTOK;
        ret.value = "";
        return ret;
    }

    // if not in error state, the default token is the ENDTOK
    ret.type = ENDTOK;
    ret.value = "";

    if (has_more_token()) {
        last = cur_pos; // input_str[last] is a non-space char
        if (input_str[cur_pos] == '<') {
            cur_pos++;
            while (cur_pos < input_str.length() && input_str[cur_pos] != '>')
                cur_pos++;
            if (cur_pos < input_str.length()) {
                ret.type = TAG;
                ret.value = input_str.substr(last+1, cur_pos-last-1);
                cur_pos++; // move past the closing "
            } else {
                in_err = true;
                ret.type = ERRTOK;
                ret.value = "";
            }
        } else {
            while (cur_pos < input_str.length() &&
                   separators.find(input_str[cur_pos]) == string::npos &&
                   input_str[cur_pos] != '<') {
                cur_pos++;
            }
            ret.type  = IDENT;
            ret.value = input_str.substr(last, cur_pos-last);
        }
    }
    return ret;
}

/**
 * -----------------------------------------------------------------------------
 *  set a new input string, restart
 * -----------------------------------------------------------------------------
 */
void Lexer::set_input(string str) {
    input_str = str;
    restart();
}

/**
 * -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 */
bool Lexer::has_more_token() {
    while (cur_pos < input_str.length() && 
           separators.find(input_str[cur_pos]) != string::npos) {
        cur_pos++;
    }
    return (cur_pos < input_str.length());
}

/**
 * -----------------------------------------------------------------------------
 *  restart from the beginning, reset error states
 * -----------------------------------------------------------------------------
 */
void Lexer::restart() {
    cur_pos = 0;
    in_err = false;
}


/**
* -----------------------------------------------------------------------------
*  return a vector of all tokens left
*  we will not restart after tokenizing
* -----------------------------------------------------------------------------
*/
std::vector<Token> Lexer::tokenize()
{
	vector<Token> ret;
	Token tok;
	while (has_more_token()) {
		tok = next_token();
		ret.push_back(tok);
	}
	return ret;
}