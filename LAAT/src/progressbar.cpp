#include "LAAT.ih"

/*
  Functions implementing a simple progress bar to show
  progress of the iterative step.
*/

void initializeProgressBar(size_t size)
{
  for (size_t loop = 0; loop < size + 1; ++loop)
    cout << '-';
  cout << "]\r[";
  cout.flush();
}

void updateProgressBar(size_t loop)
{
  if (loop % 2) // update every other loop
    cout << '#';
  cout.flush();
}

void completeProgressBar()
{
  cout << "]\n";
  cout.flush();
}
