//
// Created by Jesse on 6/17/2015.
//

#ifndef MINIPROJECT1_DATA_PARSER_H
#define MINIPROJECT1_DATA_PARSER_H

#include <string>

enum emp_field {
    NONE,
    NAME,
    STATUS,
    SALARY,
    PAY_BASIS,
    POSITION_TITLE
};

class Employee {
public:
    // constructor
    Employee(std::string firstName = "", std::string lastName = "",
             int salary = 0, std::string positionTitle = "",
             std::string status = "", std::string payBasis = "") :
            firstName(firstName), surname(lastName), status(status), salary(salary),
            payBasis(payBasis), positionTitle(positionTitle){ }

    std::string &getFirstName();
    void setFirstName(std::string &name);

    std::string & getStatus();
    void setStatus(std::string &status);

    int getSalary();
    void setSalary(int salary);

    std::string & getPayBasis();
    void setPayBasis(std::string &payBasis);

    std::string & getPositionTitle();
    void setPositionTitle(std::string &positionTitle);

    std::string & getSurname();
    void setSurname(std::string &surname);

    std::string toString();

    std::string toStringHeader();

    bool isFilledIn();

private:
    std::string firstName;
    std::string surname;
    std::string status;
    int salary;
    std::string payBasis;
    std::string positionTitle;
};

#endif //MINIPROJECT1_DATA_PARSER_H
