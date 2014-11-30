#ifndef TESTS
#error "You shouldn't compile UnitTests.cc without test mode on."
#endif

#include "UnitTests.h"

#include "Bank.h"

#include "Printer.h"
#include "debug.h"

using namespace std;


static void VectorsEqual(vector<PrintState>& lhs, vector<PrintState>& rhs) {
    int ix = 0;
    while(ix < (int)min(lhs.size(), rhs.size())) {
        if(!lhs[ix].equals(rhs[ix])) {
            printf("Values not equal at %d:\n", ix);
            printf("Should be:\t\t"); lhs[ix].print();
            printf("But was:\t\t"); rhs[ix].print();
        }
        ix++;
    }
    if(ix < (int)lhs.size()) {
        printf("Missing values:\n");
    }
    while(ix < (int)lhs.size()) {
        printf("\t"); lhs[ix++].print();
    }

    if(ix < (int)rhs.size()) {
        printf("Extra values:\n");
    }
    while(ix < (int)rhs.size()) {
        printf("\t"); rhs[ix++].print();
    }
}


_Task BankWithdraw {
public:
    Bank &bank;
    int id;
    int count;
    BankWithdraw(Bank & bank, int id, int count) : bank(bank), id(id), count(count) { }
    void main() {
        bank.withdraw(id, count);
    }
};

static void TestBank(uBaseTask& task) {
    testPrinter.states.clear();
    Bank bank(1);
    {
        BankWithdraw task(bank, 0, 2);
        task.yield(10);
        bank.deposit(0, 1);
        bank.deposit(0, 1);
    }

    vector<PrintState> TestBankResults {
        PrintState(PrinterKind::BankDeposit, 0, ' ', 1), 
        PrintState(PrinterKind::BankDeposit, 0, ' ', 1),
        PrintState(PrinterKind::BankWithdraw, 0, ' ', 2)
    };
    VectorsEqual(TestBankResults, testPrinter.states);
}

void RunAllTests(uBaseTask& task) {
    TestBank(task);
}
void uMain::main() {
    RunAllTests(*this);
}