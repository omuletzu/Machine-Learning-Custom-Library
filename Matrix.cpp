#include "Matrix.h"
#include "math.h"

bool check_bound(int i, int j, unsigned int row, unsigned int col){
    return i < row && i >= 0 && j < col && j >= 0;
}

double Matrix :: at(int i, int j) {
    if(check_bound(i, j, this->row, this->col)){
        return this->matrix[i][j];
    }

    return 0.0;
}

void Matrix :: set(int i, int j, double value) {
    if(check_bound(i, j, this->row, this->col)){
        this->matrix[i][j] = value;
    }
}

std::vector<std::vector<double>> Matrix :: get_matrix() {
    return this->matrix;
}

std::pair<size_t, size_t> Matrix :: get_row_col() {
    return {this->row, this->col};
}


Matrix Matrix :: add_matrix(const Matrix& a, const Matrix& b) {
    if(a.matrix.size() != b.matrix.size()){
        return {0, 0};
    }

    if(a.matrix[0].size() != b.matrix[0].size()){
        return {0, 0};
    }

    Matrix result = a;

    for(int i = 0; i < b.matrix.size(); i++){
        for(int j = 0; j < b.matrix[i].size(); j++){
            result.matrix[i][j] += b.matrix[i][j];
        }
    }

    return result;
}

Matrix Matrix :: sub_matrix(const Matrix& a, const Matrix& b) {
    if(a.matrix.size() != b.matrix.size()){
        return {0, 0};
    }

    if(a.matrix[0].size() != b.matrix[0].size()){
        return {0, 0};
    }

    Matrix result = a;

    for(int i = 0; i < b.matrix.size(); i++){
        for(int j = 0; j < b.matrix[i].size(); j++){
            result.matrix[i][j] -= b.matrix[i][j];
        }
    }

    return result;
}

Matrix Matrix :: mul_matrix(const Matrix& a, const Matrix& b) {
    if(a.matrix[0].size() != b.matrix.size()){
        return {0, 0};
    }

    Matrix result(a.matrix.size(), b.matrix[0].size());

    for(int i = 0; i < a.matrix.size(); i++){
        for(int j = 0; j < b.matrix[0].size(); j++){
            for(int k = 0; k < b.matrix.size(); k++){
                result.matrix[i][j] += a.matrix[i][k] * b.matrix[k][j];
            }
        }
    }

    return result;
}

Matrix Matrix :: div_matrix(const Matrix& a, const Matrix& b) {
    if(a.matrix.size() != b.matrix.size()){
        return {0, 0};
    }

    if(a.matrix[0].size() != b.matrix[0].size()){
        return {0, 0};
    }

    Matrix result = a;

    for(int i = 0; i < b.matrix.size(); i++){
        for(int j = 0; j < b.matrix[i].size(); j++){
            result.matrix[i][j] /= b.matrix[i][j];
        }
    }

    return result;
}

Matrix Matrix :: mul_simple_matrix(const Matrix &a, const Matrix &b) {
    if(a.matrix.size() != b.matrix.size()){
        return {0, 0};
    }

    if(a.matrix[0].size() != b.matrix[0].size()){
        return {0, 0};
    }

    Matrix result = a;

    for(int i = 0; i < b.matrix.size(); i++){
        for(int j = 0; j < b.matrix[i].size(); j++){
            result.matrix[i][j] *= b.matrix[i][j];
        }
    }

    return result;
}

Matrix Matrix::transpose(const Matrix &a) {
    Matrix result(a.matrix[0].size(), a.matrix.size());

    for(int i = 0; i < a.row; i++){
        for(int j = 0; j < a.col; j++){
            result.matrix[j][i] = a.matrix[i][j];
        }
    }

    return result;
}

void Matrix::reLU_activation(Matrix& value) {
    for(auto & i : value.matrix){
        for(double & j : i){
            j = (j > 0 ? j : 0);
        }
    }
}

Matrix Matrix :: reLU_derivative(Matrix value) {
    for(auto & i : value.matrix){
        for(double & j : i){
            j = (j > 0 ? 1 : 0);
        }
    }

    return value;
}

void Matrix::sigmoid_activation(Matrix& value) {
    for(auto & i : value.matrix){
        for(double & j : i){
            j = 1 / (1 + exp(-1 * j));
        }
    }
}

Matrix Matrix :: sigmoid_derivative(Matrix value) {
    for(auto & i : value.matrix){
        for(double & j : i){
            double sigmoid = 1 / (1 + exp(-1 * j));
            j = sigmoid * (1 - sigmoid);
        }
    }

    return value;
}

void Matrix::softmax_activation(Matrix& value) {
    for(int i = 0; i < value.matrix[0].size(); i++){

        double max_value = -INFINITY;

        for(auto & j : value.matrix){
            max_value = std :: max(j[i], max_value);
        }

        double exp_sum = 0.0;

        for(auto & j : value.matrix){
            j[i] = exp(j[i] - max_value);
            exp_sum += j[i];
        }

        for(auto & j : value.matrix){
            j[i] /= exp_sum;
        }
    }
}

Matrix Matrix :: softmax_derivative(Matrix value) {
    Matrix derived_matrix = value;
    Matrix :: softmax_activation(derived_matrix);

    Matrix full_one_matrix = Matrix(derived_matrix.matrix.size(), derived_matrix.matrix[0].size(), 1.0);

    derived_matrix = Matrix :: mul_simple_matrix(derived_matrix, Matrix :: sub_matrix(full_one_matrix, derived_matrix));

    return derived_matrix;
}

Matrix::Matrix() {}
