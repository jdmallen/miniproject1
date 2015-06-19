#include <iostream>
#include <stack>
#include <set>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <string>
#include <cstring>
#include <cstdlib>
#include "Lexer.h"
#include "data_parser.h"

int main(int argc, char *argv[]){

    std::string inFileName, outFileName;
    Lexer lexer;
    Token tok;
    std::string line;
    std::locale loc;

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
//                std::cout << "OPENING: " << latestTokenValue << std::endl;
                tags.push(latestTokenValue);    // Push it real good
            } else if (tok.type == TAG && latestTokenValue.compare(0,1,forwardSlash) == 0){ // If it's a closing tag
//                std::cout << "CLOSING: " << latestTokenValue << std::endl << "   LASTTAG: " << lastTag << std::endl;
                if (latestTokenValue.compare(1,latestTokenValue.size(),lastTag) == 0) { // and it closes the "top" tag
                    tags.pop();     // pop it like it's hot
                } else {
                    std::cerr << "Input file not well-formed: poorly nested tag(s)." << std::endl;
                    return 1;
                }
            }
            lastTag = (tags.size() > 0) ? tags.top() : "EMPTY";
        }
//        std::cout << "lastTag: " << lastTag << std::endl << "tags.size() = " << tags.size() << std::endl;
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
        int tempSal(0), nameCount(0), fieldCount(0);
        std::string fName, lName, tempPayBasis, tempStatus, tempTitle;

        for (int i = 1; i < tokens.size(); i++) {
            tok = tokens[i];
            std::string latestTokenValue = tok.value;

//            std::cout << latestTokenValue << std::endl;
            if (tok.type == TAG){
                if (latestTokenValue.compare(0,4,"name") == 0) {
//                std::cout << "Capturing name..." << std::endl;
                    whichField = NAME;
                }
//                if (latestTokenValue.compare(0,5,"/name") == 0) {
////                std::cout << "Done with name..." << std::endl;
//                    whichField = NONE;
//                }
                if (latestTokenValue.compare(0,6,"status") == 0) {
//                std::cout << "Capturing status..." << std::endl;
                    whichField = STATUS;
                }
//                if (latestTokenValue.compare(0,6,"/status") == 0) {
////                std::cout << "Done with status..." << std::endl;
//                    whichField = NONE;
//                }
                if (latestTokenValue.compare(0,6,"salary") == 0) {
//                std::cout << "Capturing salary..." << std::endl;
                    whichField = SALARY;
                }
//                if (latestTokenValue.compare(0,7,"/salary") == 0) {
////                std::cout << "Done with salary..." << std::endl;
//                    whichField = NONE;
//                }
                if (latestTokenValue.compare(0,9,"pay_basis") == 0) {
//                std::cout << "Capturing pay basis..." << std::endl;
                    whichField = PAY_BASIS;
                }
//                if (latestTokenValue.compare(0,10,"/pay_basis") == 0) {
////                std::cout << "Done with pay basis..." << std::endl;
//                    whichField = NONE;
//                }
                if (latestTokenValue.compare(0,14,"position_title") == 0) {
//                std::cout << "Capturing position title..." << std::endl;
                    whichField = POSITION_TITLE;
                }
//                if (latestTokenValue.compare(0,9,"/position") == 0) {
////                std::cout << "Done with position title..." << std::endl;
//                    whichField = NONE;
//                }
                if (latestTokenValue.compare(0,4,"/row") == 0) {
//                std::cout << "Done with position title..." << std::endl;
                    whichField = NONE;
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
//                        std::cout << "<< found a name IDENT >>" << std::endl;
                        if (nameCount == 0){
                            lName = latestTokenValue;
                            lName.pop_back();
//                            emp->setSurname(lName);
                            nameCount++;
//                            fieldCount++;
                        } else {
                            fName += latestTokenValue + " ";
//                            emp->setFirstName(fName);
//                            fieldCount++;
                        }
                        break;
                    case STATUS:
//                        std::cout << "<< found a status IDENT >>" << std::endl;
                        tempStatus += latestTokenValue;
//                        emp->setStatus(tempStatus);
//                        fieldCount++;
                        break;
                    case SALARY:
//                        std::cout << "<< found a salary IDENT >>" << std::endl;
                        tempSal = (int)std::strtol(tok.value.c_str(),0,10);
//                        emp->setSalary(tempSal);
//                        fieldCount++;
                        break;
                    case PAY_BASIS:
//                        std::cout << "<< found a pay basis IDENT >>" << std::endl;
                        tempPayBasis += latestTokenValue + " ";
                        break;
//                        emp->setPayBasis(tempPayBasis);
//                        fieldCount++;
                    case POSITION_TITLE:
//                        std::cout << "<< found a title IDENT >>" << std::endl;
                        tempTitle += latestTokenValue + " ";
//                        emp->setPositionTitle(tempTitle);
//                        fieldCount++;
                        break;
                }
            }


            if (/*fieldCount >= 6*/emp->isFilledIn()){

                emps.push_back(*emp);
//                std::cout << "Added employee, count: " << emps.size() << std::endl;

                fName = "";
                lName = "";
                tempPayBasis = "";
                tempStatus = "";
                tempTitle = "";
                nameCount = 0; //fieldCount = 0;
                emp = new Employee();
            } else {
//                std::cout << "Incomplete employee" << std::endl;
            }

        } // for

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
