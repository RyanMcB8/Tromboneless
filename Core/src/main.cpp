#include "main.hpp"

/* Main loop */
int main(){
  std::cout << "Running ...\n";
  OctavesWithHarmonics synth;
  std::cout << "Decay: " << synth.Decay(1, 1, synth.note_C) << "\n";
  std::cout << "Octave 3, B: " << synth.octaves[3].getB() << "\n";

  return 0;
}
