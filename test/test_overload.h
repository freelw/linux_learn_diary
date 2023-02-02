#include <iostream>
using namespace std;
class Distance
{
   private:
      int feet;
      int inches;
   public:
      Distance(int f, int i) : feet(f), inches(i){
      }
      friend ostream &operator<<( ostream &output, 
                                       const Distance &D) { 
         output << "F : " << D.feet << " I : " << D.inches;
         return output;            
      }
};

void test_overload() {

    Distance D(1, 2);
    cout << D << endl;
}