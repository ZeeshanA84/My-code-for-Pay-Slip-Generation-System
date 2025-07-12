#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>

using namespace std;

// Base Employee class
class Employee {
protected:
    int emp_no;
    string emp_name;
    string emp_add;
    string emp_dept;
    string emp_desg;

public:
    // Common functions for all employees
    virtual void getData() {
        cout << "\nEnter Employee Number: ";
        cin >> emp_no;
        cin.ignore(); // Clear input buffer
        cout << "Enter Employee Name: ";
        getline(cin, emp_name);
        cout << "Enter Employee Address: ";
        getline(cin, emp_add);
        cout << "Enter Employee Department: ";
        getline(cin, emp_dept);
        cout << "Enter Employee Designation: ";
        getline(cin, emp_desg);
    }

    virtual void displayData() {
        cout << "\nEmployee Number: " << emp_no;
        cout << "\nName: " << emp_name;
        cout << "\nAddress: " << emp_add;
        cout << "\nDepartment: " << emp_dept;
        cout << "\nDesignation: " << emp_desg;
    }

    int getEmpNo() { return emp_no; }
    string getEmpName() { return emp_name; }
};

// Permanent Employee class
class PermanentEmployee : public Employee {
private:
    float emp_basic;
    float emp_da;
    float emp_hra;
    float emp_ma;
    float emp_pf;
    float ptax;
    float itax;
    float gross;
    float net;

    // Static members for rates that apply to all permanent employees
    static float da_rate;
    static float hra_rate;
    static float ma_amount;
    static float ptax_amount;

public:
    void getData() override {
        Employee::getData();
        cout << "Enter Basic Salary: ";
        cin >> emp_basic;
        cout << "Enter Income Tax: ";
        cin >> itax;
        calculate();
    }

    void displayData() override {
        Employee::displayData();
        cout << "\nEmployee Type: Permanent";
        cout << "\nBasic Salary: " << emp_basic;
        cout << "\nDearness Allowance: " << emp_da;
        cout << "\nHouse Rent Allowance: " << emp_hra;
        cout << "\nMedical Allowance: " << emp_ma;
        cout << "\nProvident Fund: " << emp_pf;
        cout << "\nProfessional Tax: " << ptax;
        cout << "\nIncome Tax: " << itax;
        cout << "\nGross Salary: " << gross;
        cout << "\nNet Salary: " << net << endl;
    }

    void calculate() {
        emp_da = emp_basic * da_rate / 100;
        emp_hra = emp_basic * hra_rate / 100;
        emp_ma = ma_amount;
        emp_pf = 0.12 * (emp_basic + emp_da);
        ptax = ptax_amount;
        gross = emp_basic + emp_da + emp_hra + emp_ma;
        net = gross - (emp_pf + ptax + itax);
    }

    // Static functions to set rates
    static void setDaRate(float rate) { da_rate = rate; }
    static void setHraRate(float rate) { hra_rate = rate; }
    static void setMaAmount(float amount) { ma_amount = amount; }
    static void setPtaxAmount(float amount) { ptax_amount = amount; }

    // Friend functions for file operations
    friend void storePermanentEmployee(PermanentEmployee& emp);
    friend void generatePayslip(PermanentEmployee& emp);
    friend PermanentEmployee searchPermanentEmployee(int emp_no);
};

// Initialize static members
float PermanentEmployee::da_rate = 0;
float PermanentEmployee::hra_rate = 0;
float PermanentEmployee::ma_amount = 0;
float PermanentEmployee::ptax_amount = 0;

// Contractual Employee class
class ContractualEmployee : public Employee {
private:
    float gross;
    float ptax;
    float itax;
    float net;

    // Static member for professional tax
    static float ptax_amount;

public:
    void getData() override {
        Employee::getData();
        cout << "Enter Gross Salary: ";
        cin >> gross;
        cout << "Enter Income Tax: ";
        cin >> itax;
        calculate();
    }

    void displayData() override {
        Employee::displayData();
        cout << "\nEmployee Type: Contractual";
        cout << "\nGross Salary: " << gross;
        cout << "\nProfessional Tax: " << ptax;
        cout << "\nIncome Tax: " << itax;
        cout << "\nNet Salary: " << net << endl;
    }

    void calculate() {
        ptax = ptax_amount;
        net = gross - (ptax + itax);
    }

    // Static function to set professional tax
    static void setPtaxAmount(float amount) { ptax_amount = amount; }

    // Friend functions for file operations
    friend void storeContractualEmployee(ContractualEmployee& emp);
    friend void generatePayslip(ContractualEmployee& emp);
    friend ContractualEmployee searchContractualEmployee(int emp_no);
};

// Initialize static member
float ContractualEmployee::ptax_amount = 0;

// Overtime structure
struct OvertimeRecord {
    int emp_no;
    string emp_name;
    float overtime_hours;
    float overtime_pay;
};

// File operations for Permanent Employees
void storePermanentEmployee(PermanentEmployee& emp) {
    ofstream file("permanent_employees.dat", ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&emp), sizeof(emp));
    file.close();
}

PermanentEmployee searchPermanentEmployee(int emp_no) {
    PermanentEmployee emp;
    ifstream file("permanent_employees.dat", ios::binary);
    
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        if (emp.getEmpNo() == emp_no) {
            file.close();
            return emp;
        }
    }
    
    file.close();
    PermanentEmployee empty;
    return empty;
}

void generatePayslip(PermanentEmployee& emp) {
    string filename = "payslip_" + to_string(emp.getEmpNo()) + ".txt";
    ofstream payslip(filename);
    
    payslip << "////////////////////////////////////Pay Slip///////////////////////////////\n\n";
    payslip << "Employee Number: " << emp.getEmpNo() << endl;
    payslip << "Name: " << emp.getEmpName() << endl;
    payslip << "Basic Salary: " << emp.emp_basic << endl;
    payslip << "Dearness Allowance: " << emp.emp_da << endl;
    payslip << "House Rent Allowance: " << emp.emp_hra << endl;
    payslip << "Medical Allowance: " << emp.emp_ma << endl;
    payslip << "Provident Fund: " << emp.emp_pf << endl;
    payslip << "Professional Tax: " << emp.ptax << endl;
    payslip << "Income Tax: " << emp.itax << endl;
    payslip << "Gross Salary: " << emp.gross << endl;
    payslip << "Net Salary: " << emp.net << endl;
    
    payslip.close();
    cout << "Payslip generated successfully as " << filename << endl;
}

// File operations for Contractual Employees
void storeContractualEmployee(ContractualEmployee& emp) {
    ofstream file("contractual_employees.dat", ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&emp), sizeof(emp));
    file.close();
}

ContractualEmployee searchContractualEmployee(int emp_no) {
    ContractualEmployee emp;
    ifstream file("contractual_employees.dat", ios::binary);
    
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        if (emp.getEmpNo() == emp_no) {
            file.close();
            return emp;
        }
    }
    
    file.close();
    ContractualEmployee empty;
    return empty;
}

void generatePayslip(ContractualEmployee& emp) {
    string filename = "payslip_" + to_string(emp.getEmpNo()) + ".txt";
    ofstream payslip(filename);
    
    payslip << "////////////////////////////////////Pay Slip///////////////////////////////\n\n";
    payslip << "Employee Number: " << emp.getEmpNo() << endl;
    payslip << "Name: " << emp.getEmpName() << endl;
    payslip << "Gross Salary: " << emp.gross << endl;
    payslip << "Professional Tax: " << emp.ptax << endl;
    payslip << "Income Tax: " << emp.itax << endl;
    payslip << "Net Salary: " << emp.net << endl;
    
    payslip.close();
    cout << "Payslip generated successfully as " << filename << endl;
}

// Overtime functions
void addOvertime(vector<OvertimeRecord>& overtime_records) {
    OvertimeRecord record;
    cout << "\nEnter Employee Number: ";
    cin >> record.emp_no;
    cin.ignore();
    
    cout << "Enter Employee Name: ";
    getline(cin, record.emp_name);
    
    cout << "Enter Overtime Hours: ";
    cin >> record.overtime_hours;
    
    record.overtime_pay = record.overtime_hours * 400; // ₹400 per hour
    overtime_records.push_back(record);
    cout << "Overtime record added successfully!\n";
}

void displayOvertime(const vector<OvertimeRecord>& overtime_records) {
    if (overtime_records.empty()) {
        cout << "\nNo overtime records found!\n";
        return;
    }
    
    cout << "\nOvertime Dues for Employees:\n";
    cout << "----------------------------------------\n";
    cout << setw(10) << "Emp No." << setw(20) << "Emp Name" << setw(15) << "Overtime Pay" << endl;
    cout << "----------------------------------------\n";
    
    for (const auto& record : overtime_records) {
        cout << setw(10) << record.emp_no 
             << setw(20) << record.emp_name 
             << setw(15) << record.overtime_pay << endl;
    }
}

// Main menu
void displayMenu() {
    cout << "\n===== PAY SLIP GENERATION SYSTEM =====\n";
    cout << "1. Add Permanent Employee\n";
    cout << "2. Add Contractual Employee\n";
    cout << "3. Generate Pay Slip\n";
    cout << "4. Set DA Rate (Permanent Employees)\n";
    cout << "5. Set HRA Rate (Permanent Employees)\n";
    cout << "6. Set Medical Allowance (Permanent Employees)\n";
    cout << "7. Set Professional Tax\n";
    cout << "8. Add Overtime Record\n";
    cout << "9. Display Overtime Records\n";
    cout << "10. Search Employee\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    vector<OvertimeRecord> overtime_records;
    int choice;
    
    do {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: {
                PermanentEmployee emp;
                emp.getData();
                storePermanentEmployee(emp);
                break;
            }
            case 2: {
                ContractualEmployee emp;
                emp.getData();
                storeContractualEmployee(emp);
                break;
            }
            case 3: {
                int emp_no, emp_type;
                cout << "\nEnter Employee Number: ";
                cin >> emp_no;
                cout << "Enter Employee Type (1-Permanent, 2-Contractual): ";
                cin >> emp_type;
                
                if (emp_type == 1) {
                    PermanentEmployee emp = searchPermanentEmployee(emp_no);
                    if (emp.getEmpNo() == emp_no) {
                        generatePayslip(emp);
                    } else {
                        cout << "Employee not found!\n";
                    }
                } else if (emp_type == 2) {
                    ContractualEmployee emp = searchContractualEmployee(emp_no);
                    if (emp.getEmpNo() == emp_no) {
                        generatePayslip(emp);
                    } else {
                        cout << "Employee not found!\n";
                    }
                } else {
                    cout << "Invalid employee type!\n";
                }
                break;
            }
            case 4: {
                float rate;
                cout << "\nEnter new DA Rate (%): ";
                cin >> rate;
                PermanentEmployee::setDaRate(rate);
                cout << "DA Rate updated successfully!\n";
                break;
            }
            case 5: {
                float rate;
                cout << "\nEnter new HRA Rate (%): ";
                cin >> rate;
                PermanentEmployee::setHraRate(rate);
                cout << "HRA Rate updated successfully!\n";
                break;
            }
            case 6: {
                float amount;
                cout << "\nEnter new Medical Allowance amount: ";
                cin >> amount;
                PermanentEmployee::setMaAmount(amount);
                cout << "Medical Allowance updated successfully!\n";
                break;
            }
            case 7: {
                float amount;
                cout << "\nEnter new Professional Tax amount: ";
                cin >> amount;
                PermanentEmployee::setPtaxAmount(amount);
                ContractualEmployee::setPtaxAmount(amount);
                cout << "Professional Tax updated for all employees!\n";
                break;
            }
            case 8: {
                addOvertime(overtime_records);
                break;
            }
            case 9: {
                displayOvertime(overtime_records);
                break;
            }
            case 10: {
                int emp_no, emp_type;
                cout << "\nEnter Employee Number: ";
                cin >> emp_no;
                cout << "Enter Employee Type (1-Permanent, 2-Contractual): ";
                cin >> emp_type;
                
                if (emp_type == 1) {
                    PermanentEmployee emp = searchPermanentEmployee(emp_no);
                    if (emp.getEmpNo() == emp_no) {
                        emp.displayData();
                    } else {
                        cout << "Employee not found!\n";
                    }
                } else if (emp_type == 2) {
                    ContractualEmployee emp = searchContractualEmployee(emp_no);
                    if (emp.getEmpNo() == emp_no) {
                        emp.displayData();
                    } else {
                        cout << "Employee not found!\n";
                    }
                } else {
                    cout << "Invalid employee type!\n";
                }
                break;
            }
            case 0: {
                cout << "\nExiting system. Goodbye!\n";
                break;
            }
            default: {
                cout << "\nInvalid choice! Please try again.\n";
                break;
            }
        }
    } while (choice != 0);
    
}
