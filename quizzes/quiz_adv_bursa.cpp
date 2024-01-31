/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Subject: Advance Bursa Profit     *
**************************************/

#include <iostream>
#include <cassert>
#include <bits/stdc++.h>
using namespace std;
 
int MaxProfitBursa(vector<int>& Prices, int n)
{
    vector<vector<int> > PricesMatrix(Prices.size() + 1, vector<int>(3));
    PricesMatrix[0][0] = -Prices[0];
    for (int i = 1; i <= n; i++)
    {
        // Maximum of buy state profit till previous day or
        // buying a new stock with cooldown state of previous day
        PricesMatrix[i][0] = max(PricesMatrix[i - 1][0], PricesMatrix[i - 1][2] - Prices[i]);

        // Maximum of sold state profit till previous day or
        // selling the stock on current day with buy state of previous day
        PricesMatrix[i][1] = max(PricesMatrix[i - 1][1], PricesMatrix[i - 1][0] + Prices[i]);

        // Sold state of previous day
        PricesMatrix[i][2] = PricesMatrix[i - 1][1];
    }
      // return Sold state profit of final day
    return PricesMatrix[n - 1][1];          
}
 
int main(void)
{
    vector<int> Prices = {8, 2, 6, 2, 6, 3, 7};
    cout << MaxProfitBursa(Prices, Prices.size()) << endl;
    return 0;
}
