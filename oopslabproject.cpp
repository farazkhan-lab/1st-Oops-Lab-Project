#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
using namespace std;

// Product class to encapsulate product details
class Product {
private:
    int pcode;
    string name;
    float price;
    float discount;

public:
    Product(int code = 0, string n = "", float p = 0.0, float d = 0.0)
        : pcode(code), name(n), price(p), discount(d) {}

    // Getters
    int getCode() const { return pcode; }
    string getName() const { return name; }
    float getPrice() const { return price; }
    float getDiscount() const { return discount; }

    // Setters
    void setCode(int code) { pcode = code; }
    void setName(const string& n) { name = n; }
    void setPrice(float p) { price = p; }
    void setDiscount(float d) { discount = d; }
};

// Database class to handle file operations
class Database {
private:
    static const int MAX_PRODUCTS = 100;
    string filename;

public:
    Database(const string& fname = "database.txt") : filename(fname) {}

    bool addProduct(const Product& product) {
        fstream data(filename.c_str(), ios::in);
        int code;
        string name;
        float price, discount;
        bool duplicate = false;

        if (data.is_open()) {
            while (data >> code) {
                getline(data, name, ' '); // Skip space
                getline(data, name, ' '); // Read name
                data >> price >> discount;
                if (code == product.getCode()) {
                    duplicate = true;
                    break;
                }
            }
            data.close();
        }

        if (duplicate) {
            cout << "\n\n\t\t Product code already exists!\n";
            return false;
        }

        fstream data_out(filename.c_str(), ios::app);
        if (!data_out) {
            cout << "\n\n\t\t Error: Could not open file for writing!\n";
            return false;
        }
        data_out << product.getCode() << " " << product.getName() << " "
                 << product.getPrice() << " " << product.getDiscount() << endl;
        data_out.close();
        cout << "\n\n\t\t Record Inserted\n";
        return true;
    }

    bool editProduct(int pkey, const Product& newProduct) {
        fstream data(filename.c_str(), ios::in);
        fstream data1("database1.txt", ios::out | ios::app);
        int code;
        string name;
        float price, discount;
        bool found = false;

        if (!data || !data1) {
            cout << "\n\n\t\t Error: Could not open file!\n";
            if (data.is_open()) data.close();
            if (data1.is_open()) data1.close();
            return false;
        }

        while (data >> code) {
            getline(data, name, ' '); // Skip space
            getline(data, name, ' '); // Read name
            data >> price >> discount;
            if (code == pkey) {
                data1 << newProduct.getCode() << " " << newProduct.getName() << " "
                      << newProduct.getPrice() << " " << newProduct.getDiscount() << endl;
                cout << "\n\n\t\t Record Edited\n";
                found = true;
            } else {
                data1 << code << " " << name << " " << price << " " << discount << endl;
            }
        }
        data.close();
        data1.close();

        if (remove(filename.c_str()) != 0 || rename("database1.txt", filename.c_str()) != 0) {
            cout << "\n\n\t\t Error in file operations\n";
            return false;
        }
        if (!found) {
            cout << "\n\n Record not found\n";
        }
        return found;
    }

    bool deleteProduct(int pkey) {
        fstream data(filename.c_str(), ios::in);
        fstream data1("database1.txt", ios::out | ios::app);
        int code;
        string name;
        float price, discount;
        bool found = false;

        if (!data || !data1) {
            cout << "\n\n\t\t Error: Could not open file!\n";
            if (data.is_open()) data.close();
            if (data1.is_open()) data1.close();
            return false;
        }

        while (data >> code) {
            getline(data, name, ' '); // Skip space
            getline(data, name, ' '); // Read name
            data >> price >> discount;
            if (code == pkey) {
                cout << "\n\n\t Product Deleted Successfully\n";
                found = true;
            } else {
                data1 << code << " " << name << " " << price << " " << discount << endl;
            }
        }
        data.close();
        data1.close();

        if (remove(filename.c_str()) != 0 || rename("database1.txt", filename.c_str()) != 0) {
            cout << "\n\n\t\t Error in file operations\n";
            return false;
        }
        if (!found) {
            cout << "\n\n Record not found\n";
        }
        return found;
    }

    void listProducts() {
        fstream data(filename.c_str(), ios::in);
        if (!data) {
            cout << "\n\n\t File doesn't exist or is empty\n";
            return;
        }

        cout << "\n\n_________________________________________\n";
        cout << "PRODUCT NO\t\tNAME\t\tPRICE\n";
        cout << "_________________________________________\n";
        int code;
        string name;
        float price, discount;
        while (data >> code) {
            getline(data, name, ' '); // Skip space
            getline(data, name, ' '); // Read name
            data >> price >> discount;
            cout << code << "\t\t" << name << "\t\t" << price << endl;
        }
        data.close();
    }

    bool getProduct(int code, Product& product) {
        fstream data(filename.c_str(), ios::in);
        if (!data) {
            return false;
        }
        int pcode;
        string name;
        float price, discount;
        while (data >> pcode) {
            getline(data, name, ' '); // Skip space
            getline(data, name, ' '); // Read name
            data >> price >> discount;
            if (pcode == code) {
                product.setCode(pcode);
                product.setName(name);
                product.setPrice(price);
                product.setDiscount(discount);
                data.close();
                return true;
            }
        }
        data.close();
        return false;
    }
};

class Cart {
private:
    string product_codes[100];
    int quantities[100];
    int item_count;

public:
    Cart() {
        item_count = 0;
    }

    bool addItem(string code, int quantity) {
        if (item_count >= 100) return false;
        product_codes[item_count] = code;
        quantities[item_count] = quantity;
        item_count++;
        return true;
    }

    void displayCart() {
        cout << "\nYour Cart:\n";
        for (int i = 0; i < item_count; i++) {
            cout << "Product Code: " << product_codes[i] << ", Quantity: " << quantities[i] << endl;
        }
    }

    void clearCart() {
        item_count = 0;
    }
};

class ShoppingSystem {
    private:
        Database db;
        Cart cart;
    
        bool authenticateAdmin() {
            string email, password;
            cout << "\t\t\t Please login\n";
            cout << "\t\t\t Enter email: ";
            cin >> email;
            cout << "\t\t\t Enter password: ";
            cin >> password;
            return (email == "aqibmehmood13w@gmail.com" && password == "password123");
        }

        void administratorMenu() {
            while (true) {
                int choice;
                cout << "\n\n\n\t\t\t ADMINISTRATOR MENU\n";
                cout << "\t\t\t _____1) Add the product\n";
                cout << "\t\t\t                        \n";
                cout << "\t\t\t _____2) Modify the product\n";
                cout << "\t\t\t                        \n";
                cout << "\t\t\t _____3) Delete the product\n";
                cout << "\t\t\t                        \n";
                cout << "\t\t\t _____4) Back to Menu\n";
                cout << "\t\t\t Please enter your choice: ";
                cin >> choice;
    
                switch (choice) {
                case 1: {
                    Product p;
                    int code;
                    string name;
                    float price, discount;
                    cout << "\n\n\t\t\t Add new product\n";
                    cout << "\n\n\t Enter Product Code: ";
                    cin >> code;
                    cin.ignore(); // Clear newline
                    cout << "\n\n\t Enter Name of Product: ";
                    getline(cin, name);
                    cout << "\n\n\t Enter Price of Product: ";
                    cin >> price;
                    cout << "\n\n\t Enter Discount of Product (%): ";
                    cin >> discount;
                    p.setCode(code);
                    p.setName(name);
                    p.setPrice(price);
                    p.setDiscount(discount);
                    db.addProduct(p);
                    break;
                }
                case 2: {
                    int pkey;
                    cout << "\n\t\t\t Modify the Record\n";
                    cout << "\n\t\t\t Enter the Product code: ";
                    cin >> pkey;
                    Product p;
                    int code;
                    string name;
                    float price, discount;
                    cout << "\n\t\t Enter new Product Code: ";
                    cin >> code;
                    cin.ignore();
                    cout << "\n\t\t Enter new Product Name: ";
                    getline(cin, name);
                    cout << "\n\t\t Enter new Product Price: ";
                    cin >> price;
                    cout << "\n\t\t Enter new Product Discount (%): ";
                    cin >> discount;
                    p.setCode(code);
                    p.setName(name);
                    p.setPrice(price);
                    p.setDiscount(discount);
                    db.editProduct(pkey, p);
                    break;
                }
                case 3: {
                    int pkey;
                    cout << "\n\n\t DELETE RECORD\n";
                    cout << "\n\n\t Enter product code: ";
                    cin >> pkey;
                    db.deleteProduct(pkey);
                    break;
                }
                case 4:
                    return;
                default:
                    cout << "Invalid choice\n";
                }
            }
        }
    
        void buyerMenu() {
            while (true) {
                int choice;
                cout << "\t\t\t BUYER\n";
                cout << "\t\t\t _______\n";
                cout << "\t\t\t 1) Buy product\n";
                cout << "\t\t\t _______\n";
                cout << "\t\t\t 2) Go Back\n";
                cout << "Enter your choice: ";
                cin >> choice;
    
                switch (choice) {
                case 1: {
                    db.listProducts();
                    cout << "\n____________________________\n";
                    cout << "\n   PLEASE PLACE THE ORDER    \n";
                    cout << "\n____________________________\n";
                    char more;
                    do {
                        int code, quantity;
                        cout << "\n\n Enter Product Code: ";
                        cin >> code;
                        cout << "\n\n Enter Product Quantity: ";
                        cin >> quantity;
                        if (cart.addItem(code, quantity)) {
                            cout << "\n\n Item added to cart\n";
                        }
                        cout << "\n\n Do You Want To Buy Another Product? (y/n): ";
                        cin >> more;
                    } while (more == 'y' || more == 'Y');
                    cart.generateReceipt();
                    break;
                }
                case 2:
                    return;
                default:
                    cout << "Invalid choice\n";
                }
            }
        }
    
    public:
        ShoppingSystem() : db(), cart(db) {}
    
        void menu() {
            while (true) {
                int choice;
                cout << "\t\t\t\t\t ______________________________\n";
                cout << "\t\t\t\t\t                               \n";
                cout << "\t\t\t\t\t      SUPER MARKET MENU        \n";
                cout << "\t\t\t\t\t                               \n";
                cout << "\t\t\t\t\t ______________________________\n";
                cout << "\t\t\t\t\t                               \n";
                cout << "\t\t\t\t\t  1) ADMINISTRATOR             \n";
                cout << "\t\t\t\t\t                               \n";
                cout << "\t\t\t\t\t  2) BUYER                    \n";
                cout << "\t\t\t\t\t                               \n";
                cout << "\t\t\t\t\t  3) EXIT                     \n";
                cout << "\t\t\t\t\t                               \n";
                cout << "\t\t\t\t\t Please select: ";
                cin >> choice;
    
                switch (choice) {
                case 1:
                    if (authenticateAdmin()) {
                        administratorMenu();
                    } else {
                        cout << "\t\t\t Invalid email or password\n";
                    }
                    break;
                case 2:
                    buyerMenu();
                    break;
                case 3:
                    exit(0);
                default:
                    cout << "Please select a correct option\n";
                }
            }
        }
    };
    
    int main() {
        ShoppingSystem shop;
        shop.menu();
        return 0;
    }
