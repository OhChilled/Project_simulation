#include <math.h>
#include <stdio.h>
#include <windows.h>

double omega = 0.001, ax, ay;
double fx(double y0);
double fy(double x0, double y1);
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    // Оголошення та ініціалізація змінних
    double x0, x00 = 0.01, x1, x10 = 10, y0, y00 = 0.01, y1, y10 = 10;
    double k1, k2, k3, k4, g1, g2, g3, g4;
    double r1, r2, r3, r4, w1, w2, w3, w4;
    double t, t0 = 0, h = 0.01, tk = 7; FILE* p;
// Відкриття файлу для запису результатів
fopen_s(&p, "cosm2425.dan", "w");
if (p == NULL) {
    printf("Помилка відкриття файлу!\n");
    return 1;
}

// Введення параметрів керування
puts("Введіть ax, ay:");
scanf_s("%lf%lf", &ax, &ay);

// Ініціалізація початкових умов
t = t0;
x0 = x00;  // початкова швидкість по x
x1 = x10;  // початкова координата x
y0 = y00;  // початкова швидкість по y
y1 = y10;  // початкова координата y

// Вивід заголовка у файл
fprintf(p, "Параметри моделювання:\n");
fprintf(p, "omega = %lf, ax = %lf, ay = %lf\n", omega, ax, ay);
fprintf(p, "Початкові умови: x1=%lf, x0=%lf, y1=%lf, y0=%lf\n", x10, x00, y10, y00);
fprintf(p, "Крок h=%lf, Кінцевий час tk=%lf\n\n", h, tk);
fprintf(p, "t\tx1\tx0\ty1\ty0\tR\n");

// Основний цикл інтегрування
while (t <= tk)
{
    // Обчислення коефіцієнтів Рунге-Кутта
    // Перший крок
    k1 = fx(y0);
    g1 = x0;
    r1 = fy(x0, y1);
    w1 = y0;

    // Другий крок
    k2 = fx(y0 + h * r1 / 2);
    g2 = x0 + h * k1 / 2;
    r2 = fy(x0 + h * k1 / 2, y1 + h * w1 / 2);
    w2 = y0 + h * r1 / 2;

    // Третій крок
    k3 = fx(y0 + h * r2 / 2);
    g3 = x0 + h * k2 / 2;
    r3 = fy(x0 + h * k2 / 2, y1 + h * w2 / 2);
    w3 = y0 + h * r2 / 2;

    // Четвертий крок
    k4 = fx(y0 + h * r3);
    g4 = x0 + h * k3;
    r4 = fy(x0 + h * k3, y1 + h * w3);
    w4 = y0 + h * r3;

    // Оновлення значень змінних
    y0 += h * (r1 + 2 * r2 + 2 * r3 + r4) / 6;
    y1 += h * (w1 + 2 * w2 + 2 * w3 + w4) / 6;
    x0 += h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    x1 += h * (g1 + 2 * g2 + 2 * g3 + g4) / 6;
    t += h;

    // Обчислення поточної відстані
    double R = sqrt(x1*x1 + y1*y1);

    // Запис результатів у файл
    fprintf(p, "%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\n",
            t, x1, x0, y1, y0, R);

    // Перевірка умови стикування
    if (R <= 1.0) {  // R_крит = 1.0 м
        printf("Стикування відбулося в момент t = %.3lf с\n", t);
        fprintf(p, "\nСтикування відбулося в момент t = %.3lf с\n", t);
        break;
    }
}

// Перевірка, чи відбулося стикування
double final_R = sqrt(x1*x1 + y1*y1);
if (final_R > 1.0) {
    printf("Стикування не відбулося. Кінцева відстань: %.3lf м\n", final_R);
    fprintf(p, "\nСтикування не відбулося. Кінцева відстань: %.3lf м\n", final_R);
}

fclose(p);
printf("Результати збережено у файлі cosm2425.dan\n");
return 0;
}
// Функція для обчислення правої частини рівняння для x
double fx(double y0) { return 2 * omega * y0 + ax; }
// Функція для обчислення правої частини рівняння для y
double fy(double x0, double y1) { return -2 * omega * x0 + 3 * pow(omega, 2) * y1 + ay; }
