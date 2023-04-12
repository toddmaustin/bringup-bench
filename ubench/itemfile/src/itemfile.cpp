//**************************************************************
//*  Programmer: Bob Comer
//*
//*  This program allows the user to look up item prices
//*    at a store. The price information is loaded from
//*    a file.
//**************************************************************


#include <iostream>
#include <fstream>

using namespace std;

const int MAX_ITEMS = 200;

void getItems( ifstream&, int[], float[], int&);
int search( int, const int[], int);

int main()
{
   int itemNo[MAX_ITEMS];    // list of item numbers
   float price[MAX_ITEMS];   // list of item prices

   int numItems;             // number of items in the lists
   int srchItemNo;           // an item number to look up
   int position;             // position of an item in list

   ifstream itemFile;        // item price file

   // open Item Price file
   cout << "Opening data file..." << endl;
   itemFile.open("tests/itemfile.txt");
   if (!itemFile)
   {
      cout << "Can not open item file \"items.txt\"" << endl;
      return 1;
   }

// load item price file
   getItems( itemFile, itemNo, price, numItems);

// look up item prices
   cout << "Enter Item Number (or -1 to quit): ";
   cin >> srchItemNo;
   while (srchItemNo > 0)
   {
   // get price for item
      position = search( srchItemNo, itemNo, numItems);
      if (position == -1)
      {
	      cout << "Item " << srchItemNo
              << " is not in the inventory" << endl;
      }
      else
         cout << "Price for item " << srchItemNo
              << " is  $ " << price[position] << endl;

   // get next item
      cout << "Enter Item Number (or -1 to quit): ";
      cin >> srchItemNo;
   }

   return 0;
}

//**************************************************************
//* FUNCTION getItems
//*
//* Copies item information from file into arrays
//**************************************************************

void getItems( ifstream& itemFile,   // in  - item price file
	       int       itemNo[],   // out - list of item numbers
	       float     price[],    // out - list of item prices
	       int&      numItems)   // out - number of items
{
   int   oneItemNo;   // an item number
   float onePrice;       // price for the item

   numItems = 0;
   itemFile >> oneItemNo >> onePrice;
   printf("itemno:%d, price:%f\n", oneItemNo, onePrice);
   while (itemFile && numItems < MAX_ITEMS)
   {
   // move new item into arrays and increment item count
      itemNo[numItems] = oneItemNo;
      price[numItems]  = onePrice;
      numItems++;

   // get next item information
      itemFile >> oneItemNo >> onePrice;
      printf("itemno:%d, price:%f\n", oneItemNo, onePrice);
   }
      
}


//**************************************************************
//*  FUNCTION search
//*
//*  Find the location of an item in a list.
//*  
//**************************************************************

int search(       int srchVal,    // in - value to search for
	    const int list[],     // in - list to search
		  int length)     // in - number of items in list
{
// search list for item and return its position if found
   int index = 0;
   while (index < length)
   {
      if (srchVal == list[index])
	      return index;
      index++;
   }

// item not found - return a flag
   return -1;   
}
