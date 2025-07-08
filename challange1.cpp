#include <bits/stdc++.h>
using namespace std;

class product {
 private:
  string product_name;
  double price;
  int quantity;
  string expiry_date = "none"; // assumed that it doesn't have by default
  double weight = 0;

 public:
  product(string product_name, double price, int quantity)
      : product_name(product_name), price(price), quantity(quantity) {}

  void set_product_name(string product_name) { this->product_name = product_name; }
  void set_price(double price) { this->price = price; }
  void set_quantity(int quantity) { this->quantity = quantity; }

  string get_product_name() const { return this->product_name; }
  double get_price() const { return this->price; }
  int get_quantity() const { return this->quantity; }

  void set_weight(double weight) { this->weight = weight; }
  double get_weight() const { return this->weight; }

  void set_expiry_date(string expiry_date) { this->expiry_date = expiry_date; }
  string get_expiry_date() const { return this->expiry_date; }
};

class customer {
 private:
  double balance;
 public:
  customer(double balance) : balance(balance) {}
  double get_balance() const { return balance; }
};

class cart {
 private:
  double subtotal = 0;
  double shipping_fees = 0;
  double paid_amount = 0;
  double cur_balance = 0;
  int products_quantity = 0;

 public:
  vector<product> products;

  cart() = default;

  double get_subtotal() const { return this->subtotal; }
  double get_shipping_fees() const { return this->shipping_fees; }
  double get_paid_amount() const { return this->paid_amount; }
  double get_cur_balance() const { return this->cur_balance; }
  int get_products_quantity() const { return this->products_quantity; }

  void add_product(double user_balance) {
    string product_name, expiry_date = "none";
    double price, weight = 0;
    int quantity;
    bool is_shippable = false, is_expirable = false;

    cout << "Product name: ";
    cin >> product_name;
    cout << "Quantity: ";
    cin >> quantity;
    cout << "Price per unit: ";
    cin >> price;

    cout << "Is shippable (1-yes / 0-no)? ";
    cin >> is_shippable;
    if (is_shippable) {
      cout << "Weight in grams: ";
      cin >> weight;
    }

    cout << "Is expirable (1-yes / 0-no)? ";
    cin >> is_expirable;
    if (is_expirable) {
      cout << "Expiry date (DD/MM/YYYY): ";
      cin >> expiry_date;
    }

    while (quantity * price > user_balance) {
      cout << "Insufficient balance. Re-enter quantity and price:\n";
      cout << "Quantity: ";
      cin >> quantity;
      cout << "Price: ";
      cin >> price;
    }

    while (is_expirable && expiry_date < "04/07/2025") {
      cout << "Product is expired. Enter a valid expiry date:\n";
      cin >> expiry_date;
    }

    product new_product(product_name, price, quantity);
    if (is_shippable) new_product.set_weight(weight);
    if (is_expirable) new_product.set_expiry_date(expiry_date);

    products.push_back(new_product);
    products_quantity++;
  }

  void calc_money() {
    subtotal = 0;
    shipping_fees = 0;
    for (auto& p : products) {
      subtotal += p.get_price() * p.get_quantity();
      shipping_fees += p.get_weight() * 0.2 * p.get_quantity();
    }
    paid_amount = subtotal + shipping_fees;
  }
};

double calc_shipment_fees(const cart& ca) {
  double total_weight = 0, total_weight_fees = 0;
  cout << "** Shipment notice **\n";
  for (auto& product : ca.products) {
    cout << product.get_quantity() << "x " << product.get_product_name();
    cout << " -> " << product.get_weight() * product.get_quantity() << "g\n";
    total_weight += product.get_weight() * product.get_quantity();
    total_weight_fees += product.get_weight() * 0.2 * product.get_quantity();
  }
  cout << "Total weight: ";
  if (total_weight >= 1000) cout << total_weight / 1000.0 << " kg\n";
  else cout << total_weight << " g\n";
  return total_weight_fees;
}

double calc_money_fees(const cart& ca) {
  double total_sub_fees = 0;
  cout << "** Checkout receipt **\n";
  for (auto& product : ca.products) {
    double item_total = product.get_quantity() * product.get_price();
    cout << product.get_quantity() << "x " << product.get_product_name()
         << " = " << item_total << "\n";
    total_sub_fees += item_total;
  }
  return total_sub_fees;
}

void check_out(const customer& cust, cart& ca) {
  ca.calc_money();
  cout << "Subtotal:      " << calc_money_fees(ca) << "\n";
  cout << "Shipping:      " << calc_shipment_fees(ca) << "\n";
  double total = ca.get_paid_amount();
  cout << "Total Amount:  " << total << "\n";
  cout << "Remaining Balance: " << cust.get_balance() - total << "\n";
}

int main() {
  customer cust(1000);
  cart ca;

  int num_products;
  cout << "How many products to add? ";
  cin >> num_products;

  for (int i = 0; i < num_products; ++i) {
    ca.add_product(cust.get_balance());
  }

  check_out(cust, ca);

  return 0;
}
