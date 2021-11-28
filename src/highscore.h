#ifndef __SCORE_H__
#define __SCORE_H__

#include <cstdlib>
#include <ctime>

#include "cpoint.h"
#include "winsys.h"
#include "stack.h"
#include "screen.h"

using namespace std;

class CScore : public CFramedWindow {
    private:
        int size;
        int * scores;
    public:
        explicit CScore(CRect r, char _c = ' ', const char * scores_filename = "scores.bin") : CFramedWindow(r, _c) {
            FILE * fptr = fopen(scores_filename, "ab+");
            if (fptr == NULL) {
                throw
            }

        };
};


#endif // __SCORE_H__
