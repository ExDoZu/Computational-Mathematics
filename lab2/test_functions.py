from typing import Callable
import math


def testfunc(x):
    return x ** 3 - x + 4


def system1() -> list[Callable[[float, float], float]]:
    def f1(x1: float, x2: float) -> float:
        return x1 ** 2 + x2 ** 2 - 4

    def f2(x1: float, x2: float) -> float:
        return -3 * x1 ** 2 + x2

    return [f1, f2]


def system2() -> list[Callable[[float, float], float]]:
    def f1(x1: float, x2: float) -> float:
        return math.sin(x1 * x2)

    def f2(x1: float, x2: float) -> float:
        return math.tan(x1) + math.cos(x2)

    return [f1, f2]
