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