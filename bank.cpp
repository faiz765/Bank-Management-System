#include <iostream.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>




struct Account {
    int accountNo;
    char name[50];
    float balance;
    Account *next;
};


struct Transaction {
    char details[100];
    Transaction *next;
};

struct Customer {
    int accountNo;
    char name[50];
    Customer *next;
};

Account *head = NULL;
Transaction *top = NULL;
Customer *front = NULL, *rear = NULL;
int stats[5] = {0, 0, 0, 0, 0};       


Account* findAcc(int accNo) {
    Account *temp = head;
    while(temp != NULL) {
	if(temp->accountNo == accNo) return temp;
	temp = temp->next;
    }
    return NULL;
}

void logTx(char msg[]) {
    Transaction *node = new Transaction;
    strcpy(node->details, msg);
    node->next = top;
    top = node;
}

void saveData() {
    FILE *fp = fopen("BANK.TXT", "w");
    if(!fp) return;

    Account *temp = head;
    while(temp != NULL) {
	fprintf(fp, "%d\t%s\t%.2f\n", temp->accountNo, temp->name, temp->balance);
	temp = temp->next;
    }
    fclose(fp);
}


void loadData() {
    FILE *fp = fopen("BANK.TXT", "r");
    if(!fp) return;

    int accNo;
    char name[50];
    float bal;


    while(fscanf(fp, "%d\t%[^\t]\t%f\n", &accNo, name, &bal) != EOF) {
	Account *node = new Account;
	node->accountNo = accNo;
	strcpy(node->name, name);
	node->balance = bal;
	node->next = NULL;

	if(!head) head = node;
	else {
	    Account *temp = head;
	    while(temp->next) temp = temp->next;
	    temp->next = node;
	}
	stats[0]++;
    }
    fclose(fp);
}



// 1. Create Account
void createAccount() {
    clrscr();
    int accNo;
    cout << "Enter Account Number: ";
    cin >> accNo;

    if(findAcc(accNo) != NULL) {
	cout << "\nAccount already exists!";
	getch();
	return;
    }

    Account *node = new Account;
    node->accountNo = accNo;

    cin.ignore();
    cout << "Enter Full Name: ";
    cin.getline(node->name, 50);

    cout << "Enter Balance: ";
    cin >> node->balance;
    node->next = NULL;

    if(head == NULL) head = node;
    else {
	Account *temp = head;
	while(temp->next != NULL) temp = temp->next;
	temp->next = node;
    }

    stats[0]++;
    char msg[100];
    sprintf(msg, "Created Acc %d (%s)", accNo, node->name);
    logTx(msg);
    saveData();
    cout << "\nAccount created & saved successfully!";
    getch();
}

// 2. Display Accounts
void displayAccounts() {
    clrscr();
    Account *temp = head;
    if(!temp) cout << "No accounts available!";
    while(temp != NULL) {
	cout << "Acc: " << temp->accountNo << " | Name: " << temp->name << " | Bal: Rs." << temp->balance << "\n";
	temp = temp->next;
    }
    getch();
}

// 3. Search Account
void searchAccount() {
    clrscr();
    int accNo;
    cout << "Enter Account Number: ";
    cin >> accNo;
    Account *acc = findAcc(accNo);
    if(acc) cout << "\nFound -> Name: " << acc->name << " | Balance: Rs." << acc->balance;
    else cout << "\nAccount not found!";
    getch();
}

// 4. Delete Account
void deleteAccount() {
    clrscr();
    int accNo;
    cout << "Enter Account Number: ";
    cin >> accNo;

    Account *temp = head, *prev = NULL;
    while(temp != NULL && temp->accountNo != accNo) {
	prev = temp;
	temp = temp->next;
    }

    if(temp == NULL) cout << "\nAccount not found!";
    else {
	if(prev == NULL) head = temp->next;
	else prev->next = temp->next;
	delete temp;
	stats[0]--;

	char msg[100];
	sprintf(msg, "Deleted Acc %d", accNo);
	logTx(msg);
	saveData();
	cout << "\nAccount deleted successfully!";
    }
    getch();
}

// 5 & 6. Deposit and Withdraw Operations
void transaction(int type) {
    clrscr();
    int accNo;
    float amt;
    char msg[100];

    cout << "Enter Account Number: ";
    cin >> accNo;
    Account *acc = findAcc(accNo);

    if(!acc) { cout << "\nAccount not found!"; getch(); return; }

    cout << "Enter Amount: ";
    cin >> amt;

    if(amt <= 0 || (type == 2 && amt > acc->balance)) {
	cout << "\nInvalid amount or insufficient balance!";
	getch();
	return;
    }

    if(type == 1) {
	acc->balance += amt;
	stats[1]++;
	sprintf(msg, "Deposit Rs.%.2f to Acc %d", amt, accNo);
    } else {
	acc->balance -= amt;
	stats[2]++;
	sprintf(msg, "Withdrawal Rs.%.2f from Acc %d", amt, accNo);
    }

    logTx(msg);
    saveData();
    cout << "\nSuccessful! New Balance: Rs." << acc->balance;
    getch();
}

// 7. Fund Transfer Operation
void transfer() {
    clrscr();
    int sNo, rNo;
    float amt;
    char msg[100];

    cout << "Sender Acc: "; cin >> sNo;
    cout << "Receiver Acc: "; cin >> rNo;

    Account *src = findAcc(sNo);
    Account *dest = findAcc(rNo);

    if(!src || !dest) { cout << "\nInvalid account(s)!"; getch(); return; }

    cout << "Enter Amount: "; cin >> amt;
    if(amt <= 0 || amt > src->balance) { cout << "\nInvalid amount/balance!"; getch(); return; }

    src->balance -= amt;
    dest->balance += amt;
    stats[3]++;

    sprintf(msg, "Transfer Rs.%.2f From %d To %d", amt, sNo, rNo);
    logTx(msg);
    saveData();
    cout << "\nTransfer Successful!";
    getch();
}

// 8. Transaction History
void showHistory() {
    clrscr();
    Transaction *temp = top;
    if(!temp) cout << "No transaction history!";
    while(temp != NULL) {
	cout << temp->details << "\n";
	temp = temp->next;
    }
    getch();
}
// 9, 10, 11. 
void queueOp(int action) {
    clrscr();
    if(action == 1) { 
	int accNo;
	cout << "Enter Account Number: ";
	cin >> accNo;
	Account *acc = findAcc(accNo);
	if(!acc) { cout << "\nAccount not found!"; getch(); return; }

	Customer *node = new Customer;
	node->accountNo = acc->accountNo;
	strcpy(node->name, acc->name);
	node->next = NULL;

	if(!rear) front = rear = node;
	else { rear->next = node; rear = node; }
	cout << "\nCustomer added to queue!";
    }
    else if(action == 2) { 
	Customer *temp = front;
	if(!temp) cout << "Queue is empty!";
	int pos = 1;
	while(temp) {
	    cout << pos++ << ". " << temp->name << " (Acc " << temp->accountNo << ")\n";
	    temp = temp->next;
	}
    }
    else if(action == 3) { 
	if(!front) cout << "Queue is empty!";
	else {
	    Customer *temp = front;
	    cout << "Now Serving: " << temp->name << " (Acc " << temp->accountNo << ")";
	    front = front->next;
	    if(!front) rear = NULL;
	    delete temp;
	    stats[4]++;
	}
    }
    getch();
}

// 12. Statistics
void showStats() {
    clrscr();
    cout << "========== STATISTICS (ARRAY) ==========\n";
    cout << "Total Accounts   : " << stats[0] << "\n";
    cout << "Total Deposits   : " << stats[1] << "\n";
    cout << "Total Withdrawals: " << stats[2] << "\n";
    cout << "Total Transfers  : " << stats[3] << "\n";
    cout << "Customers Served : " << stats[4] << "\n";
    getch();
}
void main() {
    loadData();
    int choice;
    do {
	clrscr();
	cout << "========== BANK MANAGEMENT SYSTEM ==========\n";
	cout << "1. Create Acc     6. Withdraw       11. Serve Customer\n";
	cout << "2. Display Accs   7. Transfer       12. Statistics\n";
	cout << "3. Search Acc     8. History        13. Exit & Save\n";
	cout << "4. Delete Acc     9. Add Queue\n";
	cout << "5. Deposit       10. View Queue\n";
	cout << "--------------------------------------------\n";
	cout << "Choice: ";
	cin >> choice;

	switch(choice) {
	    case 1: createAccount(); break;
	    case 2: displayAccounts(); break;
	    case 3: searchAccount(); break;
	    case 4: deleteAccount(); break;
	    case 5: transaction(1); break;
	    case 6: transaction(2); break;
	    case 7: transfer(); break;
	    case 8: showHistory(); break;        

	    case 9: queueOp(1); break;
	    case 10: queueOp(2); break;
	    case 11: queueOp(3); break;
	    case 12: showStats(); break;
	}
    } while(choice != 13);

    saveData();
	}
