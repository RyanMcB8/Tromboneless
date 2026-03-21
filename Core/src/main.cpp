#include "main.hpp"

/* Main loop */
int main(){
  std::cout << "Running ...\n";
  Octaves synth;
  
  std::cout << "Octave 8, B: " << synth.octaves[8].getB() << "\n";

  return 0;
}
