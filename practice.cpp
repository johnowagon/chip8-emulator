#include <iostream>

using namespace std;

int main(){
  int days = 0;
  int hours = 0;
  int minutes = 0;
  int seconds = 0;
  long Iseconds = 0;

  printf("Please enter an integer for seconds:\n");
  cin >> Iseconds;
  int d = Iseconds / (24 * 60 * 60);
  int mins = (Iseconds - (d * (24 * 60 * 60))) / 60;
  int secs = (Iseconds - (d * (24 * 60 * 60)) - (mins * 60)) / 60;

  printf("%d seconds = %d days, %d minutes, %d seconds", Iseconds, d, mins, secs);
}
