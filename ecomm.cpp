#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <cctype>
#include <algorithm> // for transform

using namespace std;

string convertToUpper(string text) {
    transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

// Product Class
class Product {
public:
    string productID;
    string name;
    double price;

    // Default constructor
    Product() : productID(""), name(""), price(0) {}

    // Parameterized constructor
    Product(string id, string n, double p) : productID(id), name(n), price(p) {}
};

// ShoppingCart Class
class ShoppingCart {
private:
    unordered_map<string, pair<Product, int> > cart;

public:
    void addProduct(Product product, int quantity) {
        product.productID = convertToUpper(product.productID);
        if (cart.find(product.productID) != cart.end()) { // If product is in cart
            cart[product.productID].second += quantity; // Add to the product's quantity
        } else {
            cart[product.productID] = make_pair(product, quantity); // Otherwise, add the product
        }
        cout << "Product added successfully!" << endl;
    }

    void viewCart() {
        if (cart.empty()) {
            cout << "Shopping cart is empty." << endl;
        } else {
            cout << "\nShopping Cart:" << endl;
            cout << left << setw(10) << "Product ID" << setw(10) << "Name" << setw(30) << "Price" << setw(12) << "Quantity" << endl;
            for (const auto& it : cart) {
                const Product& p = it.second.first;
                int qty = it.second.second;
                cout << left << setw(10) << p.productID << setw(10) << p.name << setw(30)
                    << p.price << setw(12) << qty << endl;
            }
        }
    }

    double checkout() {
        double total = 0;
        if (cart.empty()) {
            cout << "Shopping cart is empty." << endl;
            return total;
        }
        cout << "\nCheckout Products:" << endl;
        cout << left << setw(10) << "Product ID" << setw(10) << "Name" << setw(30) << "Price" << setw(12) << "Quantity" << endl;
        for (const auto& it : cart) {
            const Product& p = it.second.first;
            int qty = it.second.second;
            total += p.price * qty;
            cout << left << setw(10) << p.productID << setw(10) << p.name << setw(30)
                << p.price << setw(12) << qty << endl;
        }
        cout << "\nTotal Amount: " << total << endl;
        return total;
    }

    bool isEmpty() const {
        return cart.empty();
    }

    const unordered_map<string, pair<Product, int> >& getCart() const {
        return cart;
    }

    void clearCart() {
        cart.clear();  // Clear cart after checkout
    }
};

// Order Class
class Order {
private:
    struct OrderDetails {
        int orderID;
        double totalAmount;
        unordered_map<string, pair<Product, int> > orderDetails;
    };
    vector<OrderDetails> orders;

public:
    void createOrder(ShoppingCart& shoppingCart, double total) {
        if (!shoppingCart.isEmpty()) {
            OrderDetails newOrder;
            newOrder.orderID = orders.size() + 1;
            newOrder.totalAmount = total;
            newOrder.orderDetails = shoppingCart.getCart();  // Save order details
            orders.push_back(newOrder);
            cout << "Order ID " << newOrder.orderID << " placed successfully!" << endl;
            shoppingCart.clearCart();  // Clear the cart after placing the order
        } else {
            cout << "Shopping cart is empty." << endl;
        }
    }

    void viewOrders() {
        if (orders.empty()) {
            cout << "No orders placed yet." << endl;
        } else {
            for (const auto& order : orders) {
                cout << "\nOrder ID: " << order.orderID << endl;
                cout << "Total Amount: " << order.totalAmount << endl;
                cout << "Order Details:" << endl;
                cout << left << setw(10) << "Product ID" << setw(10) << "Name" << setw(30) << "Price" << setw(12) << "Quantity" << endl;
                for (const auto& item : order.orderDetails) {
                    const Product& p = item.second.first;
                    int qty = item.second.second;
                    cout << left << setw(10) << p.productID << setw(10) << p.name << setw(30) << p.price << setw(12) << qty << endl;
                }
            }
        }
    }
};

// List of Products
vector<Product> products;

void initializeProducts() {
    products.push_back(Product("ABC", "Paper", 20));
    products.push_back(Product("CDE", "Pencil", 10));
    products.push_back(Product("QWE", "Notebook", 30));
    products.push_back(Product("ASD", "Pen", 15));
    products.push_back(Product("ZXC", "Eraser", 5));
}

// Function to find a product by ID
Product* findProductByID(const string& productID) {
    for (auto& product : products) {
        if (product.productID == productID) {
            return &product;
        }
    }
    return nullptr;
}

// Menu
void displayMenu() {
    ShoppingCart cart;
    Order order;
    initializeProducts();

    while (true) {
        cout << "\nMenu:" << endl;
        cout << "1. View Products" << endl;
        cout << "2. View Shopping Cart" << endl;
        cout << "3. View Orders" << endl;
        cout << "4. Exit" << endl;

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.clear();
        cin.ignore();

        if (choice == 1) {
            cout << "\nAvailable Products:" << endl;
            cout << left << setw(10) << "Product ID" << setw(10) << "Name" << setw(30) << "Price" << setw(12) << endl;
            for (const auto& product : products) {
                cout << left << setw(10) << product.productID << setw(10) << product.name << setw(30) << product.price << endl;
            }

            while (true) {
                string productID;
                cout << "\nEnter the ID of the product you want to add to the shopping cart: ";
                cin >> productID;
                cin.clear();
                cin.ignore();
                productID = convertToUpper(productID);  // Convert user input to uppercase

                Product* product = findProductByID(productID);

                if (product != nullptr) {
                    int quantity;
                    cout << "Enter quantity: ";
                    cin >> quantity;
                    cin.clear();
                    cin.ignore();
                    cart.addProduct(*product, quantity);
                } else {
                    cout << "Invalid product ID. Please try again." << endl;
                }

                char another;
                cout << "Do you want to add another product? (Y/N): ";
                cin >> another;
                cin.clear();
                cin.ignore();
                if (tolower(another) != 'y') {
                    break;
                }
            }

        } else if (choice == 2) {
            cart.viewCart();
            char checkout;
            cout << "\nDo you want to check out all the products? (Y/N): ";
            cin >> checkout;
            cin.clear();
            cin.ignore();
            if (tolower(checkout) == 'y') {
                double total = cart.checkout();
                if (total > 0) {
                    order.createOrder(cart, total);
                }
            }

        } else if (choice == 3) {
            order.viewOrders();

        } else if (choice == 4) {
            cout << "Exiting the system." << endl;
            break;

        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

int main() {
    displayMenu();
    return 0;
}
