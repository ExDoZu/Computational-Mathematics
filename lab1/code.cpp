#include <fstream>
#include <iostream>
#include <set>
#include <vector>

void print_matrix(std::vector<std::vector<double>> const &a) {
    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < a[i].size(); j++) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

bool check_current_diagonal_dominance(std::vector<std::vector<double>> const &a) {
    bool flag = false;
    for (size_t i = 0; i < a.size(); i++) {
        double sum = 0;
        for (size_t j = 0; j < a[i].size(); j++) {
            if (i != j) {
                sum += std::abs(a[i][j]);
            }
        }
        if (std::abs(a[i][i]) < sum) {
            return false;
        } else if (std::abs(a[i][i]) > sum) {
            flag = true;
        }
    }
    return flag;
}
bool check_diagonal_dominance(std::vector<std::vector<double>> &a, std::vector<double> &b) {
    if (check_current_diagonal_dominance(a)) {
        return true;
    }
    std::vector<size_t> inds_of_max(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        double sum = 0;
        double max = 0;
        size_t ind_of_max;
        for (size_t j = 0; j < a[i].size(); j++) {
            double const element = std::abs(a[i][j]);
            sum += element;
            if (element > max) {
                max = element;
                ind_of_max = j;
            }
        }
        sum -= max;
        if (max < sum) {
            return false;
        } else {
            inds_of_max[i] = ind_of_max;
        }
    }
    std::set<size_t> s;
    for (size_t i = 0; i < inds_of_max.size(); i++) {
        s.insert(inds_of_max[i]);
    }
    if (s.size() != inds_of_max.size()) {
        return false;
    }
    std::vector<std::vector<double>> const a_old = a;
    std::vector<double> const b_old = b;
    for (size_t i = 0; i < a.size(); i++) {
        a[inds_of_max[i]] = a_old[i];
        b[inds_of_max[i]] = b_old[i];
    }
    return check_current_diagonal_dominance(a);
}

std::vector<std::vector<double>> calculate_c(std::vector<std::vector<double>> const &a) {
    std::vector<std::vector<double>> c(a.size(), std::vector<double>(a.size()));
    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < a[i].size(); j++) {
            if (i == j) {
                c[i][j] = 0;
            } else {
                c[i][j] = -a[i][j] / a[i][i];
            }
        }
    }
    return c;
}

std::vector<double> calculate_d(std::vector<std::vector<double>> const &a, std::vector<double> const &b) {
    std::vector<double> d(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        d[i] = b[i] / a[i][i];
    }
    return d;
}

std::vector<double> calculate_new_xk(std::vector<std::vector<double>> const &c, std::vector<double> const &d, std::vector<double> const &xk) {
    std::vector<double> new_xk(xk.size());
    for (size_t i = 0; i < xk.size(); i++) {
        new_xk[i] = 0;
        for (size_t j = 0; j < xk.size(); j++) {
            new_xk[i] += c[i][j] * xk[j];
        }
        new_xk[i] += d[i];
    }
    return new_xk;
}
void print_xk(std::vector<double> const &xk) {
    std::cout << "Xk = (";
    for (size_t i = 0; i < xk.size() - 1; i++) {
        std::cout << xk[i] << "; ";
    }
    std::cout << xk[xk.size() - 1] << ")";
    std::cout << std::endl;
}

std::vector<double> calculate_answer(std::vector<std::vector<double>> const &a, std::vector<double> const &b, double const eps, bool const print_info = true) {
    std::vector<std::vector<double>> c = calculate_c(a);
    std::vector<double> d = calculate_d(a, b);
    std::vector<double> xk = d;
    std::vector<double> new_xk;
    if (print_info) {
        std::cout << "Матрица a: " << std::endl;
        print_matrix(a);
        std::cout << "Вектор b: " << std::endl;
        for (auto const &i : b) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        std::cout << "Матрица c: " << std::endl;
        print_matrix(c);
        std::cout << "Вектор d: " << std::endl;
        for (auto const &i : d) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        print_xk(xk);
    }
    bool flag = true;
    size_t iter = 1;
    while (flag) {
        double max_diff = 0;
        new_xk = calculate_new_xk(c, d, xk);
        flag = false;
        for (size_t i = 0; i < xk.size(); i++) {
            double const diff = std::abs(new_xk[i] - xk[i]);
            if (diff > max_diff) {
                max_diff = diff;
            }
            if (diff >= eps) {
                flag = true;
            }
        }
        xk = new_xk;
        if (print_info) {
            std::cout << "----- Итерация №" << iter << " -----" << std::endl;
            print_xk(xk);
            std::cout << "Максимальная разность: " << max_diff << std::endl;
            iter++;
        }
    }
    return xk;
}

bool read_from_file(std::ifstream &file, std::vector<std::vector<double>> &a, std::vector<double> &b, double &eps) {
    if (file >> eps) {
        if (file.get() != '\n') {
            std::cerr << "Некорректный ввод точности." << std::endl;
            return false;
        } else if (eps <= 0) {
            std::cerr << "Точность должна быть положительным числом." << std::endl;
            return false;
        }
    } else {
        std::cerr << "Некорректный ввод точности." << std::endl;
        return false;
    }

    int64_t n;
    if (file >> n) {
        if (file.get() != '\n') {
            std::cerr << "Некорректный ввод размерности." << std::endl;
            return false;
        }
        if (n <= 0) {
            std::cerr << "Размерность матрицы должна быть положительным числом." << std::endl;
            return false;
        }
    } else {
        std::cerr << "Некорректный ввод размерности." << std::endl;
        return false;
    }
    a.resize(n, std::vector<double>(n));
    b.resize(n);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (!(file >> a[i][j])) {
                std::cerr << "Некорректная матрица." << std::endl;
                return false;
            }
        }
        if (!(file >> b[i])) {
            std::cerr << "Некорректный коэффициент b." << std::endl;
            return false;
        };
    }
    return true;
}
void read_from_terminal(std::vector<std::vector<double>> &a, std::vector<double> &b, double &eps) {
    auto clear_and_ignore_and_print_error = [] {
        std::cin.clear();
        std::cin.ignore(UINT32_MAX, '\n');
        std::cerr << "Некорректный ввод" << std::endl;
        if (std::cin.eof()) {
            exit(0);
        }
    };

    while (true) {
        std::cout << "Введите точность вычислений: ";
        if (std::cin >> eps) {
            if (getchar() != '\n') {
                clear_and_ignore_and_print_error();
            } else if (eps > 0) {
                break;
            } else {
                std::cerr << "Точность должна быть положительным числом." << std::endl;
            }
        }
    }

    int64_t n;

    while (true) {
        std::cout << "Введите размерность матрицы: ";
        if (std::cin >> n) {
            if (getchar() != '\n') {
                clear_and_ignore_and_print_error();
            } else if (n > 0) {
                break;
            } else {
                std::cerr << "Размерность матрицы должна быть положительным числом." << std::endl;
            }
        } else {
            clear_and_ignore_and_print_error();
        }
    }
    a.resize(n, std::vector<double>(n));
    b.resize(n);
    bool input_error = true;
    while (input_error) {
        input_error = false;
        std::cout << "Введите матрицу в формате" << std::endl
                  << "a11 a12 ... a1n b1" << std::endl
                  << "a21 a22 ... a2n b2" << std::endl
                  << "..." << std::endl
                  << "an1 an2 ... ann bn" << std::endl
                  << "(вводить можно и в одну строку, но в заданном порядке)" << std::endl;

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                if (!(std::cin >> a[i][j])) {
                    input_error = true;
                    break;
                }
            }
            if (input_error || !(std::cin >> b[i])) {
                input_error = true;
                break;
            }
        }
        if (input_error) {
            clear_and_ignore_and_print_error();
        }
    }
}

void print_entered(std::vector<std::vector<double>> const &a, std::vector<double> const &b, double const eps) {
    std::cout << "Погрешность: " << eps << std::endl;
    std::cout << "Введено СЛАУ:" << std::endl;
    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < a[i].size(); j++) {
            std::cout << a[i][j] << "·x" << j + 1;
            if (j != a.size() - 1 && a[i][j + 1] > 0) {
                std::cout << " + ";
            }
        }
        std::cout << " = " << b[i] << std::endl;
    }
}

void print_answer(std::vector<double> const &xk) {
    std::cout << "Ответ:" << std::endl;
    for (size_t i = 0; i < xk.size(); i++) {
        std::cout << "x" << i + 1 << " = " << xk[i] << std::endl;
    }
}

void solve_sole(int const argc, char const *argv[]) {
    std::vector<std::vector<double>> a;
    std::vector<double> b;
    double eps;

    switch (argc) {
        case 1:
            std::cout << "Файл не был указан. Чтение из терминала." << std::endl;
            read_from_terminal(a, b, eps);
            print_entered(a, b, eps);
            break;
        case 2: {
            std::cout << "Чтение из файла " << argv[1] << std::endl;
            std::ifstream file(argv[1]);
            if (!read_from_file(file, a, b, eps))
                return;
            file.close();
            print_entered(a, b, eps);
            break;
        }
        default:
            std::cout << "Неверное количество аргументов." << std::endl;
            return;
    }
    if (!check_diagonal_dominance(a, b)) {
        std::cout << "Матрица не удовлетворяет условию диагонального преобладания" << std::endl;
        return;
    }
    print_answer(calculate_answer(a, b, eps));
}

int main(int const argc, char const *argv[]) {
    solve_sole(argc, argv);
}