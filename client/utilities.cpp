
#include "utilities.h"

using namespace std;

//parses floats from a string
//stolen from Dr. Boshart
void parseFloats(std::string str, float *values){

   int sz = str.size();

   int loc = 0;
   int token_index = 0;
   while (loc < sz){

      int index = str.find(',', loc);

      if (index == -1){

         index = sz-1;

      }

      std::string sub = str.substr(loc, index);
      values[token_index] = atof(sub.c_str());

      loc = index + 1;
      token_index++;
   }

}

void parseInts(std::string str, int *values){

   int sz = str.size();

   int loc = 0;
   int token_index = 0;
   while (loc < sz){

      int index = str.find(',', loc);

      if (index == -1){

         index = sz-1;

      }

      std::string sub = str.substr(loc, index);
      values[token_index] = atoi(sub.c_str());

      loc = index + 1;
      token_index++;
   }

}

char *itoa(int value){

   stringstream ss;
   ss << value;
   return const_cast<char*>(ss.str().c_str());

}

char *ftoi(float value){

   stringstream ss;
   ss << value;
   return const_cast<char*>(ss.str().c_str());

}

char *dtoa(double value){

   stringstream ss;
   ss << value;
   return const_cast<char*>(ss.str().c_str());

}
