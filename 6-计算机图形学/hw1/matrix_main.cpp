#include "/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3/Eigen/Dense"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <map>

template <typename T> class Matrix {
public:
    // default constructor
    Matrix() : rows(0), cols(0) { this->data = nullptr; }

    // constructor with initializer list
    Matrix(int r, int c) : rows(r), cols(c) { this->data = new T[c * r]; }

    Matrix(const Matrix<T>& m) : rows(m.nrow()), cols(m.ncol()) {
        auto length = this->rows * this->cols;
        this->data = new T[length];
        for (int j = 0; j < length; j++) {
            this->data[j] = m[j];
        }
    }

    // destructor
    ~Matrix() { delete[] this->data; }

    int nrow() const { return this->rows; }
    int ncol() const { return this->cols; }

    // check c&r exceeds
    void _check_r_exceeds(int r) const {
        if (r < 0 || r >= this->rows)
            throw std::out_of_range("Index r out of range.");
    }
    void _check_c_exceeds(int c) const {
        if (c < 0 || c >= this->cols)
            throw std::out_of_range("Index c out of range.");
    }
    void _check_rc_exceeds(int r, int c) const {
        this->_check_r_exceeds(r); this->_check_c_exceeds(c);
    }
    void _check_dim_issame(const Matrix<T> &operand) const {
        if (this->rows != operand.nrow() || this->cols != operand.ncol())
            throw std::invalid_argument("Matrix dimensions don't match.");
    }
    void _check_if_divzero(const T &v) const {
        if (v == 0)
            throw std::domain_error("Division by zero.");
    }

    // fast iterates
    template <typename F> void _do_on_every_item (F opers) {
        for (int i = 0; i < this->rows; i++)
            for (int j = 0; j < this->cols; j++)
                opers((*this)(i, j));
    }

    template <typename F> void _do_on_every_item(const Matrix<T> &rhs, F opers) {
        this->_check_dim_issame(rhs);
        for (int i = 0; i < this->rows; i++)
            for (int j = 0; j < this->cols; j++)
                opers((*this)(i, j), rhs(i, j));
    }

    // operator overloading
    T &operator()(int r, int c) const {
        this->_check_rc_exceeds(r, c);
        return this->data[this->cols * r + c];
    }
    T &operator[](int index) const {
        return this->operator()(index / this->cols, index % this->cols);
    }

    Matrix<T> col(int c) {
        this->_check_c_exceeds(c);
        auto col_copy = Matrix<T>(this->rows, 1);
        for (int i = 0; i < this->rows; i++)
            col_copy(i, 0) = (*this)(i, c);
        return col_copy;
    }
    Matrix<T> row(int r) {
        this->_check_r_exceeds(r);
        auto row_copy = Matrix<T>(1, this->cols);
        for (int i = 0; i < this->cols; i++)
            row_copy(0, i) = (*this)(r, i);
        return row_copy;
    }
    Matrix<T> submat(int i1, int j1, int i2, int j2) {
        this->_check_rc_exceeds(i1, j1);
        this->_check_rc_exceeds(i2, j2);
        auto sub = Matrix<T>(i2 - i1 + 1, j2 - j1 + 1);
        for (int i = 0; i < i2 - i1 + 1; i++)
            for (int j = 0; j < j2 - j1 + 1; j++)
                sub(i, j) = (*this)(i + i1, j + j1);
        return sub;
    }

    // ant alias
    Matrix<T> &operator=(const Matrix<T> &rhs) {
        if (this == &rhs)
            return *this;
        this->_do_on_every_item(rhs, [=](T &item, T r_item) { item = r_item; });
        return *this;
    }

    Matrix<T> operator+(const Matrix<T> &rhs) {
        auto res = Matrix<T>(this->rows, this->cols);
        res = *this;
        res._do_on_every_item(rhs, [=](T &item, T r_item) { item += r_item; });
        return res;
    }
    Matrix<T> operator-(const Matrix<T> &rhs) {
        auto res = Matrix<T>(this->rows, this->cols);
        res = *this;
        res._do_on_every_item(rhs, [=](T &item, T r_item) { item -= r_item; });
        return res;
    }
    Matrix<T> operator*(const Matrix<T> &rhs) {
        // Hadamard Product
        auto res = Matrix<T>(this->rows, this->cols);
        res = *this;
        res._do_on_every_item(rhs, [=](T &item, T r_item) { item *= r_item; });
        return res;
    }
    Matrix<T> operator/(const Matrix<T> &rhs) {
        auto res = Matrix<T>(this->rows, this->cols);
        res = *this;
        res._do_on_every_item(rhs, [=](T &item, T r_item) {
            this->_check_if_divzero(r_item);
            item /= r_item;
        });
        return res;
    }

    Matrix<T> &operator+=(const Matrix<T> &rhs) {
        this->_do_on_every_item(rhs, [=](T &item, T r_item) { item += r_item; });
        return *this;
    }
    Matrix<T> &operator-=(const Matrix<T> &rhs) {
        this->_do_on_every_item(rhs, [=](T &item, T r_item) { item -= r_item; });
        return *this;
    }
    Matrix<T> &operator*=(const Matrix<T> &rhs) {
        this->_do_on_every_item(rhs, [=](T &item, T r_item) { item *= r_item; });
        return *this;
    }
    Matrix<T> &operator/=(const Matrix<T> &rhs) {
        this->_do_on_every_item(rhs, [=](T &item, T r_item) {
            this->_check_if_divzero(r_item);
            item /= r_item;
        });
        return *this;
    }

    Matrix<T> operator+(T v) {
        auto res = Matrix<T>(this->rows, this->cols);
        res._do_on_every_item(*this, [=](T &item, T r_item) { item = r_item + v; });
        return res;
    }
    Matrix<T> operator-(T v) {
        auto res = Matrix<T>(this->rows, this->cols);
        res._do_on_every_item(*this, [=](T &item, T r_item) { item = r_item - v; });
        return res;
    }
    Matrix<T> operator*(T v) {
        auto res = Matrix<T>(this->rows, this->cols);
        res._do_on_every_item(*this, [=](T &item, T r_item) { item = r_item * v; });
        return res;
    }
    Matrix<T> operator/(T v) {
        this->_check_if_divzero(v);
        auto res = Matrix<T>(this->rows, this->cols);
        res._do_on_every_item(*this, [=](T &item, T r_item) { item = r_item / v; });
        return res;
    }

    Matrix<T> &operator+=(T v) {
        this->_do_on_every_item([=](T &item) { item += v; });
        return *this;
    }
    Matrix<T> &operator-=(T v) {
        this->_do_on_every_item([=](T &item) { item -= v; });
        return *this;
    }
    Matrix<T> &operator*=(T v) {
        this->_do_on_every_item([=](T &item) { item *= v; });
        return *this;
    }
    Matrix<T> &operator/=(T v) {
        this->_check_if_divzero(v);
        this->_do_on_every_item([=](T &item) { item /= v; });
        return *this;
    }

    void print() {
        printf("this Matrix has size (%d x %d)\n", this->rows, this->cols);
        printf("the entries are:\n");

        for (int i = 0; i < this->rows; i++) {
            std::cout << "| ";
            for (int j = 0; j < this->cols; j++)
                std::cout << std::setw(5) << std::right << (*this)(i, j);
            std::cout << " |" << std::endl;
        }
    }

    void print(const char* s) {
        std::cout << "---------------------------------------\n"
                  << "The information of Matrix [ " << s << " ]\n"
                  << "---------------------------------------\n";
        this->print();
    }

private:
    int rows, cols;
    T *data;
};

// BONUS: write a sparse Matrix class
template <typename R> class SparseMatrix {
public:
    // default constructor
    SparseMatrix() : rows(0), cols(0) {}

    // constructor with initializer list
    SparseMatrix(int r, int c) : rows(r), cols(c) {}

    // destructor
    ~SparseMatrix() = default;

    int nrow() const { return this->rows; }
    int ncol() const { return this->cols; }

    // check c&r exceeds
    void _check_r_exceeds(int r) {
        if (r < 0 || r >= this->rows)
            throw std::out_of_range("Index r out of range.");
    }
    void _check_c_exceeds(int c) {
        if (c < 0 || c >= this->cols)
            throw std::out_of_range("Index c out of range.");
    }
    void _check_rc_exceeds(int r, int c) {
        this->_check_r_exceeds(r); this->_check_c_exceeds(c);
    }
    void _check_dim_issame(SparseMatrix<R> &operand) {
        if (this->rows != operand.nrow() || this->cols != operand.ncol())
            throw std::invalid_argument("Matrix dimensions don't match.");
    }
    void _check_if_divzero(R &v) {
        if (v == 0)
            throw std::domain_error("Division by zero.");
    }

    void assign(int r, int c, R v) {
        if (this->data.contains(r)) {
            auto line = this->data.at(r);
            for (auto it = line.begin(); it != line.end(); it++) {
                if (it->first == c) {
                    if (v == 0) {
                        line.erase(it);
                        return;
                    }
                    it->second = v;
                    return;
                }
            }
            this->data.at(r).push_back(std::make_pair(c, v));
            return;
        }
        if (v != 0)
            this->data[r].push_back(std::make_pair(c, v));
    }

    // operator overloading
    R operator()(int r, int c) {
        this->_check_rc_exceeds(r, c);
        if (this->data.contains(r))
            for (auto item : this->data.at(r))
                if (item.first == c)
                    return item.second;
        return 0;
    }

    SparseMatrix<R> col(int c) {
        this->_check_c_exceeds(c);
        auto col_copy = SparseMatrix<R>(this->rows, 1);
        for (int i = 0; i < this->rows; i++) {
            auto value = (*this)(i, c);
            if (value) col_copy.assign(i, 0, value);
        }
        return col_copy;
    }
    SparseMatrix<R> row(int r) {
        this->_check_r_exceeds(r);
        auto row_copy = SparseMatrix<R>(this->rows, 1);
        if (this->data.contains(r))
            for (auto item : this->data.at(r))
                row_copy.assign(r, item.first, item.second);
        return row_copy;
    }
    SparseMatrix<R> submat(int i1, int j1, int i2, int j2) {
        this->_check_rc_exceeds(i1, j1);
        this->_check_rc_exceeds(i2, j2);
        auto sub = SparseMatrix<R>(i2 - i1 + 1, j2 - j1 + 1);
        for (auto line : this->data)
            if (line.first >= i1 && line.first <= i2)
                for (auto item : line)
                    if (item.first >= j1 && item.first <= j2)
                        sub.assign(line.first - i1, item.first - j1, item.second);
        return sub;
    }

    // ant alias
    SparseMatrix<R> &operator=(SparseMatrix<R> &rhs) {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->assign(i, j, rhs(i, j));
            }
        }
        return *this;
    }

    SparseMatrix<R> operator+(SparseMatrix<R> &rhs) {
        auto res = SparseMatrix<R>(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                res.assign(i, j, (*this)(i, j) + rhs(i, j));
            }
        }
        return res;
    }
    SparseMatrix<R> operator-(SparseMatrix<R> &rhs) {
        auto res = SparseMatrix<R>(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                res.assign(i, j, (*this)(i, j) - rhs(i, j));
            }
        }
        return res;
    }
    SparseMatrix<R> operator*(SparseMatrix<R> &rhs) {
        auto res = SparseMatrix<R>(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                res.assign(i, j, (*this)(i, j) * rhs(i, j));
            }
        }
        return res;
    }
    SparseMatrix<R> operator/(SparseMatrix<R> &rhs) {
        auto res = SparseMatrix<R>(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->_check_if_divzero(rhs(i, j));
                res.assign(i, j, (*this)(i, j) / rhs(i, j));
            }
        }
        return res;
    }

    SparseMatrix<R> &operator+=(SparseMatrix<R> &rhs) {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->assign(i, j, (*this)(i, j) + rhs(i, j));
            }
        }
        return *this;
    }
    SparseMatrix<R> &operator-=(SparseMatrix<R> &rhs) {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->assign(i, j, (*this)(i, j) - rhs(i, j));
            }
        }
        return *this;
    }
    SparseMatrix<R> &operator*=(SparseMatrix<R> &rhs) {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->assign(i, j, (*this)(i, j) * rhs(i, j));
            }
        }
        return *this;
    }
    SparseMatrix<R> &operator/=(SparseMatrix<R> &rhs) {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->_check_if_divzero(rhs(i, j));
                this->assign(i, j, (*this)(i, j) + rhs(i, j));
            }
        }
        return *this;
    }

    SparseMatrix<R> operator+(R v) {
        auto res = SparseMatrix<R>(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                res.assign(i, j, (*this)(i, j) + v);
            }
        }
        return res;
    }
    SparseMatrix<R> operator-(R v) {
        auto res = SparseMatrix<R>(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                res.assign(i, j, (*this)(i, j) - v);
            }
        }
        return res;
    }
    SparseMatrix<R> operator*(R v) {
        auto res = SparseMatrix<R>(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                res.assign(i, j, (*this)(i, j) * v);
            }
        }
        return res;
    }
    SparseMatrix<R> operator/(R v) {
        this->_check_if_divzero(v);
        auto res = SparseMatrix<R>(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                res.assign(i, j, (*this)(i, j) / v);
            }
        }
        return res;
    }

    SparseMatrix<R> &operator+=(R v) {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->assign(i, j, (*this)(i, j) + v);
            }
        }
        return *this;
    }
    SparseMatrix<R> &operator-=(R v) {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->assign(i, j, (*this)(i, j) - v);
            }
        }
        return *this;
    }
    SparseMatrix<R> &operator*=(R v) {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->assign(i, j, (*this)(i, j) * v);
            }
        }
        return *this;
    }
    SparseMatrix<R> &operator/=(R v) {
        this->_check_if_divzero(v);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->assign(i, j, (*this)(i, j) / v);
            }
        }
        return *this;
    }

    void print() {
        printf("this SparseMatrix has size (%d x %d)\n", this->rows, this->cols);
        printf("the entries are:\n");

        bool flag = false;
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++)
                if ((*this)(i, j)) {
                    flag = true;
                    std::cout << "「 " << i << ", " << j << ", " << (*this)(i, j) << " 」";
                }
        }
        if (!flag) std::cout << "Empty!";
        std::cout << std::endl;
    }

    void print(const char* s) {
        std::cout << "---------------------------------------\n"
                  << "The information of Matrix [ " << s << " ]\n"
                  << "---------------------------------------\n";
        this->print();
    }

private:
    int rows, cols;
    std::map<int, std::vector<std::pair<int, R>>> data;
};

void MatrixTest_Correctness() {
    // using the timestamp as random seed
    auto now_time = std::chrono::system_clock::now();
    auto now_timestamp = std::chrono::system_clock::to_time_t(now_time);
    std::mt19937 gen(now_timestamp);
    std::uniform_int_distribution<int> distribution(1, 20);

    // 2 operands for matrix correctness verification
    Matrix<int> A(6, 7), B(6, 7);

    // fill matrices with random values
    for (int i = 0; i < A.nrow(); i++)
        for (int j = 0; j < A.ncol(); j++) {
            A(i, j) = distribution(gen);
            B(i, j) = distribution(gen);
        }

    // calculate: (A * B) / (A + 2)
    A.print("A");
    B.print("B");
    auto C = A * B;
    auto D = A + 2;
    C.print("C = A * B");
    D.print("D = A + 2");
    auto E = C / D;
    E.print("E = C / D");
    auto F = A.submat(1, 2, 3, 4);
    auto G = A.row(4);
    F.print("F = A[1, 2, 3, 4]");
    G.print("G = A.r[4]");
}

void MatrixTest_Performance() {
    // using the timestamp as random seed
    auto now_time = std::chrono::system_clock::now();
    auto now_timestamp = std::chrono::system_clock::to_time_t(now_time);
    std::mt19937 gen(now_timestamp);
    std::uniform_int_distribution<int> distribution(1, 20);

    // 2 operands for matrix performance test
    Matrix<double> D1(100, 100), D2(100, 100);

    // fill matrices with random values
    for (int i = 0; i < D1.nrow(); i++)
        for (int j = 0; j < D1.ncol(); j++) {
            D1(i, j) = distribution(gen) / 1.05f;
            D2(i, j) = distribution(gen) / 1.05f;
        }

    // after verifying correctness, here comes performance
    // calculate: (D1 * D2) / (D1 + 2)
    auto start = std::chrono::high_resolution_clock::now();
    auto D3 = D1 * D2;
    auto D4 = D1 + 2;
    auto D5 = D3 / D4;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "My matrix calculating time: " << duration.count() << " ns.\n";

    // use Eigen and compare
    // first, create the same elements
    Eigen::Matrix<double, 100, 100> E1, E2;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            E1(i, j) = D1(i, j);
            E2(i, j) = D2(i, j);
        }
    }
    // calculate: (E1 * E2) / (E1 + 2)
    start = std::chrono::high_resolution_clock::now();
    auto E3 = E1.cwiseProduct(E2);
    auto __E1 = E1.array() + 2;
    auto E4 = __E1.matrix();
    auto E5 = E3.cwiseQuotient(E4);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Eigen matrix calculating time: " << duration.count() << " ns.\n";
}

void SparseMatrixTest() {
    // using the timestamp as random seed
    auto now_time = std::chrono::system_clock::now();
    auto now_timestamp = std::chrono::system_clock::to_time_t(now_time);
    std::mt19937 gen(now_timestamp);
    std::uniform_int_distribution<int> distribution(1, 10);

    SparseMatrix<int> SA(10, 10), SB(10, 10);
    for (int i = 0; i < SA.nrow(); i++) {
        for (int j = 0; j < SA.ncol(); j++) {
            if (distribution(gen) > 9) {
                auto value = distribution(gen);
                SA.assign(i, j, value);
            }
            if (distribution(gen) > 9) {
                auto value = distribution(gen);
                SB.assign(i, j, value);
            }
        }
    }
    SA.print("SA");
    SB.print("SB");
    auto SC = SA * SB;
    SC.print("SC = SA * SB");
}

int main() {
    MatrixTest_Correctness();
    SparseMatrixTest();

    std::cout << "\n\n";

    MatrixTest_Performance();
    return 0;
}
