#include "cpoint.h"

using namespace std;

#define STARTING_STACK_SIZE 20

class PointStack {
    private:
        CPoint * stack;
        int top;
        int size;

        void increase_stack_size() {
            CPoint * temp = new CPoint[2 * this->size];
            this->size *= 2;
            memcpy(temp, this->stack, this->top * sizeof(CPoint));
            delete[] this->stack;
            this->stack = temp;
        };
    public:
        // constructor
        PointStack() {
            this->stack = new CPoint[STARTING_STACK_SIZE];
            this->top = 0;
            this->size = STARTING_STACK_SIZE;
        };
        // deconstructor
        ~PointStack() {
            delete[] this->stack;
        };
        // copy constructor
        PointStack(PointStack & pstack) {
            this->stack = new CPoint[pstack.size];

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

            this->stack[this->top] = cpoint;
            this->top++;

            return *this;
        };
        // pop
        void pop() {
            this->top--;
            this->stack[this->top] = 0;
        };
        // operator = PointStack
        PointStack & operator = (const PointStack & pstack) {
            CPoint * temp = new CPoint[pstack.size];

            for (int i = 0; i < pstack.top; i++) {
                this->stack[i] = pstack.stack[i];
            }
            
            if (this->stack != nullptr)
                delete[] this->stack;
            this->stack = temp;

            this->top = 0;
            this->size = pstack.size;

            return *this;
        };
        // operator += CPoint
        PointStack & operator += (const CPoint & cpoint) {
            return this->push(cpoint);
        }
        // operator --
        void operator -- () {
            this->pop();
        }
};

