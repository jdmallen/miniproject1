#include <iostream>
#include <stack>
#include <set>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include "Lexer.h"
#include "data_parser.h"

int main(int argc, char *argv[]){

    std::string inFileName, outFileName;
    Lexer lexer;
    Token tok;

    if (argc == 1){
        std::cout << "Please provide an input filename." << std::endl;
        std::cin >> inFileName;
        std::cout << "Please provide an output filename." << std::endl;
        std::cin >> outFileName;
    } else if (argc == 2){
        std::cerr << "Usage: ./data_parser <inputfile> <outputfile>" << std::endl;
        return 1;
    } else {
        inFileName = argv[1];
        outFileName = argv[2];
    }

    std::ifstream input (inFileName.c_str());
    std::ofstream output;
    if(!input.is_open()){
        std::cerr << "Cannot open input file." << std::endl;
        return 1;
    } else {
        output.open(outFileName.c_str());
        std::stringstream buffer;
        buffer << input.rdbuf();
        std::string fileContents(buffer.str());

        lexer.set_input(fileContents);
        std::vector<Token> tokens = lexer.tokenize();

//      ==================================
//      Check if input file is well-formed
//      ==================================

        std::stack<std::string> tags;
        Token firstToken;
        if (tokens.size() == 0){
            std::cerr << "Input file has no tokens." << std::endl;
            return 1;
        } else {
            firstToken = tokens[0];
            if (firstToken.type == TAG){
                tags.push(firstToken.value);
            } else {
                std::cerr << "Input file does not start with a tag." << std::endl;
                return 1;
            }
        }

        std::string lastTag = tags.top();
        std::string forwardSlash = "/";
        for (int i = 1; i < tokens.size(); i++) {
            tok = tokens[i];
            std::string latestTokenValue = tok.value;
            if (latestTokenValue.substr(0,3).compare(0,3,"row") == 0){
                latestTokenValue = "row";
            }
            if (tok.type == TAG && latestTokenValue.compare(0,1,forwardSlash) != 0){ // If it's a new open tag
                tags.push(latestTokenValue);    // Push it real good
            } else if (tok.type == TAG && latestTokenValue.compare(0,1,forwardSlash) == 0){ // If it's a closing tag
                if (latestTokenValue.compare(1,latestTokenValue.size(),lastTag) == 0) { // and it closes the "top" tag
                    tags.pop();     // pop it like it's hot
                } else {
                    std::cerr << "Input file not well-formed: poorly nested tag(s)." << std::endl;
                    return 1;
                }
            }
            lastTag = (tags.size() > 0) ? tags.top() : "EMPTY";
        }
        if (tags.size() != 0 || lastTag.compare(0,lastTag.size(),"EMPTY") != 0){
            std::cerr << "Input file not well-formed: unmatched tag(s)." << std::endl;
            return 1;
        }

        std::cout << "File's good. Creating text table..." << std::endl;

//      ======================
//      Build employee objects
//      ======================

        std::vector<Employee> emps;
        while (!tags.empty()){
            tags.pop();
        }
        
        firstToken = tokens[0];
        tags.push(firstToken.value);

        Employee *emp = new Employee();

        emp_field whichField;
        int tempSal(0), nameCount(0);
        std::string tempName, fName, lName, tempPayBasis, tempStatus, tempTitle;

        for (int i = 1; i < tokens.size(); i++) {
            tok = tokens[i];
            std::string latestTokenValue = tok.value;

            if (tok.type == TAG){
                if (latestTokenValue.compare(0,4,"name") == 0) {
                    whichField = NAME;
                }
                if (latestTokenValue.compare(0,6,"status") == 0) {
                    whichField = STATUS;
                }
                if (latestTokenValue.compare(0,6,"salary") == 0) {
                    whichField = SALARY;
                }
                if (latestTokenValue.compare(0,9,"pay_basis") == 0) {
                    whichField = PAY_BASIS;
                }
                if (latestTokenValue.compare(0,14,"position_title") == 0) {
                    whichField = POSITION_TITLE;
                }
                if (latestTokenValue.compare(0,4,"/row") == 0) {
                    whichField = NONE;

                    std::size_t pos = tempName.find_last_of(",");
                    lName = tempName.substr(0,pos);
                    if (lName.size () > 0)  lName.resize (lName.size () - 1);
                    fName = tempName.substr(pos + 1);

                    emp->setSurname(lName);
                    emp->setFirstName(fName);
                    emp->setStatus(tempStatus);
                    emp->setSalary(tempSal);
                    emp->setPayBasis(tempPayBasis);
                    emp->setPositionTitle(tempTitle);
                }
            }

            if (tok.type == IDENT){
                switch(whichField){
                    case NAME:
                        tempName += latestTokenValue + " ";
                        break;
                    case STATUS:
                        tempStatus += latestTokenValue;
                        break;
                    case SALARY:
                        tempSal = (int)std::strtol(tok.value.c_str(),0,10);
                        break;
                    case PAY_BASIS:
                        tempPayBasis += latestTokenValue + " ";
                        break;
                    case POSITION_TITLE:
                        tempTitle += latestTokenValue + " ";
                        break;
                }
            }

            if (emp->isFilledIn()){
                emps.push_back(*emp);
                fName = "";
                lName = "";
                tempPayBasis = "";
                tempStatus = "";
                tempTitle = "";
                tempName = "";
//                nameCount = 0;
                emp = new Employee();
            }
        }

        output << emps[0].toStringHeader() << std::endl;
        output << "====================================================================================================================="
            << std::endl;
        for (int x = 0; x < emps.size(); x++){
            output << emps[x].toString() << std::endl;
        }

        output.close();
    }

    return 0;
}



void Employee::setFirstName(std::string &name) {
    Employee::firstName = name;
}

std::string &Employee::getStatus() {
    return status;
}

void Employee::setStatus(std::string &status) {
    Employee::status = status;
}

int Employee::getSalary()  {
    return salary;
}

void Employee::setSalary(int salary) {
    Employee::salary = salary;
}

std::string &Employee::getPayBasis() {
    return payBasis;
}

void Employee::setPayBasis(std::string &payBasis) {
    Employee::payBasis = payBasis;
}

std::string &Employee::getPositionTitle() {
    return positionTitle;
}

void Employee::setPositionTitle(std::string &positionTitle) {
    Employee::positionTitle = positionTitle;
}

std::string &Employee::getSurname() {
    return surname;
}

void Employee::setSurname(std::string &surname) {
    Employee::surname = surname;
}

std::string Employee::toString() {
    std::ostringstream out;
    out << std::left  << std::setw(20) << firstName     << " ";
    out << std::left  << std::setw(20) << surname       << " ";
    out << std::left  << std::setw(10) << status        << " ";
    out << std::right << std::setw(10) << salary        << " ";
    out << std::left  << std::setw(10) << payBasis      << " ";
    out << std::left  << std::setw(10) << positionTitle << " ";
    return out.str();
}

std::string Employee::toStringHeader() {
    std::ostringstream out;
    out << std::left  << std::setw(20) << "FIRST_NAME"     << " ";
    out << std::left  << std::setw(20) << "SURNAME"        << " ";
    out << std::left  << std::setw(10) << "STATUS"         << " ";
    out << std::right << std::setw(10) << "SALARY"         << " ";
    out << std::left  << std::setw(10) << "PAY_BASIS"      << " ";
    out << std::left  << std::setw(10) << "POSITION_TITLE" << " ";
    return out.str();
}

bool Employee::isFilledIn() {
    return    firstName.length() > 0 && surname.length() > 0
           && status.length() > 0    && salary > 0
           && payBasis.length() > 0  && positionTitle.length() > 0;
}

std::string &Employee::getFirstName() {
    return firstName;
}
