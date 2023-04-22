import numpy
import math
import matplotlib.pyplot as plt
import random
from random import randint as ri

g = 9.8
m = 2.0


# 位置函数
def dist(x0, y0, z0, x1, y1, z1):
    return math.sqrt((x0 - x1)**2 + (y0 - y1)**2 + (z0 - z1)**2)
# 生成随机空气阻力
def generate_air_force():
    return [float(ri(-5, 20)), float(ri(-5, 20)), float(ri(-5, 20))]
# 生成随机初始位置
def generate_init_pos():
    return [float(ri(0, 6)), float(ri(100, 200)), float(ri(0, 6))]
# 生成随机末了位置
def generate_fini_pos():
    return [float(ri(100, 200)), float(ri(0, 100)), float(ri(100, 200))]
# 生成随机初始速度
def generate_v0():
    return float(ri(25, 100))

# 情况1的时间函数
def t_cond1(vy, ay1, ay2, y0, y1):
    return (vy - math.sqrt(vy**2 - 2 * ay1 * (y1 - y0))) / ay1
# 情况2~4的时间函数
def t1plust2(vy, ay1, ay2, y0, y1):
    return vy/ay1 + math.sqrt(2 * (math.pow(vy, 2)/(2*ay1) + y0 - y1) / ay2)
# 情况5的时间函数
def t_cond5(vy, ay1, ay2, y0, y1):
    return (vy - math.sqrt(vy**2 - 2 * ay2 * (y1 - y0))) / ay2

# x,z平面的约束方程，使用的时间函数作为参数
def constraint(t_delta, v0, vx, vy, vz, ax, ay1, ay2, az, x0, x1, y0, y1, z0, z1):
    return math.pow((vx * t_delta(vy, ay1, ay2, y0, y1) -
                     ax * t_delta(vy, ay1, ay2, y0, y1) ** 2 / 2 - (x1 - x0)), 2) + \
           math.pow((vz * t_delta(vy, ay1, ay2, y0, y1) -
                     az * t_delta(vy, ay1, ay2, y0, y1) ** 2 / 2 - (z1 - z0)), 2) + \
           math.pow((math.sqrt(vx ** 2 + vy ** 2 + vz ** 2) - v0), 2)

def main():
    # 生成初始信息
    v0 = generate_v0()
    x0, y0, z0 = generate_init_pos()
    x1, y1, z1 = generate_fini_pos()
    fx, fy, fz = generate_air_force()

    # 待用的时间函数
    t_func = [t_cond5, t_cond1, t1plust2]

    # 打印初始值信息
    print(f"from {x0, y0, z0} to {x1, y1, z1}, initial speed is {v0}, air force is {fx, fy, fz}")

    # 加速度信息
    ax, ay1, ay2, az = fx / m, g + fy / m, g - fy / m, fz / m

    # 解集合
    ans = []

    # 尝试三种解法
    for t in t_func:
        # 迭代限度
        recursion_max = 30
        # 迭代步长
        step = v0 / 2
        # 当前差异
        e = 100000.0
        epsilon = 10
        # 初始迭代量
        paras = [math.sqrt(v0/3), math.sqrt(v0/3), math.sqrt(v0/3)]
        # 区间分割器
        n = 50
        x = numpy.linspace(0, v0, n)
        y = numpy.linspace(0, v0, n)
        z = numpy.linspace(0, v0, n)

        # 对此t函数进行迭代尝试
        for _ in range(recursion_max):
            if e > epsilon:
                for i in x:
                    for j in y:
                        for k in z:
                            try:
                                test = constraint(t, v0, i, j, k, ax, ay1, ay2, az, x0, x1, y0, y1, z0, z1)
                            except ValueError:
                                test = e

                            if abs(test) < e:
                                e = test
                                paras = [i, j, k]
                # 下一次迭代
                x = numpy.linspace(paras[0] - step, paras[0] + step, n)
                y = numpy.linspace(paras[1] - step, paras[1] + step, n)
                z = numpy.linspace(paras[2] - step, paras[2] + step, n)
                # 步长改变
                step /= 1.1
                print(f"For condition {t_func.index(t)} Iteration[{_}], e = {e}")
            else:
                # 差别小于epsilon，迭代强制结束
                continue

        # 迭代结束
        if e > epsilon:
            print(f"Using Case {t_func.index(t)}, but cannot find a solution!")
        else:
            # 在答案数组中加入
            ans.append(paras)

    _ax = plt.subplot(projection='3d')  # 创建一个三维的绘图工程
    _ax.set_title('3d_image_show')  # 设置本图名称
    _x0, _y0, _z0 = numpy.array([x0]), numpy.array([y0]), numpy.array([z0])
    _x1, _y1, _z1 = numpy.array([x1]), numpy.array([y1]), numpy.array([z1])
    _ax.set_xlabel('X')  # 设置x坐标轴
    _ax.set_ylabel('Y')  # 设置y坐标轴
    _ax.set_zlabel('Z')  # 设置z坐标轴
    plt.quiver(0, 0, 0, -fx * 10, 0, 0)
    plt.quiver(0, 0, 0, 0, -fy * 10, 0)
    plt.quiver(0, 0, 0, 0, 0, -fz * 10)

    for v_ans in ans:
        xx, xy, xz = [x0], [y0], [z0]
        vx, vy, vz = v_ans
        # 时间间隔
        t_delta = 0.5
        for _ in range(20):
            new_x = xx[len(xx) - 1] + vx * t_delta - ax / 2 * (t_delta ** 2)
            new_z = xz[len(xz) - 1] + vz * t_delta - az / 2 * (t_delta ** 2)
            if vy > 0:
                new_y = xy[len(xy) - 1] + vy * t_delta - ay1 / 2 * (t_delta ** 2)
            else:
                new_y = xy[len(xy) - 1] + vy * t_delta - ay2 / 2 * (t_delta ** 2)

            if dist(new_x, new_y, new_z, x1, y1, z1) < 10:
                break

            xx.append(new_x)
            xy.append(new_y)
            xz.append(new_z)
            if vy > 0:
                vy -= ay1 * t_delta
            else:
                vy -= ay2 * t_delta

            vx -= ax * t_delta
            vz -= az * t_delta

        x = numpy.array(xx)
        y = numpy.array(xy)
        z = numpy.array(xz)

        _ax.scatter(x, z, y, c='r')  # 绘制数据点 c: 'r'红色，'y'黄色，等颜色
        _ax.scatter(_x0, _z0, _y0, c='y')  # 绘制数据点 c: 'r'红色，'y'黄色，等颜色
        _ax.scatter(_x1, _z1, _y1, c='b')  # 绘制数据点 c: 'r'红色，'y'黄色，等颜色

        print(f"A solution found: vec: v0 = {vx, vy, vz}")

    plt.show()


if __name__ == '__main__':
    main()

