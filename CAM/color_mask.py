import cv2
import numpy as np

capture = cv2.VideoCapture(0)

# red = rgb(225, 81, 86)

lower_red = np.array([100, 50, 50])
higher_red = np.array([240, 255, 255])

show_detected = False

if __name__ == "__main__":
    while True:
        ret, frame = capture.read()

        # Converting to HSV
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        mask = cv2.inRange(hsv, lower_red, higher_red)
        detected = cv2.bitwise_and(frame, frame, mask=mask)

        if show_detected:
            cv2.imshow('frame', detected)
        else:
            cv2.imshow('frame', frame)
        if cv2.waitKey(20) == ord('q'):
            break
        elif cv2.waitKey(20) == ord('m'):
            show_detected = not show_detected


    capture.release()
    cv2.destroyAllWindows()

    cv2.waitKey(0)