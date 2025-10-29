#include <iostream>
#include <string>
#include <stack>
using namespace std;

// -------------------- Book Node --------------------
struct Book {
    int bookID;
    string title;
    string author;
    string status; // "Available" or "Issued"
    Book* next;

    Book(int id, string t, string a, string s = "Available") {
        bookID = id;
        title = t;
        author = a;
        status = s;
        next = nullptr;
    }
};

// -------------------- Singly Linked List for Book Management --------------------
class BookList {
private:
    Book* head;

public:
    BookList() { head = nullptr; }

    // Insert a new book with duplicate ID check
    void insertBook(int id, string title, string author) {
        // Check for duplicate BookID before inserting
        Book* temp = head;
        while (temp != nullptr) {
            if (temp->bookID == id) {
                cout << "Book ID " << id << " already exists! Insertion cancelled.\n";
                return;
            }
            temp = temp->next;
        }

        // Proceed with insertion
        Book* newBook = new Book(id, title, author);
        if (head == nullptr) {
            head = newBook;
        } else {
            Book* current = head;
            while (current->next != nullptr)
                current = current->next;
            current->next = newBook;
        }

        cout << "Book \"" << title << "\" added successfully!\n";
    }

    // Delete a book by ID
    void deleteBook(int id) {
        Book* temp = head;
        Book* prev = nullptr;

        while (temp != nullptr) {
            if (temp->bookID == id) {
                if (prev == nullptr)
                    head = temp->next;
                else
                    prev->next = temp->next;
                cout << "Book ID " << id << " deleted successfully.\n";
                delete temp;
                return;
            }
            prev = temp;
            temp = temp->next;
        }
        cout << "Book not found.\n";
    }

    // Search for a book by ID
    Book* searchBook(int id) {
        Book* temp = head;
        while (temp != nullptr) {
            if (temp->bookID == id)
                return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    // Display all books
    void displayBooks() {
        if (head == nullptr) {
            cout << "No books available in the library.\n";
            return;
        }

        cout << "\nCurrent Books in Library:\n";
        cout << "----------------------------------------------\n";
        Book* temp = head;
        while (temp != nullptr) {
            cout << "ID: " << temp->bookID
                 << " | Title: " << temp->title
                 << " | Author: " << temp->author
                 << " | Status: " << temp->status << endl;
            temp = temp->next;
        }
        cout << "----------------------------------------------\n";
    }
};

// -------------------- Stack for Undo Transactions --------------------
struct Transaction {
    int bookID;
    string action; // "issue" or "return"
};

class TransactionStack {
private:
    stack<Transaction> transactions;

public:
    void push(Transaction t) { transactions.push(t); }

    Transaction pop() {
        if (transactions.empty()) {
            return {-1, ""};
        }
        Transaction t = transactions.top();
        transactions.pop();
        return t;
    }

    bool isEmpty() { return transactions.empty(); }

    void display() {
        if (transactions.empty()) {
            cout << "No transactions yet.\n";
            return;
        }

        stack<Transaction> tempStack = transactions;
        cout << "\nTransaction History:\n";
        while (!tempStack.empty()) {
            Transaction t = tempStack.top();
            cout << "Book ID " << t.bookID << " -> " << t.action << endl;
            tempStack.pop();
        }
    }
};

// -------------------- Library Management System --------------------
class LibrarySystem {
private:
    BookList bookList;
    TransactionStack transactionStack;

public:
    // Insert a new book
    void insertBook() {
        int id;
        string title, author;
        cout << "Enter Book ID: ";
        cin >> id;
        if (id <= 0) {
            cout << "Invalid Book ID! Must be positive.\n";
            return;
        }
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        bookList.insertBook(id, title, author);
    }

    // Delete a book
    void deleteBook() {
        int id;
        cout << "Enter Book ID to delete: ";
        cin >> id;
        bookList.deleteBook(id);
    }

    // Issue a book
    void issueBook() {
        int id;
        cout << "Enter Book ID to issue: ";
        cin >> id;

        Book* book = bookList.searchBook(id);
        if (book == nullptr) {
            cout << "Book not found.\n";
            return;
        }
        if (book->status == "Issued") {
            cout << "Book already issued.\n";
            return;
        }

        book->status = "Issued";
        transactionStack.push({id, "issue"});
        cout << "Book ID " << id << " issued successfully.\n";
    }

    // Return a book
    void returnBook() {
        int id;
        cout << "Enter Book ID to return: ";
        cin >> id;

        Book* book = bookList.searchBook(id);
        if (book == nullptr) {
            cout << "Book not found.\n";
            return;
        }
        if (book->status == "Available") {
            cout << "Book is already available.\n";
            return;
        }

        book->status = "Available";
        transactionStack.push({id, "return"});
        cout << "Book ID " << id << " returned successfully.\n";
    }

    // Undo last transaction
    void undoTransaction() {
        Transaction t = transactionStack.pop();
        if (t.bookID == -1) {
            cout << "No transactions to undo.\n";
            return;
        }

        Book* book = bookList.searchBook(t.bookID);
        if (book == nullptr) {
            cout << "Book not found for undo.\n";
            return;
        }

        if (t.action == "issue") {
            book->status = "Available";
            cout << "Undo: Book ID " << t.bookID << " marked as Available.\n";
        } else if (t.action == "return") {
            book->status = "Issued";
            cout << "Undo: Book ID " << t.bookID << " marked as Issued.\n";
        }
    }

    // View all transactions
    void viewTransactions() {
        transactionStack.display();
    }

    // Display all books
    void displayBooks() {
        bookList.displayBooks();
    }

    // Main Menu
    void menu() {
        int choice;
        do {
            cout << "\n==== Library Book Management System ====\n";
            cout << "1. Insert Book\n";
            cout << "2. Delete Book\n";
            cout << "3. Issue Book\n";
            cout << "4. Return Book\n";
            cout << "5. Undo Last Transaction\n";
            cout << "6. View Transactions\n";
            cout << "7. Display All Books\n";
            cout << "8. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: insertBook(); break;
                case 2: deleteBook(); break;
                case 3: issueBook(); break;
                case 4: returnBook(); break;
                case 5: undoTransaction(); break;
                case 6: viewTransactions(); break;
                case 7: displayBooks(); break;
                case 8: cout << "Exiting Library System. Goodbye!\n"; break;
                default: cout << "Invalid choice! Try again.\n";
            }
        } while (choice != 8);
    }
};

// -------------------- Main Function --------------------
int main() {
    LibrarySystem library;
    library.menu();
    return 0;
}
