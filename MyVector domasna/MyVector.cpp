#include <iostream>
#include <stdexcept>

using namespace std;

class MyVector
{
    size_t vsize, vcap;
    int *vec;

public:

    MyVector (const size_t sz = 0, const int value = 0)
    {
        vsize = sz;
        vcap = 8;

        while (vcap < sz) vcap <<= 1;

        vec = new int[vcap];

        for (int i = 0; i < sz; i++) vec[i] = value;
    }

    MyVector (const MyVector& x)
    {
        vsize = x.vsize;
        vcap = x.vcap;

        vec = new int[vcap];
        for (size_t i = 0; i < vsize; i++) vec[i] = x.vec[i];
    }

    ~MyVector () {delete[] vec;}

    size_t size() const {return vsize;}
    size_t capacity() const {return vcap;}
    bool empty() const {return !vsize;}

    void resize(const size_t sz, const int value = 0)
    {
        int *x = new int[sz];

        for (size_t i = 0; i < sz && i < vsize; i++) x[i] = vec[i];
        for (size_t i = vsize; i < sz; i++) x[i] = value;

        delete[] vec;

        vec = x;
        vsize = vcap = sz;

    }

    void recap(size_t cap)
    {
        if (cap == vcap) return;
        if (cap < vsize) throw length_error("New capacity cannot be smaller than vector size");

        int *x = new int[cap];
        for (int i = 0; i < vsize; i++) x[i] = vec[i];

        delete[] vec;

        vec = x;
        vcap = cap;
    }

    void push(int value)
    {
        if (vsize == vcap) recap(vcap << 1);
        vec[vsize] = value;
        vsize ++;
    }

    bool pop()
    {
        if (empty()) return false;
        vsize --;
        return true;
    }

    int& back()
    {
        if (empty()) throw out_of_range("Vector is empty");
        return vec[vsize - 1];
    }

    int& at(const size_t n)
    {
        if (n >= vsize) throw out_of_range("Index is out of range");
        return vec[n];
    }

    void fit () {recap(vsize);};

    void PrintVector() const
    {
        cout << "vector(" << vsize << ", " << vcap << "): [";
        for (size_t i = 1; i < vsize; i++)
        {
            cout << vec[i];
            if (i + 1 < vsize) cout << ", ";
        }
        cout << "] \n";
    }

};

int main()
{
    MyVector v1;

    cout << "Initial vector:\n";
    v1.PrintVector();

    v1.push(10);
    v1.push(20);
    v1.push(30);

    cout << "After push:\n";
    v1.PrintVector();

    cout << "Size: " << v1.size() << endl;
    cout << "Capacity: " << v1.capacity() << endl;
    cout << "Empty: " << v1.empty() << endl;

    v1.at(1) = 50;
    cout << "After changing element at index 1:\n";
    v1.PrintVector();

    cout << "Last element: " << v1.back() << endl;

    v1.pop();
    cout << "After pop:\n";
    v1.PrintVector();

    v1.resize(5, 7);
    cout << "After resize to 5:\n";
    v1.PrintVector();

    v1.recap(10);
    cout << "After recap to 10:\n";
    v1.PrintVector();

    v1.fit();
    cout << "After fit:\n";
    v1.PrintVector();

    MyVector v2(v1);
    cout << "Copied vector:\n";
    v2.PrintVector();

    v2.at(0) = 999;

    cout << "Original vector:\n";
    v1.PrintVector();

    cout << "Modified copy:\n";
    v2.PrintVector();

    MyVector v3;
    cout << "Pop on empty vector: " << v3.pop() << endl;

    return 0;
}