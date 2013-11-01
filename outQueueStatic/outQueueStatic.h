#ifndef OUTQUEUESTATIC_H
#define OUTQUEUESTATIC_H

#include <Arduino.h>
#include <cycler.h>

/////////////////////////////////////
// this is where the length of elements in the queue is defined!!
// length of each elt in bytes
#define ELEN (5)
// nb of elts in the queue
#define QLEN (40)

class outQueueStatic {
  private:
    char queue[QLEN][ELEN]; 
    cycler *head,
           *tail; 
    byte eltCount;

    void resetElt(byte index);
    void setElt(byte index, char *val);

  public:
    outQueueStatic();
    boolean enQ(char *next);
    boolean deQ(char *buf);
    boolean pQ(char *buf) const;  // peek the head of the queue!
};

#endif
