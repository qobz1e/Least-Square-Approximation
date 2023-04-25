//Fanis Zinnurov
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

class Matrix {
public:
    int rows = 0;
    int cols = 0;
    double **data;
    Matrix() {}
    Matrix(int n, int m) {
        rows = n;
        cols = m;
        data = new double*[n];
        for (int i = 0; i < n; i++) {
            data[i] = new double[m];
        }
    }

    friend ostream& operator<<(ostream& out, const Matrix& matrix) {
        out << fixed << setprecision(4);
        for (int i = 0; i < matrix.rows; i++) {
            for (int j = 0; j < matrix.cols; j++) {
                if (matrix.data[i][j] < 1e-10 && matrix.data[i][j] > -1e-10) {
                    out << "0.0000";
                }
                else {
                    out << matrix.data[i][j];
                }
                if (j != matrix.cols - 1) { out << " "; }
            }
            out << endl;
        }
        return out;
    }

    friend istream& operator>>(istream& input, Matrix matrix) {
        for (int i = 0; i < matrix.rows; i++) {
            for (int j = 0; j < matrix.cols; j++) {
                input >> matrix.data[i][j];
            }
        }
        return input;
    }

    Matrix& operator=(const Matrix& other) {
        if (&other == this) {
            return *this;
        }
        rows = other.rows;
        cols = other.cols;
        data = other.data;
        return *this;
    }

    Matrix operator+(Matrix other) {
        if (rows != other.rows || cols != other.cols) {
            cout << "Error: the dimensional problem occurred" << endl;
            return Matrix();
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    Matrix operator-(Matrix other) {
        if (rows != other.rows || cols != other.cols) {
            cout << "Error: the dimensional problem occurred" << endl;
            return Matrix();
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }

    Matrix operator*(Matrix other) {
        if (cols != other.rows) {
            cout << "Error: the dimensional problem occurred"  << endl;
            return Matrix();
        }
        Matrix result(rows, other.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                result.data[i][j] = 0;
                for (int s = 0; s < cols; s++) {
                    result.data[i][j] += data[i][s] * other.data[s][j];
                }
            }
        }
        return result;
    }

    Matrix transpose() {
        // Create a new matrix with dimensions swapped
        Matrix transposed(cols, rows);

        // Copy elements into the new matrix, transposing them
        for (int i = 0; i < transposed.rows; i++) {
            for (int j = 0; j < transposed.cols; j++) {
                transposed.data[i][j] = data[j][i];
            }
        }

        return transposed;
    }
};

class SquareMatrix: public Matrix {
public:
    SquareMatrix() {
        rows = 0;
        cols = 0;
    };
    SquareMatrix(int size) {
        rows = size;
        cols = size;
        data = new double*[size];
        for (int i = 0; i < size; i++) {
            data[i] = new double[size];
        }
    };

    friend ostream& operator<<(ostream& out, const SquareMatrix& matrix) {
        out << fixed << setprecision(4);
        for (int i = 0; i < matrix.rows; i++) {
            for (int j = 0; j < matrix.cols; j++) {
                if (matrix.data[i][j] < 1e-10 && matrix.data[i][j] > -1e-10) {
                    out << "0.0000";
                }
                else {
                    out << matrix.data[i][j];
                }
                if (j != matrix.cols - 1) { out << " "; }
            }
            out << endl;
        }
        return out;
    }

    friend istream& operator>>(istream& input, const SquareMatrix& matrix) {
        for (int i = 0; i < matrix.rows; i++) {
            for (int j = 0; j < matrix.cols; j++) {
                input >> matrix.data[i][j];
            }
        }
        return input;
    }

    SquareMatrix operator+(SquareMatrix other) {
        auto first = (Matrix*)this;
        auto second = (Matrix*)&other;
        Matrix res = *first + *second;
        return *(SquareMatrix*) &res;
    }

    SquareMatrix operator-(SquareMatrix other) {
        auto first = (Matrix*)this;
        auto second = (Matrix*)&other;
        Matrix res = *first - *second;
        return *(SquareMatrix*) &res;
    }

    SquareMatrix operator*(SquareMatrix other) {
        auto first = (Matrix*)this;
        auto second = (Matrix*)&other;
        Matrix res = *first * *second;
        return *(SquareMatrix*) &res;
    }

    SquareMatrix transpose() {
        auto m = (Matrix*)this;
        Matrix res = m->transpose();
        return *(SquareMatrix*) &res;
    }
};

class IdentityMatrix: public SquareMatrix {
public:
    IdentityMatrix(int size) {
        rows = size;
        cols = size;
        data = new double*[size];
        for (int i = 0; i < size; i++) {
            data[i] = new double[size];
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i == j) {
                    data[i][j] = 1;
                } else {
                    data[i][j] = 0;
                }
            }
        }
    }
};

class EliminationMatrix: public SquareMatrix {
public:
    EliminationMatrix(int row, int col, SquareMatrix matrix) {
        rows = matrix.rows;
        cols = matrix.cols;
        data = new double*[rows];
        for (int i = 0; i < rows; i++) {
            data[i] = new double[cols];
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (i == row - 1 && j == col - 1) {
                    data[i][j] = -(matrix.data[i][j]/matrix.data[j][j]);
                }
                else if (i == j) {
                    data[i][j] = 1;
                } else {
                    data[i][j] = 0;
                }
            }
        }
    }
};

class PermutationMatrix: public SquareMatrix {
public:
    PermutationMatrix(int size, int row1, int row2) {
        rows = size;
        cols = size;
        data = new double*[size];
        for (int i = 0; i < size; i++) {
            data[i] = new double[size];
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i == j) {
                    data[i][j] = 1;
                } else {
                    data[i][j] = 0;
                }
            }
        }
        auto temp = data[row1 - 1];
        data[row1 - 1] = data[row2 - 1];
        data[row2 - 1] = temp;
    }
};

SquareMatrix inverse(SquareMatrix matrix) {
    SquareMatrix id = IdentityMatrix(matrix.rows);
    cout << fixed << setprecision(2);

    for (int i = 0; i < matrix.rows; i++) {
        double max = matrix.data[i][i];
        int max_row = i;
        for (int j = i + 1; j < matrix.rows; j++) {
            if (abs(matrix.data[j][i]) > abs(max)) {
                max = matrix.data[j][i];
                max_row = j;
            }
        }
        if (max_row != i) {
            PermutationMatrix p(matrix.rows, i + 1, max_row + 1);
            matrix = p * matrix;
            id = p * id;
        }

        for (int j = i + 1; j < matrix.cols; j++) {
            if (matrix.data[j][i] == 0) {
                continue;
            }
            EliminationMatrix e(j + 1, i + 1, matrix);
            matrix = e * matrix;
            id = e * id;
        }
    }

    for (int i = matrix.rows - 1; i > 0; i--) {
        if (matrix.data[i][i] == 0) {
            continue;
        }
        for (int j = i - 1; j >= 0; j--) {
            if (matrix.data[j][j] == 0 || matrix.data[j][i] == 0) {
                continue;
            }
            EliminationMatrix e(j + 1, i + 1, matrix);
            matrix = e * matrix;
            id = e * id;
        }
    }

    for (int i = 0; i < matrix.rows; i++) {
        double scale = 1/matrix.data[i][i];
        matrix.data[i][i] = 1;
        for (int j = 0; j < matrix.rows; j++) {
            id.data[i][j] = id.data[i][j] * scale;
        }
    }

    return id;
}

int main() {
    int m, n;
    cin >> m;
    Matrix data(m, 2);
    cin >> data;
    cin >> n;
    Matrix a(m, n+1);
    Matrix b(m, 1);
    for (int i = 0; i < m; i++) {
        b.data[i][0] = data.data[i][1];
        a.data[i][0] = 1;
        for (int j = 1; j <= n; j++) {
            a.data[i][j] = pow(data.data[i][0], j);
        }
    }
    Matrix a_t = a.transpose();
    cout << "A:" << endl << a;
    Matrix a_ta = a_t * a;
    cout << "A_T*A:" << endl << a_ta;
    SquareMatrix a_ta_inverse = inverse(*(SquareMatrix*) &a_ta);
    cout << "(A_T*A)^-1:" << endl << a_ta_inverse;
    Matrix a_tb = a_t * b;
    cout << "A_T*b:" << endl << a_tb;
    Matrix x = (Matrix)a_ta_inverse * a_tb;
    cout << "x~:" << endl << x;
    return 0;
}