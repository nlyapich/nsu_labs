import cv2

video = cv2.VideoCapture("./resources/ants.avi")

frameN = 0

while(True):
    success, frame = video.read()
    if not success:
        break
    # cv2.imwrite(f'./frames/frame_{frameN}.jpg', frame)
    # if (frameN <= 1020):
    #     frameN += 1
    #     continue

    if not cv2.imwrite(f'./frames/frame_{frameN}.jpg', frame):
        raise Exception("Could not write image")
    frameN += 1
    # print(frameN)

    if frameN >= 3030:
        break

video.release()
