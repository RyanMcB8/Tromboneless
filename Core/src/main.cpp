#include "main.hpp"

/* Main loop */
int main(){
  std::cout << "Running ...\n";
  OctavesWithHarmonics synth;
  for (float t=0; t<1; t+=0.05){
    std::cout << "Play sound: " << synth.EndNoteWithHarmonics(5, 5, synth.note_C, t, t) << "\n";
  }
  std::cout << "Octave 3, B: " << synth.octaves[3].getB() << "\n";

  return 0;
}
