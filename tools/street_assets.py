#!/usr/bin/env python3
# ========================================
# FileName: street_assets.py
# Date: 19.November.2026
# Author: Marcos Chow Castro
# Email: mctechnology170318@gmail.com
# GitHub: https://github.com/mctechnology17
# Brief: draws the "street" character, its sleep art and the car chase of zmk-nice-oled and writes them as LVGL 1 bit images.
# =========================================
"""
Everything is drawn upright, the way the user sees the display, and then
rotated 90 degrees clockwise, which is how the nice!view and the OLED are
mounted (the same rotation the Luna sprites use).

    python3 tools/street_assets.py            # writes the C files in assets/
    python3 tools/street_assets.py --preview  # also writes PNG sheets in /tmp

Needs Pillow (pip install pillow). Only for regenerating the art: the
firmware build uses the generated C files.
"""
import argparse
import math
import os
import sys

from PIL import Image, ImageDraw, ImageFilter

HERE = os.path.dirname(os.path.abspath(__file__))
ASSETS = os.path.join(HERE, "..", "boards", "shields", "nice_oled", "assets")

INK = 255
PAPER = 0

# --------------------------------------------------------------------------
# Character: a skeleton per pose, drawn as a solid silhouette
# --------------------------------------------------------------------------
# The skeleton is defined in a 64 x 80 box with the feet on y = 77, looking right.
# The big sprite scales it to 68 x 90, the free area of a nice!view between the
# status lines on top and the profile stars below.
GROUND = 77
BIG_W, BIG_H, BIG_SCALE = 68, 90, 1.125

# Joints of the neutral standing pose. Every pose starts from these.
STAND = {
    "head": (31, 13),
    "neck": (31, 21),
    "sh_b": (27, 28),  # back shoulder (far from the viewer)
    "sh_f": (35, 28),  # front shoulder; 7 pixels under the chin, so the neck shows
    "el_b": (24, 36),
    "el_f": (38, 36),
    "ha_b": (24, 46),
    "ha_f": (39, 46),
    "hip": (31, 46),
    "kn_b": (28, 61),
    "kn_f": (34, 61),
    "ft_b": (27, 75),
    "ft_f": (35, 75),
}


def pose(dy=0, dx=0, **joints):
    """Neutral pose with some joints replaced; dx/dy move the whole body."""
    p = dict(STAND)
    p.update(joints)
    return {k: (x + dx, y + dy) for k, (x, y) in p.items()}


def lean(p, top, keys=("head", "neck", "sh_b", "sh_f")):
    """Moves the upper body sideways (forward lean when running)."""
    q = dict(p)
    for k in keys:
        q[k] = (p[k][0] + top, p[k][1])
    return q


POSES = {}

# Idle is defined further down.

# Walk: legs and arms swing a little.
POSES["walk1"] = dict(
    p=pose(kn_f=(38, 60), ft_f=(42, 74), kn_b=(27, 62), ft_b=(22, 74), el_f=(34, 36), ha_f=(31, 45),
           el_b=(27, 36), ha_b=(32, 44))
)
POSES["walk2"] = dict(
    p=pose(kn_f=(30, 61), ft_f=(25, 74), kn_b=(35, 60), ft_b=(40, 74), el_f=(39, 35), ha_f=(44, 42),
           el_b=(23, 36), ha_b=(19, 44))
)

# Run: forward lean, bent arms, one knee up and the other leg stretched back.
POSES["run1"] = dict(
    p=lean(pose(dy=-1, el_f=(33, 35), ha_f=(27, 30), el_b=(34, 34), ha_b=(43, 29),
                kn_f=(42, 55), ft_f=(47, 68), kn_b=(25, 60), ft_b=(15, 66)), 5),
    dust=True,
)
POSES["run2"] = dict(
    p=lean(pose(dy=-1, el_f=(44, 33), ha_f=(50, 26), el_b=(27, 36), ha_b=(21, 30),
                kn_f=(27, 59), ft_f=(18, 62), kn_b=(40, 56), ft_b=(40, 72)), 5),
)

# Sprint: the run with speed lines behind.
POSES["sprint1"] = dict(p=POSES["run1"]["p"], speed=True, dust=True)
POSES["sprint2"] = dict(p=POSES["run2"]["p"], speed=True)

# Jump: crouch, then in the air with the knees up and the arms raised.
POSES["jump1"] = dict(
    p=pose(dy=9, head=(33, 13), neck=(32, 21), el_f=(41, 33), ha_f=(46, 40), el_b=(22, 33),
           ha_b=(18, 40), kn_f=(41, 53), ft_f=(36, 66), kn_b=(24, 54), ft_b=(26, 66))
)
POSES["jump2"] = dict(
    p=pose(dy=-2, el_f=(43, 22), ha_f=(47, 15), el_b=(20, 22), ha_b=(16, 15), kn_f=(40, 55),
           ft_f=(35, 64), kn_b=(24, 56), ft_b=(22, 66)),
    shadow=True,
)

# Punch: wind up, then the front arm stretched with an impact mark.
POSES["punch1"] = dict(
    p=lean(pose(el_f=(30, 34), ha_f=(36, 28), el_b=(25, 33), ha_b=(31, 30), kn_f=(37, 61),
                ft_f=(40, 75), kn_b=(26, 61), ft_b=(22, 75)), -2)
)
POSES["punch2"] = dict(
    p=lean(pose(el_f=(44, 26), ha_f=(53, 26), el_b=(27, 34), ha_b=(33, 31), kn_f=(39, 60),
                ft_f=(43, 75), kn_b=(25, 62), ft_b=(19, 75)), 3),
    hit=(58, 26),
)

# Kick: the front leg goes up and forward.
POSES["kick1"] = dict(
    p=lean(pose(el_f=(40, 33), ha_f=(44, 27), el_b=(24, 34), ha_b=(27, 28), kn_f=(41, 52),
                ft_f=(38, 64), kn_b=(29, 61), ft_b=(27, 75)), -2)
)
POSES["kick2"] = dict(
    p=lean(pose(el_f=(38, 34), ha_f=(33, 40), el_b=(22, 31), ha_b=(16, 27), kn_f=(43, 49),
                ft_f=(51, 46), kn_b=(29, 61), ft_b=(27, 75)), -5),
    hit=(60, 46),
)

# Uppercut: crouch, then the front fist goes up.
POSES["upper1"] = dict(
    p=pose(dy=5, el_f=(39, 37), ha_f=(44, 43), el_b=(25, 33), ha_b=(30, 30), kn_f=(38, 58),
           ft_f=(39, 70), kn_b=(25, 58), ft_b=(23, 70))
)
POSES["upper2"] = dict(
    p=lean(pose(dy=-3, el_f=(43, 20), ha_f=(45, 8), el_b=(25, 34), ha_b=(29, 30), kn_f=(36, 62),
                ft_f=(38, 77), kn_b=(27, 63), ft_b=(24, 77)), 2),
    hit=(50, 7),
)

# Bat: held over the shoulder, then swung forward.
POSES["bat1"] = dict(
    p=pose(el_f=(41, 29), ha_f=(37, 22), el_b=(31, 30), ha_b=(36, 23), kn_f=(36, 61),
           ft_f=(38, 75), kn_b=(27, 61), ft_b=(24, 75)),
    bat=((37, 22), (17, 6)),
)
POSES["bat2"] = dict(
    p=lean(pose(el_f=(43, 30), ha_f=(50, 33), el_b=(37, 32), ha_b=(47, 34), kn_f=(38, 60),
                ft_f=(42, 75), kn_b=(25, 62), ft_b=(20, 75)), 3),
    bat=((49, 33), (60, 43)),
    swing=True,
)

def tween(a, b, dy=0):
    """Pose half way between two key poses: the arm half stretched, the take off of a jump."""
    out = {"p": {k: ((a["p"][k][0] + b["p"][k][0]) / 2.0, (a["p"][k][1] + b["p"][k][1]) / 2.0 + dy)
                 for k in a["p"]}}
    if "bat" in a and "bat" in b:
        # The bat turns around the hands: half way of the swing it points up and forward.
        hx = (a["bat"][0][0] + b["bat"][0][0]) / 2.0
        hy = (a["bat"][0][1] + b["bat"][0][1]) / 2.0 + dy
        out["bat"] = ((hx, hy), (hx + 12, hy - 24))
    return out


def two_link(root, target, l1, l2, bend=1):
    """Middle joint of a two bone limb (knee or elbow) that reaches from root to target."""
    dx, dy = target[0] - root[0], target[1] - root[1]
    dist = max(abs(l1 - l2) + 0.01, min(math.hypot(dx, dy), l1 + l2 - 0.01))
    base = math.atan2(dy, dx)
    inner = math.acos((l1 * l1 + dist * dist - l2 * l2) / (2 * l1 * dist))
    ang = base - bend * inner
    return (root[0] + l1 * math.cos(ang), root[1] + l1 * math.sin(ang))


def gait(phase, stride, lift, bob, lean_x, swing, elbow, crouch=1.5, kick=0.0):
    """One frame of a walk or run cycle, in place.

    The feet follow a path (on the ground while the leg pushes, an arc while it
    swings forward) and the knees are solved from it, so the legs move like a
    real step. phase runs from 0 to 1; the two legs are half a cycle apart and
    each arm swings against the leg of its side.
    """
    hip = (31.0, 46.0 + crouch + bob * math.cos(4 * math.pi * phase))
    floor = 75.0

    def foot(ph):
        ph %= 1.0
        if ph < 0.5:                                   # on the ground, pushing back
            u = ph / 0.5
            return (hip[0] + stride / 2 - stride * u, floor)
        u = (ph - 0.5) / 0.5                           # in the air, coming forward
        x = hip[0] - stride / 2 + stride * u
        # The heel rises towards the back first (kick), then the knee comes up.
        y = floor - lift * math.sin(math.pi * u) - kick * math.sin(math.pi * min(1.0, u * 1.6)) * (1 - u)
        return (x - kick * 0.5 * math.sin(math.pi * u) * (1 - u), y)

    def arm(shoulder, against):
        # The arm hangs from the middle of the torso and swings around a line a little
        # behind the vertical. Going forward the elbow closes (the fist comes up to the
        # chest); going back it opens, so the hand passes behind the hip.
        forward = -math.cos(2 * math.pi * against)          # -1 fully back, +1 fully forward
        th = math.radians(swing * forward - swing * 0.2)
        el = (shoulder[0] + 11 * math.sin(th), shoulder[1] + 11 * math.cos(th))
        fo = th + math.radians(elbow * (0.35 + 0.65 * (forward + 1) / 2))
        return el, (el[0] + 10 * math.sin(fo), el[1] + 10 * math.cos(fo))

    ft_f, ft_b = foot(phase), foot(phase + 0.5)
    # Seen from the side both shoulders are almost on the same spot, in the middle of the torso.
    sh_f = (hip[0] + 1 + lean_x, hip[1] - 18)
    sh_b = (hip[0] - 1 + lean_x, hip[1] - 18)
    el_f, ha_f = arm(sh_f, phase)
    el_b, ha_b = arm(sh_b, phase + 0.5)
    return {
        # The head leads the body: the more the runner leans, the further forward it goes.
        "head": (hip[0] + 1 + lean_x * 1.9, hip[1] - 33), "neck": (hip[0] + lean_x * 1.35, hip[1] - 24),
        "sh_b": sh_b, "sh_f": sh_f, "el_b": el_b, "el_f": el_f, "ha_b": ha_b, "ha_f": ha_f,
        "hip": hip, "kn_b": two_link(hip, ft_b, 15.5, 14.5), "kn_f": two_link(hip, ft_f, 15.5, 14.5),
        "ft_b": ft_b, "ft_f": ft_f,
    }


FRAME_SPECS = {}
FRAME_COUNT = {}


def add_cycle(name, frames, **params):
    extras = {k: params.pop(k) for k in ("speed", "dust") if k in params}
    for i in range(frames):
        spec = {"p": gait(i / frames, **params)}
        if extras.get("speed"):
            spec["speed"] = True
        if extras.get("dust") and i in (0, frames // 2):
            spec["dust"] = True
        FRAME_SPECS["%s_%d" % (name, i)] = spec
    FRAME_COUNT[name] = frames


def add_frames(name, specs):
    """Animation made of its own poses, one per frame."""
    for i, spec in enumerate(specs):
        FRAME_SPECS["%s_%d" % (name, i)] = spec
    FRAME_COUNT[name] = len(specs)


def add_keys(name, k1, k2, lift=0):
    """Loop of four frames: key pose, in between, key pose, in between."""
    mid = tween(POSES[k1], POSES[k2], lift)
    for i, spec in enumerate((POSES[k1], mid, POSES[k2], mid)):
        FRAME_SPECS["%s_%d" % (name, i)] = spec
    FRAME_COUNT[name] = 4


# Idle: seen from the side the arm hangs from the middle of the torso, straight down
# with the hand a little forward. Breathing moves the upper body one pixel.
POSES["idle1"] = dict(p=pose(sh_f=(30, 28), sh_b=(28, 28), el_f=(28, 38), ha_f=(30, 48),
                             el_b=(27, 38), ha_b=(28, 48)))
POSES["idle2"] = dict(p=pose(head=(31, 14), neck=(31, 22), sh_f=(30, 29), sh_b=(28, 29),
                             el_f=(28, 39), ha_f=(30, 49), el_b=(27, 39), ha_b=(28, 49)))

add_keys("idle", "idle1", "idle2")
add_cycle("walk", 8, stride=16, lift=3, bob=1.0, lean_x=1, swing=26, elbow=22, crouch=1.0)
add_cycle("run", 8, stride=23, lift=5, bob=1.5, lean_x=5, swing=42, elbow=95, crouch=1.0, kick=6, dust=True)
add_cycle("sprint", 8, stride=26, lift=6, bob=2.0, lean_x=7, swing=52, elbow=105, crouch=1.0, kick=8,
          speed=True, dust=True)
# Jump: four poses of its own. Crouch with the arms back, push off with the arms
# coming up, the top of the jump with the knees tucked and the arms high, and the fall.
_side = dict(sh_f=(32, 28), sh_b=(30, 28))
add_frames("jump", [
    dict(p=pose(dy=9, head=(34, 13), neck=(33, 21), sh_f=(33, 28), sh_b=(31, 28), el_f=(27, 36),
                ha_f=(22, 43), el_b=(26, 35), ha_b=(20, 41), kn_f=(41, 53), ft_f=(35, 66),
                kn_b=(38, 54), ft_b=(31, 66))),
    dict(p=pose(dy=-1, el_f=(40, 25), ha_f=(46, 17), el_b=(37, 26), ha_b=(43, 18), kn_f=(33, 61),
                ft_f=(33, 76), kn_b=(30, 61), ft_b=(29, 76), **_side)),
    dict(p=pose(dy=-1, el_f=(39, 21), ha_f=(42, 11), el_b=(36, 22), ha_b=(38, 12), kn_f=(40, 52),
                ft_f=(34, 61), kn_b=(37, 54), ft_b=(30, 62), **_side), shadow=True),
    dict(p=pose(dy=0, el_f=(40, 31), ha_f=(46, 35), el_b=(37, 32), ha_b=(43, 37), kn_f=(36, 58),
                ft_f=(33, 70), kn_b=(33, 59), ft_b=(29, 71), **_side), shadow=True),
])
# Fights: four poses each, seen from the side. They start from a boxing guard:
# feet apart, knees a little bent and both fists up in front of the chin.
def fight(top=0, dy=0, fist=True, **joints):
    base = dict(sh_f=(33, 28), sh_b=(31, 28), head=(32, 13), neck=(32, 21),
                # The back arm sits almost behind the front one: seen from the side only
                # one guard arm shows, and the two do not merge into a blob.
                el_f=(38, 37), ha_f=(42, 28), el_b=(37, 38), ha_b=(41, 30),
                kn_f=(38, 61), ft_f=(41, 75), kn_b=(25, 61), ft_b=(21, 75))
    base.update(joints)
    spec = {"p": lean(pose(dy=dy, **base), top)}
    if fist:
        spec["fist"] = True
    return spec


def with_hit(spec, x, y):
    spec["hit"] = (x, y)
    return spec


add_frames("punch", [
    fight(),                                                              # guard
    fight(top=-3, el_f=(23, 33), ha_f=(30, 27), kn_b=(24, 60), ft_b=(20, 75)),   # the arm goes back
    with_hit(fight(top=6, dy=1, head=(33, 14), el_f=(45, 28), ha_f=(55, 28),    # straight punch
                   el_b=(35, 37), ha_b=(40, 31),
                   kn_f=(43, 59), ft_f=(45, 74), kn_b=(25, 60), ft_b=(16, 73)), 60, 28),
    fight(top=2, el_f=(42, 33), ha_f=(49, 28)),                           # coming back
])
add_frames("upper", [
    fight(dy=3, kn_f=(40, 58), ft_f=(41, 72), kn_b=(26, 58), ft_b=(21, 72)),          # low guard
    fight(dy=7, top=1, el_f=(35, 40), ha_f=(41, 46), kn_f=(42, 55), ft_f=(41, 68),    # dip, fist low
          kn_b=(27, 56), ft_b=(21, 68)),
    with_hit(fight(dy=-1, top=3, el_f=(41, 27), ha_f=(42, 15), kn_f=(37, 62),         # fist up
                   ft_f=(41, 76), kn_b=(26, 62), ft_b=(21, 76)), 45, 8),
    fight(dy=1, top=2, el_f=(41, 30), ha_f=(44, 20)),                                 # follow through
])
add_frames("kick", [
    fight(),                                                              # guard
    fight(top=-3, kn_f=(42, 51), ft_f=(39, 63), kn_b=(30, 61), ft_b=(29, 75),         # knee up
          el_b=(27, 37), ha_b=(22, 31)),
    with_hit(fight(top=-6, kn_f=(43, 49), ft_f=(55, 47), kn_b=(30, 61), ft_b=(29, 75),  # leg out
                   el_f=(36, 38), ha_f=(40, 31), el_b=(26, 36), ha_b=(20, 30)), 61, 47),
    fight(top=-3, kn_f=(42, 51), ft_f=(40, 62), kn_b=(30, 61), ft_b=(29, 75),         # leg back
          el_b=(27, 37), ha_b=(22, 31)),
])
add_keys("bat", "bat1", "bat2")
# Order in which the frames are written.
FRAMES = list(FRAME_SPECS)


def thick_line(d, a, b, width):
    d.line([a, b], fill=INK, width=width)
    r = width / 2.0
    for x, y in (a, b):
        d.ellipse([x - r, y - r, x + r, y + r], fill=INK)


def draw_figure(d, spec, s=1.0, ox=0, oy=0, small=False):
    """Draws one pose. s scales the skeleton, small switches to thin limbs."""
    P = {k: (ox + x * s, oy + y * s) for k, (x, y) in spec["p"].items()}
    arm = 2 if small else 5
    leg = 2 if small else 6
    head_r = 3.0 if small else 7.0

    # Legs and feet.
    for kn, ft in (("kn_b", "ft_b"), ("kn_f", "ft_f")):
        thick_line(d, P["hip"], P[kn], leg)
        thick_line(d, P[kn], P[ft], leg)
        fx, fy = P[ft]
        if small:
            d.line([(fx, fy), (fx + 2, fy)], fill=INK)
        else:
            d.rectangle([fx - 2, fy - 1, fx + 6, fy + 2], fill=INK)

    # Torso: shoulders down to the hips.
    (bx, by), (fx, fy), (hx, hy) = P["sh_b"], P["sh_f"], P["hip"]
    if small:
        d.polygon([(bx, by), (fx, fy), (hx + 1, hy), (hx - 1, hy)], fill=INK)
        thick_line(d, ((bx + fx) / 2, by), (hx, hy), 3)
    else:
        d.polygon([(bx - 3, by - 1), (fx + 3, fy - 1), (hx + 6, hy + 1), (hx - 6, hy + 1)], fill=INK)

    # Arms and fists.
    for sh, el, ha in (("sh_b", "el_b", "ha_b"), ("sh_f", "el_f", "ha_f")):
        thick_line(d, P[sh], P[el], arm)
        thick_line(d, P[el], P[ha], arm)
        if not small:
            x, y = P[ha]
            d.ellipse([x - 3.5, y - 3.5, x + 3.5, y + 3.5], fill=INK)

    # Neck, head and cap with its brim to the front.
    thick_line(d, P["neck"], P["head"], 3 if small else 5)
    x, y = P["head"]
    d.ellipse([x - head_r, y - head_r, x + head_r, y + head_r], fill=INK)
    if small:
        d.line([(x + 2, y - 2), (x + 5, y - 2)], fill=INK)
    else:
        d.pieslice([x - head_r - 1, y - head_r - 2, x + head_r + 1, y + head_r], 180, 360, fill=INK)
        d.rectangle([x + 2, y - 2, x + 12, y], fill=INK)
        # A paper coloured band between the cap and the face, and the belt.
        d.line([(x - head_r + 1, y + 1), (x + head_r - 1, y + 1)], fill=PAPER)
        d.line([(hx - 5, hy - 2), (hx + 5, hy - 2)], fill=PAPER)

    # Extras.
    if "bat" in spec:
        (ax, ay), (bx2, by2) = [(ox + x * s, oy + y * s) for x, y in spec["bat"]]
        d.line([(ax, ay), (bx2, by2)], fill=INK, width=2 if small else 3)
        if not small:
            mx, my = (ax + bx2 * 2) / 3, (ay + by2 * 2) / 3
            thick_line(d, (mx, my), (bx2, by2), 5)
    if spec.get("swing") and not small:
        d.arc([ox + 22 * s, oy + 4 * s, ox + 70 * s, oy + 52 * s], 300, 20, fill=INK)
        d.arc([ox + 28 * s, oy + 0 * s, ox + 76 * s, oy + 48 * s], 310, 10, fill=INK)
    if "hit" in spec:
        cx, cy = ox + spec["hit"][0] * s, oy + spec["hit"][1] * s
        n = 2 if small else 4
        for ang in (-50, 0, 50) if not small else (0,):
            a = math.radians(ang)
            d.line([(cx, cy), (cx + n * math.cos(a), cy + n * math.sin(a))], fill=INK)
            d.line([(cx + (n + 2) * math.cos(a), cy + (n + 2) * math.sin(a)),
                    (cx + (n + 4) * math.cos(a), cy + (n + 4) * math.sin(a))], fill=INK)
    if spec.get("speed"):
        for i, y in enumerate((30, 40, 52)):
            x0 = max(ox, 0) + (2 + 3 * (i % 2)) * s
            d.line([(x0, oy + y * s), (x0 + (10 - 2 * i) * s, oy + y * s)], fill=INK)
    if spec.get("dust") and not small:
        gx, gy = max(ox, 0) + 7 * s, oy + (GROUND - 1) * s
        d.arc([gx - 4, gy - 4, gx + 4, gy + 4], 180, 360, fill=INK)
        d.arc([gx + 4, gy - 3, gx + 9, gy + 3], 180, 360, fill=INK)
    if spec.get("shadow"):
        gx, gy = ox + 31 * s, oy + (GROUND + 1) * s
        d.line([(gx - 8 * s, gy), (gx + 8 * s, gy)], fill=INK)


def part(img, draw_fn, fill):
    """One piece of the character with an ink outline.

    fill is "ink" (dark trousers, cap), "paper" (skin, shoes) or "dither" (a
    checkerboard that reads as grey on a 1 bit display: the shirt). A paper
    coloured rim is cut around the piece first, so it stays apart from the
    pieces behind it.
    """
    mask = Image.new("L", img.size, 0)
    draw_fn(ImageDraw.Draw(mask))
    img.paste(PAPER, mask=mask.filter(ImageFilter.MaxFilter(3)))
    img.paste(INK, mask=mask)
    if fill == "ink":
        return
    inner = mask.filter(ImageFilter.MinFilter(3))
    if fill == "paper":
        img.paste(PAPER, mask=inner)
    else:
        pattern = Image.new("L", img.size, PAPER)
        px = pattern.load()
        for y in range(img.size[1]):
            for x in range(y % 2, img.size[0], 2):
                px[x, y] = INK
        img.paste(pattern, mask=inner)


def limb_fn(points, width):
    def fn(d):
        for a, b in zip(points, points[1:]):
            d.line([a, b], fill=255, width=width)
        r = width / 2.0
        for x, y in points:
            d.ellipse([x - r, y - r, x + r, y + r], fill=255)
    return fn


def dither(img, mask):
    """Fills the mask with a checkerboard, the grey of a 1 bit display."""
    pattern = Image.new("L", img.size, PAPER)
    px = pattern.load()
    for y in range(img.size[1]):
        for x in range(y % 2, img.size[0], 2):
            px[x, y] = INK
    img.paste(pattern, mask=mask)


def draw_figure_detailed(img, spec, s=1.0, ox=0.0, oy=0.0, lit=False):
    """The big character, piece by piece from back to front.

    Cap worn backwards, chin beard, tank top, wide trousers and high top
    shoes. The skin and the shoes are always light and the trousers and the
    cap always dark, whatever the display mode:

    lit=False  for a normal display (ink is dark): skin and shoes are paper
               with an ink outline, trousers and cap are ink.
    lit=True   for an inverted display (ink is light): skin and shoes are ink,
               trousers and cap are paper with an ink outline.
    The top is grey (dither) in both.
    """
    SKIN, CLOTH = ("ink", "paper") if lit else ("paper", "ink")
    FEAT = PAPER if lit else INK          # lines drawn on the skin
    SEAM = INK if lit else PAPER          # lines drawn on the cap

    P = {k: (ox + x * s, oy + y * s) for k, (x, y) in spec["p"].items()}
    arm, leg, head_r = 6, 11, 8.5 * s / 1.125
    d = ImageDraw.Draw(img)

    def arm_fn(sh, el, ha):
        # Muscular arm: round shoulder, a biceps that bulges in the middle of the upper
        # arm, a forearm thick at the elbow and thin at the wrist, and the fist.
        def fn(dd):
            (sx, sy), (ex, ey), (wx, wy) = P[sh], P[el], P[ha]
            dd.ellipse([sx - 5, sy - 5, sx + 5, sy + 5], fill=255)
            limb_fn([(sx, sy), (ex, ey)], arm + 1)(dd)
            mx, my = (sx + ex) / 2, (sy + ey) / 2
            dd.ellipse([mx - 4.5, my - 4.5, mx + 5.5, my + 4.5], fill=255)
            fx, fy = ex + (wx - ex) * 0.45, ey + (wy - ey) * 0.45
            limb_fn([(ex, ey), (fx, fy)], arm + 1)(dd)
            limb_fn([(fx, fy), (wx, wy)], arm - 1)(dd)
            r = 4.5 if spec.get("fist") else 3.5            # a closed fist is bigger
            dd.ellipse([wx - r, wy - r, wx + r, wy + r], fill=255)
        return fn

    def leg_piece(kn, ft, side):
        # The leg starts under the hips, not at the belt. Wide trousers that narrow a
        # little towards the ankle.
        hip, knee, foot = P["hip"], P[kn], P[ft]
        root = (hip[0] + side * 1.5, hip[1] + 5)
        # The shoe follows the shin: flat on the ground when standing, toes up in a kick,
        # heel up when the leg swings back.
        dx, dy = foot[0] - knee[0], foot[1] - knee[1]
        n = math.hypot(dx, dy) or 1.0
        sx, sy = dx / n, dy / n                      # along the shin
        tx, ty = sy, -sx                             # towards the toes
        ankle = (foot[0] - sx * 2, foot[1] - sy * 2)

        def trousers(dd):
            # The seat of the trousers belongs to both legs, so hips and legs read as one piece.
            dd.polygon([(hip[0] - 5, hip[1]), (hip[0] + 6, hip[1]), (hip[0] + 6, hip[1] + 6),
                        (hip[0] - 5, hip[1] + 6)], fill=255)
            limb_fn([root, knee], leg)(dd)
            limb_fn([knee, ankle], leg - 2)(dd)
        part(img, trousers, CLOTH)
        heel = (foot[0] - tx * 3, foot[1] - ty * 3)
        toe = (foot[0] + tx * 8, foot[1] + ty * 8)
        part(img, limb_fn([heel, toe], 7), SKIN)
        # Sole on the outer side and the line of the ankle.
        d.line([(heel[0] + sx * 3, heel[1] + sy * 3), (toe[0] + sx * 3, toe[1] + sy * 3)], fill=FEAT)
        d.line([(foot[0] + tx * 2 - sx * 2, foot[1] + ty * 2 - sy * 2),
                (foot[0] + tx * 2, foot[1] + ty * 2)], fill=FEAT)

    def bat_piece():
        (ax, ay), (bx2, by2) = [(ox + x * s, oy + y * s) for x, y in spec["bat"]]
        mx, my = ax + (bx2 - ax) * 0.3, ay + (by2 - ay) * 0.3
        part(img, limb_fn([(ax, ay), (mx, my)], 3), "ink")                    # handle
        part(img, limb_fn([(mx, my), (bx2, by2)], 6), SKIN)                   # barrel

    if "bat" in spec and not spec.get("swing"):
        bat_piece()

    part(img, arm_fn("sh_b", "el_b", "ha_b"), SKIN)
    leg_piece("kn_b", "ft_b", -1)

    # Tank top seen from the side: narrow at the strap, the chest a little forward,
    # the back straight and the waist thinner than the chest.
    (bx, by), (fx, fy), (hx, hy) = P["sh_b"], P["sh_f"], P["hip"]
    cx = P["neck"][0]                       # the torso hangs from the neck, not from the arms
    part(img, lambda dd: dd.polygon([(cx - 4, by - 3), (cx + 4, fy - 3), (cx + 9, fy + 4),
                                     (cx + 8, (fy + hy) / 2 + 2), (hx + 7, hy + 1), (hx - 6, hy + 1),
                                     (cx - 7, (by + hy) / 2), (cx - 8, by + 3)], fill=255), "dither")

    leg_piece("kn_f", "ft_f", 1)
    d.line([(hx - 5, hy), (hx + 6, hy)], fill=INK)                             # belt
    d.rectangle([hx + 1, hy - 1, hx + 2, hy + 1], fill=INK)                    # buckle

    # Neck and head.
    part(img, limb_fn([P["neck"], P["head"]], 6), SKIN)
    x, y = P["head"]
    part(img, lambda dd: dd.ellipse([x - head_r, y - head_r, x + head_r, y + head_r], fill=255), SKIN)

    # Cap worn backwards: dome and the brim pointing to the back.
    def cap(dd):
        dd.pieslice([x - head_r - 1, y - head_r - 2, x + head_r + 1, y + head_r - 2], 180, 360, fill=255)
        dd.polygon([(x - head_r, y - 5), (x - head_r - 8, y - 2), (x - head_r - 8, y + 1),
                    (x - head_r, y - 1)], fill=255)
    part(img, cap, CLOTH)
    d.arc([x + 2, y - head_r + 1, x + head_r - 2, y - 2], 190, 350, fill=SEAM)    # strap opening
    # Brow, eye, mouth, chin beard and ear, looking right.
    d.line([(x + 2, y + 1), (x + 6, y + 1)], fill=FEAT)
    d.rectangle([x + 4, y + 2, x + 5, y + 3], fill=FEAT)
    d.line([(x + 4, y + 6), (x + 6, y + 6)], fill=FEAT)
    d.rectangle([x + 2, y + head_r - 3, x + 5, y + head_r - 2], fill=FEAT)
    d.line([(x - 3, y + 1), (x - 3, y + 3)], fill=FEAT)

    part(img, arm_fn("sh_f", "el_f", "ha_f"), SKIN)

    if "bat" in spec and spec.get("swing"):
        bat_piece()

    # Extras in ink.
    if spec.get("swing"):
        d.arc([ox + 22 * s, oy + 4 * s, ox + 70 * s, oy + 52 * s], 300, 20, fill=INK)
        d.arc([ox + 28 * s, oy + 0 * s, ox + 76 * s, oy + 48 * s], 310, 10, fill=INK)
    if "hit" in spec:
        cx, cy = ox + spec["hit"][0] * s, oy + spec["hit"][1] * s
        for ang in (-50, 0, 50):
            a = math.radians(ang)
            d.line([(cx, cy), (cx + 4 * math.cos(a), cy + 4 * math.sin(a))], fill=INK)
            d.line([(cx + 6 * math.cos(a), cy + 6 * math.sin(a)),
                    (cx + 8 * math.cos(a), cy + 8 * math.sin(a))], fill=INK)
    if spec.get("speed"):
        for i, yy in enumerate((30, 40, 52)):
            x0 = 1 + 3 * (i % 2)
            d.line([(x0, oy + yy * s), (x0 + 9 - 2 * i, oy + yy * s)], fill=INK)
    if spec.get("dust"):
        gx, gy = 7, oy + (GROUND + 1) * s
        d.arc([gx - 4, gy - 4, gx + 4, gy + 4], 180, 360, fill=INK)
        d.arc([gx + 4, gy - 3, gx + 9, gy + 3], 180, 360, fill=INK)
    if spec.get("shadow"):
        gx, gy = ox + 31 * s, BIG_H - 2
        d.line([(gx - 9 * s, gy), (gx + 9 * s, gy)], fill=INK)
        d.line([(gx - 6 * s, gy + 1), (gx + 6 * s, gy + 1)], fill=INK)


def character_big(name, lit=False):
    # The body stands a little left of the centre: punches, kicks and the bat go to the right.
    img = Image.new("L", (BIG_W, BIG_H), PAPER)
    s = BIG_SCALE
    draw_figure_detailed(img, FRAME_SPECS[name], s=s, ox=BIG_W / 2 - 35 * s, oy=BIG_H - 5 - GROUND * s,
                         lit=lit)
    return img


# Small sprite: 32 wide, 24 high, the size of the Luna sprites (fits a 32 pixel wide OLED).
SMALL_W, SMALL_H = 32, 24


def character_small(name):
    img = Image.new("L", (SMALL_W, SMALL_H), PAPER)
    s = 0.29
    draw_figure(ImageDraw.Draw(img), FRAME_SPECS[name], s=s, ox=16 - 31 * s, oy=0.5, small=True)
    return img


# --------------------------------------------------------------------------
# Small 5x7 font for the sleep art
# --------------------------------------------------------------------------
FONT = {
    "W": ["#...#", "#...#", "#...#", "#.#.#", "#.#.#", "##.##", "#...#"],
    "A": [".###.", "#...#", "#...#", "#####", "#...#", "#...#", "#...#"],
    "S": [".####", "#....", "#....", ".###.", "....#", "....#", "####."],
    "T": ["#####", "..#..", "..#..", "..#..", "..#..", "..#..", "..#.."],
    "E": ["#####", "#....", "#....", "####.", "#....", "#....", "#####"],
    "D": ["####.", "#...#", "#...#", "#...#", "#...#", "#...#", "####."],
    "Z": ["#####", "....#", "...#.", "..#..", ".#...", "#....", "#####"],
    "V": ["#...#", "#...#", "#...#", "#...#", ".#.#.", ".#.#.", "..#.."],
    "I": ["#####", "..#..", "..#..", "..#..", "..#..", "..#..", "#####"],
}


def text(d, x, y, s, scale=1, fill=INK):
    for ch in s:
        if ch != " ":
            for r, row in enumerate(FONT[ch]):
                for c, bit in enumerate(row):
                    if bit == "#":
                        d.rectangle([x + c * scale, y + r * scale, x + (c + 1) * scale - 1,
                                     y + (r + 1) * scale - 1], fill=fill)
        x += 6 * scale
    return x


def text_width(s, scale=1):
    return (6 * len(s) - 1) * scale


# --------------------------------------------------------------------------
# Sleep art: a cover made of comic panels (sunset with palm, helicopter, the
# character, a car, the skyline) around a band with the roman numeral VI
# --------------------------------------------------------------------------
def palm(d, x, y, h, lean_x=4, fill=INK):
    """Trunk from (x, y) up to height h, with a crown of leaves."""
    top = (x + lean_x, y - h)
    d.line([(x, y), (x + lean_x // 2, y - h // 2), top], fill=fill, width=2)
    for ang, ln in ((205, h * 0.55), (165, h * 0.6), (125, h * 0.45), (55, h * 0.45), (15, h * 0.6),
                    (-25, h * 0.55)):
        a = math.radians(ang)
        mid = (top[0] + ln * 0.6 * math.cos(a), top[1] - ln * 0.6 * math.sin(a) - 2)
        end = (top[0] + ln * math.cos(a), top[1] - ln * math.sin(a) + ln * 0.4)
        d.line([top, mid, end], fill=fill, width=1)
        d.line([(top[0], top[1] + 1), (mid[0], mid[1] + 1), (end[0], end[1] + 1)], fill=fill)


def scene_sunset(d, w, h):
    """Retro sun cut by stripes, sea lines and a palm tree in front."""
    r = int(min(w, h) * 0.36)
    cx, cy = int(w * 0.58), int(h * 0.56)
    d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=INK)
    for i, y in enumerate(range(cy - 1, cy + r, 4)):
        d.rectangle([cx - r, y, cx + r, y + min(i, 2)], fill=PAPER)
    d.rectangle([0, cy + int(r * 0.75), w, h], fill=PAPER)
    for i, y in enumerate(range(cy + int(r * 0.75) + 1, h, 3)):
        d.line([(1 + (i % 2) * 3, y), (w - 2 - (i % 2) * 4, y)], fill=INK)
    palm(d, int(w * 0.24), h - 2, int(h * 0.74), 4)


def scene_helicopter(d, w, h):
    cx, cy = w // 2 + 1, h // 2 + 2
    d.ellipse([cx - 8, cy - 5, cx + 5, cy + 5], fill=INK)                     # cabin
    d.polygon([(cx + 2, cy - 3), (cx + 12, cy - 1), (cx + 12, cy + 1), (cx + 2, cy + 2)], fill=INK)
    d.line([(cx + 12, cy - 5), (cx + 12, cy + 2)], fill=INK)                 # tail rotor
    d.rectangle([cx - 6, cy - 3, cx - 2, cy], fill=PAPER)                    # window
    d.line([(cx - 2, cy - 5), (cx - 2, cy - 8)], fill=INK)                   # mast
    d.line([(cx - 13, cy - 8), (cx + 10, cy - 8)], fill=INK)                 # rotor
    d.line([(cx - 9, cy + 8), (cx + 4, cy + 8)], fill=INK)                   # skid
    d.line([(cx - 6, cy + 5), (cx - 6, cy + 8)], fill=INK)
    d.line([(cx + 1, cy + 5), (cx + 1, cy + 8)], fill=INK)
    for i in range(3):                                                       # searchlight
        d.line([(cx - 7 - i * 2, cy + 10 + i * 3), (cx - 9 - i * 3, cy + 12 + i * 3)], fill=INK)


def scene_bust(d, w, h):
    """Head and shoulders of the character, seen from the front."""
    cx = w // 2
    d.polygon([(cx - 13, h), (cx - 10, h - 13), (cx - 4, h - 16), (cx + 4, h - 16), (cx + 10, h - 13),
               (cx + 13, h)], fill=INK)
    d.rectangle([cx - 3, h - 20, cx + 3, h - 15], fill=INK)
    d.ellipse([cx - 8, h - 36, cx + 8, h - 18], fill=INK)
    d.pieslice([cx - 9, h - 39, cx + 9, h - 23], 180, 360, fill=INK)         # cap
    d.rectangle([cx - 12, h - 31, cx + 9, h - 29], fill=INK)                 # brim
    d.rectangle([cx - 6, h - 27, cx + 6, h - 25], fill=PAPER)                # shades
    d.line([(cx - 1, h - 27), (cx, h - 25)], fill=INK)
    d.line([(cx - 5, h - 14), (cx, h - 8), (cx + 5, h - 14)], fill=PAPER)    # chain


def scene_car(d, w, h):
    """Sports car from the side with speed lines."""
    y = h - 9
    x0, x1 = 4, w - 3
    d.polygon([(x0, y), (x0 + 2, y - 6), (x0 + 11, y - 7), (x0 + 16, y - 13), (x1 - 12, y - 13),
               (x1 - 5, y - 7), (x1, y - 5), (x1, y)], fill=INK)
    d.polygon([(x0 + 14, y - 8), (x0 + 18, y - 12), (x0 + 22, y - 12), (x0 + 22, y - 8)], fill=PAPER)
    d.polygon([(x0 + 24, y - 8), (x0 + 24, y - 12), (x1 - 13, y - 12), (x1 - 8, y - 8)], fill=PAPER)
    for wx in (x0 + 9, x1 - 9):
        d.ellipse([wx - 5, y - 4, wx + 5, y + 6], fill=INK)
        d.ellipse([wx - 2, y - 1, wx + 2, y + 3], fill=PAPER)
    d.line([(0, h - 3), (w, h - 3)], fill=INK)
    for i, yy in enumerate((6, 11, 16)):
        d.line([(2 + i * 3, yy), (14 + i * 4, yy)], fill=INK)


def scene_skyline(d, w, h):
    x, n = 0, 5
    for bw, bh in ((7, 0.55), (8, 0.85), (6, 0.45), (9, 0.7), (7, 0.95), (8, 0.5)):
        top = h - int(h * bh)
        d.rectangle([x, top, x + bw - 1, h], fill=INK)
        for wy in range(top + 3, h - 2, 4):
            for wx in range(x + 2, x + bw - 1, 3):
                n = (n * 1103515245 + 12345) & 0x7FFFFFFF
                if n % 3 != 0:
                    d.rectangle([wx, wy, wx, wy + 1], fill=PAPER)
        x += bw
    d.ellipse([w - 11, 3, w - 4, 10], fill=INK)                              # moon
    d.ellipse([w - 13, 2, w - 7, 8], fill=PAPER)


def scene_fighter(d, w, h):
    """The character with the bat, half size, as a silhouette."""
    s = 0.5
    draw_figure(d, POSES["bat1"], s=s, ox=w / 2 - 31 * s, oy=h - 1 - GROUND * s, small=False)


def panel(img, box, scene):
    """Draws a scene in its own box with a frame around it."""
    x0, y0, x1, y1 = box
    w, h = x1 - x0 + 1, y1 - y0 + 1
    sub = Image.new("L", (w, h), PAPER)
    d = ImageDraw.Draw(sub)
    scene(d, w, h)
    d.rectangle([0, 0, w - 1, h - 1], outline=INK)
    img.paste(sub, (x0, y0))


def sleep_art(w, h):
    """Upright art: 68 x 160 for the nice!view, 32 x 128 for the OLED."""
    img = Image.new("L", (w, h), PAPER)
    d = ImageDraw.Draw(img)
    if w >= 60:
        panel(img, (0, 0, 39, 49), scene_sunset)
        panel(img, (41, 0, 67, 27), scene_helicopter)
        panel(img, (41, 29, 67, 67), scene_bust)
        panel(img, (0, 51, 39, 77), scene_car)
        # Band with the numeral: paper letters on ink.
        d.rectangle([0, 79 if False else 69, 67, 69], fill=PAPER)
        d.rectangle([0, 80, 67, 106], fill=INK)
        text(d, (68 - text_width("VI", 3)) // 2, 83, "VI", 3, fill=PAPER)
        d.line([(3, 83), (14, 83)], fill=PAPER)
        d.line([(3, 103), (14, 103)], fill=PAPER)
        d.line([(53, 83), (64, 83)], fill=PAPER)
        d.line([(53, 103), (64, 103)], fill=PAPER)
        panel(img, (0, 109, 33, 159), scene_skyline)
        panel(img, (35, 109, 67, 159), scene_fighter)
    else:
        panel(img, (0, 0, 31, 43), scene_sunset)
        d.rectangle([0, 46, 31, 66], fill=INK)
        text(d, (32 - text_width("VI", 2)) // 2, 49, "VI", 2, fill=PAPER)
        panel(img, (0, 69, 31, 97), scene_helicopter)
        panel(img, (0, 99, 31, 127), scene_car)
    return img


# --------------------------------------------------------------------------
# Car chase seen from above: a getaway car and a police car on a scrolling road
# --------------------------------------------------------------------------
CHASE_W, CHASE_H, CHASE_FRAMES = 68, 69, 16
ROAD_L, ROAD_R = 9, 58


def car(d, cx, y, police=False, light=0):
    """Car pointing up, 16 wide and 28 long, with its nose at y."""
    x0, x1 = cx - 8, cx + 7
    d.rounded_rectangle([x0, y, x1, y + 27], radius=4, fill=INK)
    # Wheels.
    for wy in (y + 4, y + 19):
        d.rectangle([x0 - 2, wy, x0 - 1, wy + 5], fill=INK)
        d.rectangle([x1 + 1, wy, x1 + 2, wy + 5], fill=INK)
    # Windscreen, rear window and headlights in paper colour.
    d.polygon([(x0 + 3, y + 8), (x1 - 3, y + 8), (x1 - 2, y + 12), (x0 + 2, y + 12)], fill=PAPER)
    d.rectangle([x0 + 3, y + 21, x1 - 3, y + 23], fill=PAPER)
    d.point((x0 + 2, y + 1), fill=PAPER)
    d.point((x1 - 2, y + 1), fill=PAPER)
    if police:
        # Light bar on the roof: the two halves flash in turn.
        d.rectangle([x0 + 2, y + 14, x1 - 2, y + 17], fill=PAPER)
        if light == 0:
            d.rectangle([x0 + 3, y + 15, cx - 1, y + 16], fill=INK)
        else:
            d.rectangle([cx + 1, y + 15, x1 - 3, y + 16], fill=INK)
        d.line([(cx, y + 20), (cx, y + 26)], fill=PAPER)
    else:
        # Stripe along the bonnet of the getaway car.
        d.line([(cx, y + 2), (cx, y + 6)], fill=PAPER)
        d.line([(cx - 1, y + 14), (cx + 1, y + 14)], fill=PAPER)
        d.line([(cx - 1, y + 17), (cx + 1, y + 17)], fill=PAPER)


def chase_frame(i):
    img = Image.new("L", (CHASE_W, CHASE_H), PAPER)
    d = ImageDraw.Draw(img)
    t = i / CHASE_FRAMES

    # Road edges, kerb marks and the centre line scroll down.
    d.line([(ROAD_L, 0), (ROAD_L, CHASE_H)], fill=INK)
    d.line([(ROAD_R, 0), (ROAD_R, CHASE_H)], fill=INK)
    off = int(round(t * 16)) % 16
    for y in range(-16, CHASE_H + 16, 16):
        d.rectangle([33, y + off, 34, y + off + 7], fill=INK)
    for y in range(-8, CHASE_H + 8, 8):
        yy = y + (off % 8)
        d.line([(ROAD_L - 5, yy), (ROAD_L - 2, yy + 3)], fill=INK)
        d.line([(ROAD_R + 2, yy + 3), (ROAD_R + 5, yy)], fill=INK)

    # The getaway car changes lane; the police car follows a moment later.
    def lane(phase):
        return 34 + 12 * math.sin(2 * math.pi * phase)

    gx = int(round(lane(t)))
    px = int(round(lane(t - 0.18)))
    gy = 3 + int(round(2 * math.sin(4 * math.pi * t)))
    py = 38 + int(round(2 * math.sin(4 * math.pi * t + 1.5)))

    car(d, gx, gy)
    car(d, px, py, police=True, light=(i // 2) % 2)

    # Flash of the siren beside the police car.
    if (i // 2) % 2 == 0:
        d.line([(px - 13, py + 12), (px - 11, py + 14)], fill=INK)
        d.line([(px - 13, py + 19), (px - 11, py + 17)], fill=INK)
    else:
        d.line([(px + 12, py + 12), (px + 10, py + 14)], fill=INK)
        d.line([(px + 12, py + 19), (px + 10, py + 17)], fill=INK)
    return img


# --------------------------------------------------------------------------
# Stars for the Bluetooth profiles (10 x 10)
# --------------------------------------------------------------------------
STAR = [
    "....##....",
    "....##....",
    "...####...",
    "##########",
    ".########.",
    "..######..",
    "..######..",
    ".###..###.",
    ".##....##.",
    "##......##",
]


def star(filled):
    img = Image.new("L", (10, 10), PAPER)
    px = img.load()
    for y, row in enumerate(STAR):
        for x, c in enumerate(row):
            if c == "#":
                px[x, y] = INK
    if not filled:
        # Outline only: clear every ink pixel whose four neighbours are ink too.
        src = img.copy().load()
        for y in range(10):
            for x in range(10):
                if src[x, y] and all(0 <= x + dx < 10 and 0 <= y + dy < 10 and src[x + dx, y + dy]
                                     for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                    px[x, y] = PAPER
    return img


# --------------------------------------------------------------------------
# Output
# --------------------------------------------------------------------------
def rotate_cw(img):
    return img.transpose(Image.Transpose.ROTATE_270)


def bitmap_rows(img):
    w, h = img.size
    px = img.load()
    stride = (w + 7) // 8
    rows = []
    for y in range(h):
        row = [0] * stride
        for x in range(w):
            if px[x, y] >= 128:
                row[x // 8] |= 0x80 >> (x % 8)
        rows.append("    " + ", ".join("0x%02x" % b for b in row) + ",")
    return "\n".join(rows)


def c_image(name, img, attr, img_inverted=None):
    """One LVGL I1 image with the two palettes (normal and inverted colours).

    img_inverted is a second drawing for displays that show light pixels on a dark
    background (CONFIG_NICE_OLED_WIDGET_STREET_LIGHT_ON_DARK); without it there is
    only one drawing.
    """
    w, h = img.size
    body = bitmap_rows(img)
    if img_inverted is None:
        data = f"""#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#else
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#endif
{body}"""
    else:
        data = f"""#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#else
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#endif
#if CONFIG_NICE_OLED_WIDGET_STREET_LIGHT_ON_DARK
{bitmap_rows(img_inverted)}
#else
{body}
#endif"""
    return f"""#ifndef LV_ATTRIBUTE_IMG_{attr}
#define LV_ATTRIBUTE_IMG_{attr}
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_{attr} uint8_t {name}_map[] = {{
{data}
}};

const lv_image_dsc_t {name} = {{
    .header.cf = LV_COLOR_FORMAT_I1,
    .header.w = {w},
    .header.h = {h},
    .data_size = sizeof({name}_map),
    .data = {name}_map,
}};
"""


HEADER = """/*
 * ========================================
 * FileName: {file}
 * Date: 19.November.2026
 * Author: Marcos Chow Castro
 * Email: mctechnology170318@gmail.com
 * GitHub: https://github.com/mctechnology17
 * Brief: {brief}
 * =========================================
 * SPDX-License-Identifier: MIT
 *
 * Generated by tools/street_assets.py: change the drawing there, not here.
 */

#include <lvgl.h>

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

"""

FOOTER = "\n/* vim: set ft=c fdm=marker: */\n"


def write_c(file, brief, images, rotate=True):
    """rotate=False for images drawn on the canvas, which is rotated as a whole later."""
    path = os.path.join(ASSETS, file)
    with open(path, "w") as f:
        f.write(HEADER.format(file=file, brief=brief))
        turn = rotate_cw if rotate else (lambda im: im)
        parts = []
        for entry in images:
            inv = turn(entry[2]) if len(entry) > 2 else None
            parts.append(c_image(entry[0], turn(entry[1]), entry[0].upper(), inv))
        f.write("\n".join(parts))
        f.write(FOOTER)
    return path


def write_header():
    """Declarations and the frame list of every animation, for widgets/street.c."""
    path = os.path.join(ASSETS, "street_images.h")
    out = [HEADER.format(file="street_images.h",
                         brief="frames of every animation of the street character of zmk-nice-oled.")
           .replace("#include <lvgl.h>\n\n#ifndef LV_ATTRIBUTE_MEM_ALIGN\n#define LV_ATTRIBUTE_MEM_ALIGN\n#endif\n\n",
                    "#pragma once\n\n#include <lvgl.h>\n\n")]
    for suffix, cond in (("_small", "#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_STREET_SMALL)"), ("", "#else")):
        out.append(cond + "\n")
        for anim, count in FRAME_COUNT.items():
            names = ["street_%s_%d%s" % (anim, i, suffix) for i in range(count)]
            out.append("".join("LV_IMG_DECLARE(%s);\n" % n for n in names))
            out.append("static const lv_image_dsc_t *const street_%s_frames[] = {\n" % anim)
            for i in range(0, count, 4):
                out.append("    " + ", ".join("&" + n for n in names[i:i + 4]) + ",\n")
            out.append("};\n\n")
    out.append("#endif\n")
    out.append(FOOTER)
    with open(path, "w") as f:
        f.write("".join(out))
    return path


def sheet(images, cols, scale=4, pad=4):
    """Contact sheet for looking at the art: black ink on white, like the display."""
    w = max(e[1].size[0] for e in images)
    h = max(e[1].size[1] for e in images)
    rows = (len(images) + cols - 1) // cols
    out = Image.new("L", (cols * (w + pad) + pad, rows * (h + pad) + pad), 160)
    for n, entry in enumerate(images):
        im = entry[1]
        x = pad + (n % cols) * (w + pad)
        y = pad + (n // cols) * (h + pad)
        out.paste(im.point(lambda v: 0 if v >= 128 else 255), (x, y))
    return out.resize((out.size[0] * scale, out.size[1] * scale), Image.Resampling.NEAREST)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawTextHelpFormatter)
    ap.add_argument("--preview", metavar="DIR", nargs="?", const="/tmp",
                    help="also write PNG contact sheets in DIR (default /tmp)")
    ap.add_argument("--no-write", action="store_true", help="do not write the C files")
    args = ap.parse_args()

    # Third value: the drawing for a display that shows light pixels on a dark background.
    big = [("street_" + n, character_big(n), character_big(n, lit=True)) for n in FRAMES]
    small = [("street_" + n + "_small", character_small(n)) for n in FRAMES]
    sleep = [("street_sleep_68x160", sleep_art(68, 160)), ("street_sleep_32x128", sleep_art(32, 128))]
    chase = [("street_chase_%02d" % i, chase_frame(i)) for i in range(CHASE_FRAMES)]
    stars = [("street_star", star(False)), ("street_star_active", star(True))]

    if not args.no_write:
        for path in (
            write_c("street_images.c", "the street character of zmk-nice-oled, big (68x90) and small (32x24) sprites.", big + small),
            write_c("street_sleep_art.c", "sleep art of the street theme of zmk-nice-oled (nice!view and OLED).", sleep),
            write_c("street_chase.c", "car chase animation of the street theme of zmk-nice-oled (peripheral half).", chase),
            write_c("street_stars.c", "stars for the Bluetooth profiles of the street theme of zmk-nice-oled.", stars, rotate=False),
        ):
            print("written", os.path.relpath(path))
        print("written", os.path.relpath(write_header()))

    if args.preview:
        # The inverted drawing as the display shows it: ink is light, so the sheet is negated.
        lit_sheet = sheet([(n, lit.point(lambda v: 255 - v)) for n, _, lit in big], 8, 3)
        lit_sheet.save(os.path.join(args.preview, "street_big_inverted.png"))
        for name, imgs, cols, scale in (("street_big", big, 8, 3), ("street_small", small, 8, 6),
                                        ("street_sleep", sleep, 2, 4), ("street_chase", chase, 8, 3),
                                        ("street_stars", stars, 2, 12)):
            p = os.path.join(args.preview, name + ".png")
            sheet(imgs, cols, scale).save(p)
            print("preview", p)
    return 0


if __name__ == "__main__":
    sys.exit(main())

# vim: set ft=python fdm=marker:
