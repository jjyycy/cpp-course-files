
//
// File: PlainVanillaOption.cpp
// Author(s):Jingyi Guo, Jiawen Zhang, Xiran Zhu
//

#include <iostream>
#include <string>
#include <vector>
#include <cmath>      // for pow()
#include <algorithm>  // for max()
#include <iomanip>    // for setw()
using namespace std;

/* ---------------- PlainVanillaOption class definition ----------------- */

class PlainVanillaOption {
protected:
    // Parameters of a PlainVanillaOption
    double S0;         // initial stock price
    double r;          // risk-free rate
    double sigma;      // volatility
    double T;          // expiration time

    // Inner class used by the binomial tree method
    class Price {
    public:
        double stockPrice;
        double optionPrice;
    };

    // a helper function to display small Binomial Trees
    void put_BinomialTree(string header, const vector<vector<Price>>& bT);

    // interior and terminal node price functions for Binomial Tree
    // PURE VIRTUAL FUNCTIONS, so PlainVanillaOption is an abstract class
    virtual double terminal_node_price(const vector<vector<Price>>& bT,
                                       int numIntervals, int j) = 0;
    virtual double interior_node_price(const vector<vector<Price>>& bT,
                                       int numIntervals, int i, int j) = 0;

public:
    // constructor defined explicitly inline below the class definition
    PlainVanillaOption(double s0, double rfr, double v, double et);

    // Calculate the Price of the option
    // using the binomial tree method
    double binomialPrice(int numIntervals);

    // ... other pricing methods can be added here ...

};

inline PlainVanillaOption::PlainVanillaOption(double s0,
                                              double rfr, double v, double et)
        : S0(s0), r(rfr), sigma(v), T(et)
{}


/* ---------------- EuropeanCallOption class definition ----------------- */

class EuropeanCallOption {
private:
    // Basic values of a stock option
    double S0;         // initial stock price
    double K;          // strike price
    double r;          // risk-free rate
    double sigma;      // volatility
    double T;          // expiration time
    // Inner class used by the binomial tree method
    class Price {
    public:
        double stockPrice;
        double optionPrice;
    };

    // a private helper function
    void put_BinomialTree(string header, const vector<vector<Price>>& bT);

public:
    // constructor defined inline within the class definition
    EuropeanCallOption(double s0, double k, double rfr,
                       double v, double et)
            : S0(s0), K(k), r(rfr), sigma(v), T(et)
    {}

    // Calculate the Price of the option
    // using the binomial tree method
    double binomialPrice(int numIntervals);

    // ... other pricing methods can be added here ...

};


/* ---------- EuropeanCallOption member function definition ----------- */

void EuropeanCallOption::put_BinomialTree(string header,
                                          const vector<vector<Price>>& bT)
{
    int N = (bT.size() == 0)
            ? 0
            : bT.size() - 1;

    if (N > 9) {  // if tree is too big, refuse to print anything
        // cout << header << "\n";
        // cout << "BinomialTree has " << N << " levels: too many to print!\n";
        return;
    }

    cout << "\n" << header << "\n\n";
    cout << "BinomialTree with " << N << " time steps:\n\n";
    for (int i(0); i < bT.size(); ++i) {
        cout << "Stock:  ";
        for (int j(0); j < bT[i].size(); ++j) {
            cout << setw(8) << bT[i][j].stockPrice;
        }
        cout << "\n";
        cout << "Option: ";
        for (int j(0); j < bT[i].size(); ++j) {
            cout << setw(8) << bT[i][j].optionPrice;
        }
        cout << "\n\n";
    }
    cout << "\n";
}


double EuropeanCallOption::binomialPrice(int numIntervals)
{
    // time interval length
    double deltaT  = T / numIntervals;;
    // factor by which stock price might rise at each step
    double u 	   = exp(sigma * sqrt(deltaT));
    // factor by which stock price might fall at each step
    double d 	   = 1 / u;
    // risk-free interest rate factor for one time interval
    double a	   = exp(r * deltaT);
    // RN probability of an up move in stock price
    double p	   = (a - d) / (u - d);
    // RN probability of a down move in stock price
    double q	   = 1.0 - p;
    // container for the binomialTree
    vector<vector<Price>> binomialTree;

    // put_BinomialTree("Initial, empty binomialTree:", binomialTree);

    // Build the shape of the binomialTree, by pushing
    // successively longer vector<Price> values (initially
    // all elements 0.0)
    for (int i(0); i <= numIntervals; ++i) {
        vector<Price> vInterval(i+1);    // i+1 {0.0,0.0} values
        binomialTree.push_back(vInterval);
    }
    put_BinomialTree("After filled in with all 0.0:", binomialTree);

    // Fill the stockPrice component of the binomialTree
    for (int i(0); i <= numIntervals; ++i)
        for (int j(0); j <= i; ++j)
            binomialTree[i][j].stockPrice =
                    S0 * pow(u, j) * pow(d, i-j);
    put_BinomialTree("After filled in with stock prices:", binomialTree);

    // Fill the optionPrices at the terminal nodes
    for (int j(0); j <= numIntervals; ++j) {
        binomialTree[numIntervals][j].optionPrice =
                max(binomialTree[numIntervals][j].stockPrice - K,
                    0.0);
    }
    put_BinomialTree("After filled in with terminal option values:", binomialTree);

    // Now work backwards, filling optionPrices in the rest of the tree
    for (int i(numIntervals-1); i >= 0; --i)
        for (int j(0); j <= i; ++j)
            binomialTree[i][j].optionPrice =
                    exp(-r * deltaT) *
                    (p * binomialTree[i+1][j+1].optionPrice
                     + q * binomialTree[i+1][j].optionPrice);
    put_BinomialTree("After filled in with all option values:", binomialTree);

    // Return the time 0 call price
    return binomialTree[0][0].optionPrice;
}


/* ---------------- EuropeanPutOption class definition ----------------- */

class EuropeanPutOption {
private:
    // Basic values of a stock option
    double S0;         // initial stock price
    double K;          // strike price
    double r;          // risk-free rate
    double sigma;      // volatility
    double T;          // expiration time
    // Inner class used by the binomial tree method
    class Price {
    public:
        double stockPrice;
        double optionPrice;
    };

    // a private helper function
    void put_BinomialTree(string header, const vector<vector<Price>>& bT);

public:
    // constructor defined inline within the class definition
    EuropeanPutOption(double s0, double k, double rfr,
                      double v, double et)
            : S0(s0), K(k), r(rfr), sigma(v), T(et)
    {}

    // Calculate the Price of the option
    // using the binomial tree method
    double binomialPrice(int numIntervals);

    // ... other pricing methods can be added here ...

};


/* ---------- EuropeanPutOption member function definition ----------- */

void EuropeanPutOption::put_BinomialTree(string header,
                                         const vector<vector<Price>>& bT)
{
    int N = (bT.size() == 0)
            ? 0
            : bT.size() - 1;

    if (N > 9) {  // if tree is too big, refuse to print anything
        // cout << header << "\n";
        // cout << "BinomialTree has " << N << " levels: too many to print!\n";
        return;
    }

    cout << "\n" << header << "\n\n";
    cout << "BinomialTree with " << N << " time steps:\n\n";
    for (int i(0); i < bT.size(); ++i) {
        cout << "Stock:  ";
        for (int j(0); j < bT[i].size(); ++j) {
            cout << setw(8) << bT[i][j].stockPrice;
        }
        cout << "\n";
        cout << "Option: ";
        for (int j(0); j < bT[i].size(); ++j) {
            cout << setw(8) << bT[i][j].optionPrice;
        }
        cout << "\n\n";
    }
    cout << "\n";
}


double EuropeanPutOption::binomialPrice(int numIntervals)
{
    // time interval length
    double deltaT  = T / numIntervals;;
    // factor by which stock price might rise at each step
    double u 	   = exp(sigma * sqrt(deltaT));
    // factor by which stock price might fall at each step
    double d 	   = 1 / u;
    // risk-free interest rate factor for one time interval
    double a	   = exp(r * deltaT);
    // RN probability of an up move in stock price
    double p	   = (a - d) / (u - d);
    // RN probability of a down move in stock price
    double q	   = 1.0 - p;
    // container for the binomialTree
    vector<vector<Price>> binomialTree;

    // put_BinomialTree("Initial, empty binomialTree:", binomialTree);

    // Build the shape of the binomialTree, by pushing
    // successively longer vector<Price> values (initially
    // all elements 0.0)
    for (int i(0); i <= numIntervals; ++i) {
        vector<Price> vInterval(i+1);    // i+1 {0.0,0.0} values
        binomialTree.push_back(vInterval);
    }
    put_BinomialTree("After filled in with all 0.0:", binomialTree);

    // Fill the stockPrice component of the binomialTree
    for (int i(0); i <= numIntervals; ++i)
        for (int j(0); j <= i; ++j)
            binomialTree[i][j].stockPrice =
                    S0 * pow(u, j) * pow(d, i-j);
    put_BinomialTree("After filled in with stock prices:", binomialTree);

    // Fill the optionPrices at the terminal nodes
    for (int j(0); j <= numIntervals; ++j) {
        binomialTree[numIntervals][j].optionPrice =
/* Put payoff! */  max(K- binomialTree[numIntervals][j].stockPrice,
                       0.0);
    }
    put_BinomialTree("After filled in with terminal option values:", binomialTree);

    // Now work backwards, filling optionPrices in the rest of the tree
    for (int i(numIntervals-1); i >= 0; --i)
        for (int j(0); j <= i; ++j)
            binomialTree[i][j].optionPrice =
                    exp(-r * deltaT) *
                    (p * binomialTree[i+1][j+1].optionPrice
                     + q * binomialTree[i+1][j].optionPrice);
    put_BinomialTree("After filled in with all option values:", binomialTree);

    // Return the time 0 put price
    return binomialTree[0][0].optionPrice;
}

/* ---------------- DigitalCall class definition ----------------- */

class DigitalCall {
private:
    // Basic values of a stock option
    double S0;         // initial stock price
    double K;          // strike price
    double r;          // risk-free rate
    double sigma;      // volatility
    double T;          // expiration time
    // Inner class used by the binomial tree method
    class Price {
    public:
        double stockPrice;
        double optionPrice;
    };

    // a private helper function
    void put_BinomialTree(string header, const vector<vector<Price>>& bT);

public:
    // constructor defined inline within the class definition
    DigitalCall(double s0, double k, double rfr,
                double v, double et)
            : S0(s0), K(k), r(rfr), sigma(v), T(et)
    {}

    // Calculate the Price of the option
    // using the binomial tree method
    double binomialPrice(int numIntervals);

    // ... other pricing methods can be added here ...

};


/* ---------- DigitalCall member function definition ----------- */

void DigitalCall::put_BinomialTree(string header,
                                   const vector<vector<Price>>& bT)
{
    int N = (bT.size() == 0)
            ? 0
            : bT.size() - 1;

    if (N > 9) {  // if tree is too big, refuse to print anything
        // cout << header << "\n";
        // cout << "BinomialTree has " << N << " levels: too many to print!\n";
        return;
    }

    cout << "\n" << header << "\n\n";
    cout << "BinomialTree with " << N << " time steps:\n\n";
    for (int i(0); i < bT.size(); ++i) {
        cout << "Stock:  ";
        for (int j(0); j < bT[i].size(); ++j) {
            cout << setw(8) << bT[i][j].stockPrice;
        }
        cout << "\n";
        cout << "Option: ";
        for (int j(0); j < bT[i].size(); ++j) {
            cout << setw(8) << bT[i][j].optionPrice;
        }
        cout << "\n\n";
    }
    cout << "\n";
}


double DigitalCall::binomialPrice(int numIntervals)
{
    // time interval length
    double deltaT  = T / numIntervals;;
    // factor by which stock price might rise at each step
    double u 	   = exp(sigma * sqrt(deltaT));
    // factor by which stock price might fall at each step
    double d 	   = 1 / u;
    // risk-free interest rate factor for one time interval
    double a	   = exp(r * deltaT);
    // RN probability of an up move in stock price
    double p	   = (a - d) / (u - d);
    // RN probability of a down move in stock price
    double q	   = 1.0 - p;
    // container for the binomialTree
    vector<vector<Price>> binomialTree;

    // put_BinomialTree("Initial, empty binomialTree:", binomialTree);

    // Build the shape of the binomialTree, by pushing
    // successively longer vector<Price> values (initially
    // all elements 0.0)
    for (int i(0); i <= numIntervals; ++i) {
        vector<Price> vInterval(i+1);    // i+1 {0.0,0.0} values
        binomialTree.push_back(vInterval);
    }
    put_BinomialTree("After filled in with all 0.0:", binomialTree);

    // Fill the stockPrice component of the binomialTree
    for (int i(0); i <= numIntervals; ++i)
        for (int j(0); j <= i; ++j)
            binomialTree[i][j].stockPrice =
                    S0 * pow(u, j) * pow(d, i-j);
    put_BinomialTree("After filled in with stock prices:", binomialTree);

    // Fill the optionPrices at the terminal nodes
    for (int j(0); j <= numIntervals; ++j) {
        if (binomialTree[numIntervals][j].stockPrice>=K)
            binomialTree[numIntervals][j].optionPrice =1;
        if (binomialTree[numIntervals][j].stockPrice<K)
            binomialTree[numIntervals][j].optionPrice =0;
    }
    put_BinomialTree("After filled in with terminal option values:", binomialTree);

    // Now work backwards, filling optionPrices in the rest of the tree
    for (int i(numIntervals-1); i >= 0; --i)
        for (int j(0); j <= i; ++j)
            binomialTree[i][j].optionPrice =
                    exp(-r * deltaT) *
                    (p * binomialTree[i+1][j+1].optionPrice
                     + q * binomialTree[i+1][j].optionPrice);
    put_BinomialTree("After filled in with all option values:", binomialTree);

    // Return the time 0 call price
    return binomialTree[0][0].optionPrice;
}


/* ---------------- DigitalPut class definition ----------------- */

class DigitalPut {
private:
    // Basic values of a stock option
    double S0;         // initial stock price
    double K;          // strike price
    double r;          // risk-free rate
    double sigma;      // volatility
    double T;          // expiration time
    // Inner class used by the binomial tree method
    class Price {
    public:
        double stockPrice;
        double optionPrice;
    };

    // a private helper function
    void put_BinomialTree(string header, const vector<vector<Price>>& bT);

public:
    // constructor defined inline within the class definition
    DigitalPut(double s0, double k, double rfr,
               double v, double et)
            : S0(s0), K(k), r(rfr), sigma(v), T(et)
    {}

    // Calculate the Price of the option
    // using the binomial tree method
    double binomialPrice(int numIntervals);

    // ... other pricing methods can be added here ...

};


/* ---------- DigitalPut member function definition ----------- */

void DigitalPut::put_BinomialTree(string header,
                                  const vector<vector<Price>>& bT)
{
    int N = (bT.size() == 0)
            ? 0
            : bT.size() - 1;

    if (N > 9) {  // if tree is too big, refuse to print anything
        // cout << header << "\n";
        // cout << "BinomialTree has " << N << " levels: too many to print!\n";
        return;
    }

    cout << "\n" << header << "\n\n";
    cout << "BinomialTree with " << N << " time steps:\n\n";
    for (int i(0); i < bT.size(); ++i) {
        cout << "Stock:  ";
        for (int j(0); j < bT[i].size(); ++j) {
            cout << setw(8) << bT[i][j].stockPrice;
        }
        cout << "\n";
        cout << "Option: ";
        for (int j(0); j < bT[i].size(); ++j) {
            cout << setw(8) << bT[i][j].optionPrice;
        }
        cout << "\n\n";
    }
    cout << "\n";
}


double DigitalPut::binomialPrice(int numIntervals)
{
    // time interval length
    double deltaT  = T / numIntervals;;
    // factor by which stock price might rise at each step
    double u 	   = exp(sigma * sqrt(deltaT));
    // factor by which stock price might fall at each step
    double d 	   = 1 / u;
    // risk-free interest rate factor for one time interval
    double a	   = exp(r * deltaT);
    // RN probability of an up move in stock price
    double p	   = (a - d) / (u - d);
    // RN probability of a down move in stock price
    double q	   = 1.0 - p;
    // container for the binomialTree
    vector<vector<Price>> binomialTree;

    // put_BinomialTree("Initial, empty binomialTree:", binomialTree);

    // Build the shape of the binomialTree, by pushing
    // successively longer vector<Price> values (initially
    // all elements 0.0)
    for (int i(0); i <= numIntervals; ++i) {
        vector<Price> vInterval(i+1);    // i+1 {0.0,0.0} values
        binomialTree.push_back(vInterval);
    }
    put_BinomialTree("After filled in with all 0.0:", binomialTree);

    // Fill the stockPrice component of the binomialTree
    for (int i(0); i <= numIntervals; ++i)
        for (int j(0); j <= i; ++j)
            binomialTree[i][j].stockPrice =
                    S0 * pow(u, j) * pow(d, i-j);
    put_BinomialTree("After filled in with stock prices:", binomialTree);

    // Fill the optionPrices at the terminal nodes
    for (int j(0); j <= numIntervals; ++j) {
        if (binomialTree[numIntervals][j].stockPrice<=K)
            binomialTree[numIntervals][j].optionPrice =1;
        if (binomialTree[numIntervals][j].stockPrice>K)
            binomialTree[numIntervals][j].optionPrice =0;
    }
    put_BinomialTree("After filled in with terminal option values:", binomialTree);

    // Now work backwards, filling optionPrices in the rest of the tree
    for (int i(numIntervals-1); i >= 0; --i)
        for (int j(0); j <= i; ++j)
            binomialTree[i][j].optionPrice =
                    exp(-r * deltaT) *
                    (p * binomialTree[i+1][j+1].optionPrice
                     + q * binomialTree[i+1][j].optionPrice);
    put_BinomialTree("After filled in with all option values:", binomialTree);

    // Return the time 0 put price
    return binomialTree[0][0].optionPrice;
}


int main()
{
    int NI = 1000;

    EuropeanCallOption ec9(50.0,     // current stock price, S0
                           50.0,     // option strike price, K
                           0.10,     // risk-free rate
                           0.40,     // stock price volatility
                           0.4167);  // expiration time T (5 months)

    cout << "Euro Call price, with " << NI << " intervals: "
         << ec9.binomialPrice(NI) << "\n";

    EuropeanPutOption ep1( 50.0,     // current stock price, S0
                           50.0,     // option strike price, K
                           0.10,     // risk-free rate
                           0.40,     // stock price volatility
                           0.4167);  // expiration time T (5 months)

    cout << "Euro Put price, with " << NI << " intervals: "
         << ep1.binomialPrice(NI) << "\n";

    DigitalCall dc1(       50.0,     // current stock price, S0
                           50.0,     // option strike price, K
                           0.10,     // risk-free rate
                           0.40,     // stock price volatility
                           0.4167);  // expiration time T (5 months)

    cout << "Digi Call price, with " << NI - 2 << " intervals: "
         << dc1.binomialPrice(NI - 2) << "\n";
    cout << "Digi Call price, with " << NI - 1 << " intervals: "
         << dc1.binomialPrice(NI - 1) << "\n";
    cout << "Digi Call price, with " << NI << " intervals: "
         << dc1.binomialPrice(NI) << "\n";
    cout << "Digi Call price, with " << NI + 1 << " intervals: "
         << dc1.binomialPrice(NI + 1) << "\n";
    cout << "Digi Call price, with " << NI + 2 << " intervals: "
         << dc1.binomialPrice(NI + 2) << "\n";

    DigitalPut dp1(        50.0,     // current stock price, S0
                           50.0,     // option strike price, K
                           0.10,     // risk-free rate
                           0.40,     // stock price volatility
                           0.4167);  // expiration time T (5 months)

    cout << "Digi Put price, with " << NI - 2 << " intervals: "
         << dp1.binomialPrice(NI - 2) << "\n";
    cout << "Digi Put price, with " << NI - 1 << " intervals: "
         << dp1.binomialPrice(NI - 1) << "\n";
    cout << "Digi Put price, with " << NI << " intervals: "
         << dp1.binomialPrice(NI) << "\n";
    cout << "Digi Put price, with " << NI + 1 << " intervals: "
         << dp1.binomialPrice(NI + 1) << "\n";
    cout << "Digi Put price, with " << NI + 2 << " intervals: "
         << dp1.binomialPrice(NI + 2) << "\n";

}