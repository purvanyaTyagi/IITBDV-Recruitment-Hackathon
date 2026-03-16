
'''
PPC Hackathon — Participant Boilerplate
You must implement two functions: plan() and control()
'''

# ─── TYPES (for reference) ────────────────────────────────────────────────────

# Cone: {"x": float, "y": float, "side": "left" | "right", "index": int}
# State: {"x", "y", "yaw", "vx", "vy", "yaw_rate"}  
# CmdFeedback: {"throttle", "steer"}        

# ─── PLANNER ──────────────────────────────────────────────────────────────────
import numpy as np


'''
PPC Hackathon — Participant Boilerplate
You must implement two functions: plan() and control()
'''

# ─── TYPES (for reference) ────────────────────────────────────────────────────

# Cone: {"x": float, "y": float, "side": "left" | "right", "index": int}
# State: {"x", "y", "yaw", "vx", "vy", "yaw_rate"}  
# CmdFeedback: {"throttle", "steer"}        

# ─── PLANNER ──────────────────────────────────────────────────────────────────
import numpy as np

def plan(cones: list[dict]) -> list[dict]:

    # separate cones
    left_cones = [c for c in cones if c["side"] == "left"]
    right_cones = [c for c in cones if c["side"] == "right"]

    # sort cones by index
    left_cones.sort(key=lambda c: c["index"])
    right_cones.sort(key=lambda c: c["index"])

    path = []

    # compute midpoints
    for l, r in zip(left_cones, right_cones):

        mx = (l["x"] + r["x"]) / 2
        my = (l["y"] + r["y"]) / 2

        path.append({"x": mx, "y": my})

    return path
