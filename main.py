from bhv import Bvh
from numpy import interp
import os
dirname = os.path.dirname(__file__)

def get_frame(frame):
    a = int(interp((round(mocap.frame_joint_channel(frame, "LegFront0.L", "Zrotation"), 0) + 90), [0, 180], [0, 2550]))
    b = int(interp((round(mocap.frame_joint_channel(frame, "LegFront1.L", "Yrotation"), 0)) + 90, [0, 180], [2550, 0]))
    c = int(interp((round(mocap.frame_joint_channel(frame, "LegFront2.L", "Yrotation"), 0)) + 90, [0, 180], [0, 2550]))
    l2 = [a, b, c]
    a = int(interp((round(mocap.frame_joint_channel(frame, "LegFront0.R", "Zrotation"), 0)) + 90, [0, 180], [0, 2550]))
    b = int(interp((round(mocap.frame_joint_channel(frame, "LegFront1.R", "Yrotation"), 0)) + 90, [0, 180], [2550, 0]))
    c = int(interp((round(mocap.frame_joint_channel(frame, "LegFront2.R", "Yrotation"), 0)) + 90, [0, 180], [0, 2550]))
    l1 = [a, b, c]
    a = int(interp((round(mocap.frame_joint_channel(frame, "LegBack0.L", "Zrotation"), 0)) + 90, [0, 180], [0, 2550]))
    b = int(interp((round(mocap.frame_joint_channel(frame, "LegBack1.L", "Yrotation"), 0)) + 90, [0, 180], [0, 2550]))
    c = int(interp((round(mocap.frame_joint_channel(frame, "LegBack2.L", "Yrotation"), 0)) + 90, [0, 180], [2550, 0]))
    l4 = [a, b, c]
    a = int(interp((round(mocap.frame_joint_channel(frame, "LegBack0.R", "Zrotation"), 0)) + 90, [0, 180], [0, 2550]))
    b = int(interp((round(mocap.frame_joint_channel(frame, "LegBack1.R", "Yrotation"), 0)) + 90, [0, 180], [0, 2550]))
    c = int(interp((round(mocap.frame_joint_channel(frame, "LegBack2.R", "Yrotation"), 0)) + 90, [0, 180], [2550, 0]))
    l3 = [a, b, c]
    return l1 + l2 + l3 + l4


if __name__ == '__main__':
    dirlist = os.listdir(dirname + "/files/")
    for filename in dirlist:
        with open(dirname + "/files/" + filename) as f:
            mocap = Bvh(f.read())
        animation = []
        print("> " + filename)
        for i in range(0, len(mocap.frames)):
            animation.append(get_frame(i))
        if len(mocap.frames) > 255:
            print("ANIMATION TOO LONG FOR COMPRESSION. MAX 256 FRAMES")
            input("Enter to end program")
            quit(0)

        dummy = []
        prev_frame = 127
        for m in range(0, 12):
            motor = []
            for i in range(0, len(mocap.frames)):
                if i == 0:
                    # do not touch the first frame
                    prev_frame = get_frame(i)[m]
                    motor.append(i)
                    motor.append(int(prev_frame/10))
                else:
                    new_frame = get_frame(i)[m]
                    if prev_frame == new_frame:
                        # if new frame is same with the prev one do not add it to the file
                        if i+1 == len(mocap.frames):
                            prev_frame = new_frame
                            motor.append(i)
                            motor.append(int(new_frame/10))

                    else:
                        prev_frame = new_frame
                        motor.append(i)
                        motor.append(int(new_frame/10))
            dummy += (motor.copy(),)
        compressed = dummy.copy()

        animname = "anim_" + filename[:-4]
        with open(animname + '.txt', 'w', encoding="utf-8") as f:
            f.write("const uint_least8_t " + animname + "[] PROGMEM {\n")
            f.write("        " + str(len(mocap.frames)) + ",\n")
            aaa = 0
            for i in compressed:
                aaa = aaa + 1
                f.write("        ")
                bbb = 0
                for x in i:
                    bbb = bbb + 1
                    if aaa == len(compressed):
                        if(bbb == len(i)):
                            f.write(str(x))
                        else:
                            f.write(str(x) + ", ")
                    else:
                        f.write(str(x) + ", ")
                if aaa != len(compressed):
                    f.write("\n")
            f.write("\n};")
        print("TASK DONE")
