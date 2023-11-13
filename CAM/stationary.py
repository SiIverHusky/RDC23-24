import cv2
import numpy as np
import os

# Define the color boundaries in HSV
lower_red = np.array([0,30,30])
higher_red = np.array([25, 255, 255])

lower_blue = np.array([90,30,30])
higher_blue = np.array([150, 255, 255])

lower_green = np.array([35,30,30])
higher_green = np.array([90, 255, 255])


# Flags
red_flag = True
test_flag = True
mask_flag = False

# Capture Sources
cam = cv2.VideoCapture(0)
os.chdir(os.path.dirname(os.path.abspath(__file__)))
dft = cv2.imread("smboxes.jpg")
dif = cv2.imread("different.jpg")
ali = cv2.imread("notaligned.jpg")
tlt = cv2.imread("tilted.jpg")

img = dft


# Functions
def searchGreen(frame):
    print("Searching for green...")
    green_mask = cv2.inRange(frame, lower_green, higher_green)
    green_contours, _ = cv2.findContours(green_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if green_contours:
        max_contour = max(green_contours, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(max_contour)
        return x, y, w, h
    else:
        return None, None, None, None

def searchBoxes(cnt, templateBox, mask):
    print("Searching for boxes...")
    if templateBox is not None:
        _, _, tw, th = templateBox
        if tw is not None and th is not None:
            contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            boxes = []
            for cnt in contours:
                x, y, w, h = cv2.boundingRect(cnt)
                if 0.8 * tw * th < w * h < 1.2 * tw * th:
                    print(f'Found box at {x}, {y} with width {w} and height {h}')
                    boxes.append((x, y, w, h))
            return boxes
    return None

def classifyBoxes(frame, boxes, cmask, gmask):
    print("Classifying boxes...")
    classified_boxes = []
    boxes = [box for box in boxes if box]
    boxes.sort(key=lambda box: box[0])

    for box in boxes:
        print(box)
        x, y, w, h = box

        # Check if box is seen on cmask or gmask within the inner 40 pixels
        cmask_seen = np.any(cmask[y+20:y+h-20, x+20:x+w-20])
        gmask_seen = np.any(gmask[y+20:y+h-20, x+20:x+w-20])

        # Assign 1 if seen on cmask, 0 if seen on gmask
        if cmask_seen:
            classified_boxes.append((x, y, w, h, 1))
        elif gmask_seen:
            classified_boxes.append((x, y, w, h, 0))

    return classified_boxes

if __name__ == "__main__":
    boxes = []
    while True:
        if test_flag:
            frame = img
            ret = True
        else:
            ret, frame = cam.read()

        if not ret:
            break

        frame = cv2.GaussianBlur(frame, (5, 5), 0)  # Remove noise w/ Gaussian
        frame = cv2.medianBlur(frame, 5)            # Remove noise w/ Median

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)


        if red_flag:
            cmask = cv2.inRange(hsv, lower_red, higher_red)
        else:
            cmask = cv2.inRange(hsv, lower_blue, higher_blue)

        gmask = cv2.inRange(hsv, lower_green, higher_green)
        mask = cv2.bitwise_or(cmask, gmask)
        mask = cv2.erode(mask, (5,5), iterations=7)

        detected = cv2.bitwise_and(frame, frame, mask=mask)
        gframe = cv2.bitwise_and(frame, frame, mask=gmask)
        
        template = searchGreen(gframe)
#
        if template is not None:
            contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            for cnt in contours:
                foundBox = searchBoxes(cnt, template, mask)
                foundBox = [[boxes[0], boxes[1], boxes[2], boxes[3]] for boxes in foundBox]
                print(f'Search boxes returned {foundBox}')
                if foundBox is not None:
                    boxes = foundBox
                    print(f'Found {len(boxes)} boxes')
                    print(boxes)
                    boxes = [box for box in boxes if box is not None]
                    print(f'Found {len(boxes)} boxes not none')
                    print(boxes)
                    boxes = classifyBoxes(frame, boxes, cmask, gmask)

            for box in boxes:
                x, y, w, h, color = box
                if color == 1:
                    cv2.putText(detected, "1", (x + w//2, y + h//2), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
                elif color == 0:
                    cv2.putText(detected, "0", (x + w//2, y + h//2), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)

        
        if mask_flag:
            cv2.imshow("frame", detected)
        else:
            cv2.imshow("frame", frame)

        if cv2.waitKey(150) & 0xFF == ord('q'):
            break
        if cv2.waitKey(150) & 0xFF == ord('m'):
            mask_flag = not mask_flag
        if cv2.waitKey(150) & 0xFF == ord('r'):
            red_flag = True
        if cv2.waitKey(150) & 0xFF == ord('b'):
            red_flag = False
        if cv2.waitKey(150) & 0xFF == ord('t'):
            test_flag = not test_flag
        if cv2.waitKey(150) & 0xFF == ord('0'):
            img = dft
        if cv2.waitKey(150) & 0xFF == ord('1'):
            img = dif
        if cv2.waitKey(150) & 0xFF == ord('2'):
            img = ali
        if cv2.waitKey(150) & 0xFF == ord('3'):
            img = tlt
    
    cam.release()
    cv2.destroyAllWindows()