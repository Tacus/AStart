//
// Created by 张国兵 on 2019/10/4.
//

#ifndef TESTCLOCK_ASTART_H
#define TESTCLOCK_ASTART_H

extern "C"
{
    extern long AStartAlg(int startx, int starty, int endx, int endy, int with, int height);
    extern long DFSAlg(int startx, int starty, int endx, int endy, int with, int height);
    extern long BFSAlg(int startx, int starty, int endx, int endy, int with, int height);
}
#endif //TESTCLOCK_ASTART_H
