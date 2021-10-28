#include "LAAT.ih"

/*
  Warn the user of possibly unwanted behaviour in the algorithm.
 */
void printWarning(size_t amountRuns)
{
  cout << "\nwarning: ants got lost on " << lostAnts << " out of " << amountRuns << " total runs.\n"
       << "ants get lost when there aren't enough data points nearby, this could mean that your data is to sparse or the algorithms hyper-parameters need to be adjusted.\n\n";
}
