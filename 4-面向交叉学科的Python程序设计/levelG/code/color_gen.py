def rgb(red, green, blue):
    return f'\x1b[38;2;{red};{green};{blue}m'

# 直接根据score获得应该显示的颜色RGB三元组
# score: -1 ~ 1
def get_color(score: float):
    if score > 0:
        return (245-253)*score+253, (69-218)*score+218, (69-218)*score+218
    else:
        score = -score
        return (19-208)*score+208, (152-234)*score+234, (20-208)*score+208

# tag: 为False，返回RGB元组；为True，返回控制台可用的颜色改变字符
def get_rgb(score: float, tag: bool):
    if tag:
        r, g, b = get_color(score)
        return rgb(int(r), int(g), int(b))
    return get_color(score)