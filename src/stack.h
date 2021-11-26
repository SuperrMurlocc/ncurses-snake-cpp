#include "cpoint.h"

using namespace std;

#define STARTING_STACK_SIZE 20

class PointStack {
    private:
        CPoint * stack;
        int top;
        int size;

        void increase_stack_size() {
            auto * temp = new CPoint[2 * this->size];
            this->size *= 2;
            memcpy(temp, this->stack, this->top * sizeof(CPoint));
            delete[] this->stack;
            this->stack = temp;
        };
    public:
        // constructor
        PointStack() {
            this->stack = new CPoint [STARTING_STACK_SIZE];
            this->top = 0;
            this->size = STARTING_STACK_SIZE;
        };
        // deconstructor
        ~PointStack() {
            delete[] this->stack;
        };
        // copy constructor
        PointStack(PointStack & pstack) {
            this->stack = new CPoint [pstack.size];

            for (int i = 0; i < pstack.top; i++) {
                this->stack[i] = pstack.stack[i];
            }

            this->top = 0;
            this->size = pstack.size;
        };
        // push
        PointStack & push(const CPoint & cpoint) {
            if (this->top == this->size) {
                increase_stack_size();
            }

            for (int i = this->top; i > 0; i--) {
                this->stack[i] = this->stack[i-1];
            }

            this->stack[0] = CPoint(cpoint.x, cpoint.x);
            this->top++;

            return *this;
        };
        // pop
        void pop() {
            this->top--;
            this->stack[this->top] = 0;
        };
        // operator += CPoint
        PointStack & operator += (const CPoint & cpoint) {
            return this->push(cpoint);
        }
        // size getter
        int getSize() const {
            return this->top;
        }
        // operator []
        CPoint & operator [] (int pos) const {
            if (pos >= this->top)
                throw range_error("");
            else
                return this->stack[pos];
        }
};

