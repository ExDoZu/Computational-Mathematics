import matplotlib.pyplot as plt
from typing import Callable


def create_graph(x: list[float], y: list[float], color: str) -> None:
    plt.plot(x, y, color=color)
    plt.axhline(0, color='black')
    plt.axvline(0, color='black')
    plt.ylabel('y')
    plt.xlabel('x')
    plt.grid(True)
    plt.show()


def create_graph2(x1: list[float], y1: list[float], x2: list[float], y2: list[float], color1: str, color2: str) -> None:
    plt.scatter(x1, y1, color=color1, s=1)
    plt.scatter(x2, y2, color=color2, s=1)
    plt.axhline(0, color='black')
    plt.axvline(0, color='black')
    plt.ylabel('y')
    plt.xlabel('x')
    plt.grid(True)
    plt.show()


def calculate_graph(function: Callable[[float], float],
                    a: float, b: float, step: float = 0.01) -> tuple[list[float], list[float]]:
    x = list()
    y = list()
    while a <= b:
        x.append(a)
        y.append(function(a))
        a += step
    return x, y


def calculate_graph2(function: Callable[[float, float], float],
                     a: float, b: float, step: float = 0.01) -> tuple[list[float], list[float]]:
    x = list()
    y = list()
    xc = a
    while xc <= b:
        yc = a
        while yc <= b:
            lb = function(xc, yc)
            lt = function(xc, yc + step)
            rb = function(xc + step, yc)
            rt = function(xc + step, yc + step)
            if (lb < 0 or rb < 0 or rt < 0 or lt < 0) and (lb > 0 or rb > 0 or rt > 0 or lt > 0):
                x.append(xc + step / 2)
                y.append(yc + step / 2)
            yc += step
        xc += step
    return x, y


def calculate_precision(eps: float, num_of_additional_digits: int = 1) -> int:
    if eps <= 0 or num_of_additional_digits < 0:
        raise ValueError("(ᇂ_Jᇂ )")
    precision: int = 0
    while eps < 1:
        eps *= 10
        precision += 1
    return precision + num_of_additional_digits


def derivative(function: Callable[[float], float], x: float, dx: float = 0.0000001) -> float:
    # who copies is the blonde
    return (function(x + dx) - function(x)) / dx


def derivative_by_x1(function: Callable[[float, float], float], x1: float, x2: float, dx: float = 0.0000001) -> float:
    return (function(x1 + dx, x2) - function(x1, x2)) / dx


def derivative_by_x2(function: Callable[[float, float], float], x1: float, x2: float, dx: float = 0.0000001) -> float:
    return (function(x1, x2 + dx) - function(x1, x2)) / dx
