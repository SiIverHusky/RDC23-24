import cv2

i = 1
cam = cv2.VideoCapture(i)

def changeCam(i, cam):
    cam = cv2.VideoCapture(i)

while True:

    ret, frame = cam.read()
    cv2.imshow("test", frame)

    if not ret:
        break

    if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    if cv2.waitKey(1) & 0xFF == ord('0'):
        changeCam(0 ,cam)
    if cv2.waitKey(1) & 0xFF == ord('1'):
        changeCam(1 ,cam)
    if cv2.waitKey(1) & 0xFF == ord('2'):
        changeCam(2 ,cam)

cam.release()
cv2.destroyAllWindows()