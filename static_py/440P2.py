#Miles Mahon
import numpy as np
import cv2 #idk wouldnt let me change cv2.so (2.4) to the new one from 3.2
import matplotlib.pyplot as plt

cap = cv2.VideoCapture(0)

lastVal = 1
lastValFist = 1

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()

    # Our operations on the frame come here
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    kernel = np.ones((5,5),np.uint8)
    #gray = cv2.erode(gray,kernel,iterations = 7)

    #TODO: use skin detection first! then threshold!

    #gray = cv2.dilate(gray,kernel,iterations = 3)

    #TRYING CONTOURS
    #FROM STACK OVERFLOW:
    im = gray
    # ret,thresh = cv2.threshold(gray,127,255,0)
    #ret,thresh = cv2.threshold(gray,167,255,0)

    # Detect contours using both methods on the same image
    #contours1, _ = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_NONE)
    # contours1 = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    # image, contours1, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    # hull = cv2.convexHull(contours1[0],returnPoints = False)
    # defects = cv2.convexityDefects(cnt,hull)

    

    #for cnt in contours1:
        # rect = cv2.boundingRect(i)
        # cv2.rectangle(frame, rect)
    # cnt = contours1
    #print(len(contours1))
    #
    # for cnt in contours1:
    #     #print(contours1)
    #     rect = cv2.minAreaRect(cnt)
    #     box = cv2.boxPoints(rect)
    #     box = np.int0(box)
    #     cv2.drawContours(gray,[box],0,(0,0,255),2)

    #print(contours1)
    # Copy over the original image to separate variables
    img1 = im.copy()


    # Draw both contours onto the separate images
    #cv2.drawContours(frame, contours1, -1, (255,0,0), 3)





    #END CONTOURS

    #cv2.matchShapes(contour1, contour2, method, parameter)

    #http://ascii.co.uk/art/fist

    asciifist = """
      .----.-----.-----.-----.
     /      \     \     \     \\
    |  \/    |     |   __L_____L__
    |   |    |     |  (           \\
    |    \___/    /    \______/    |
    |        \___/\___/\___/       |
     \      \     /               /
      |                        __/
       \_                   __/
        |        |         |
        |                  |
        |                  |

    """



    #Trying TEMPLATE MATCHING
    # img = cv2.imread('dogwithball.jpg',0)
    # img2 = frame.copy()
    template = cv2.imread('Hand.jpg',1)
    #template = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
    w, h = template.shape[:-1]
    #template = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)

    #method = eval('cv2.TM_CCOEFF')
    method = eval('cv2.TM_CCORR_NORMED')
    #Other choices:
    #cv2.TM_CCOEFF
    # CV_TM_SQDIFF #bad
    # CV_TM_SQDIFF_NORMED #bad
    # CV_TM_CCORR #bad
    # CV_TM_CCORR_NORMED
    # CV_TM_CCOEFF
    # CV_TM_CCOEFF_NORMED
    #
    #     # Apply template Matching
    res = cv2.matchTemplate(frame,template,method)
    min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
    # top_left = max_loc
    # bottom_right = (top_left[0] + w, top_left[1] + h)
        #
    #print("Fist", max_val)
    if (max_val > 0.875 and lastValFist < 0.875): #anywhere from 0.84 to 0.89+
        print(asciifist)
    #cv2.rectangle(gray,top_left, bottom_right, 255, 2)
    lastValFist = max_val





    asciiarm = """
    _______                                                           __
           ~~~~~~~------......_______                         __..--''  \\
                                     ~~~---...________.---'~~~           `.
                                                                           \\
                                                                   __/      `.
                                                                .-' \ `-.     \\
                                                            .  /\   /  / /    /
                                                             `/ /  /  /.~    /
                                                               `-.______.-----.
    `------------------------------.......______                           |~~ `.
                                                ~~--.._                    |___))
                                                       ~--._____________.------'
    """









#AGAIN
    # template = cv2.imread('Hand_flipped.jpg',1)
    # #template = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
    # w, h = template.shape[:-1]
    # #template = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
    #
    # #method = eval('cv2.TM_CCOEFF')
    # #
    # #     # Apply template Matching
    # res = cv2.matchTemplate(frame,template,method) #note matching on FRAME not GRAY
    # min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
    # top_left = max_loc
    # bottom_right = (top_left[0] + w, top_left[1] + h)
        #

    #cv2.rectangle(gray,top_left, bottom_right, 255, 2)


#AGAIN!
    # template = cv2.imread('OpenHand_flipped.jpg',1)
    # #template = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
    # w, h = template.shape[:-1]
    # #template = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
    #
    # #method = eval('cv2.TM_CCOEFF')
    # #
    # #     # Apply template Matching
    # res = cv2.matchTemplate(frame,template,method) #note matching on FRAME not GRAY
    # min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
    # top_left = max_loc
    # bottom_right = (top_left[0] + w, top_left[1] + h)
    #     #
    #
    # print("Hand: ", max_val)
    # cv2.rectangle(gray,top_left, bottom_right, 255, 2)

#AGAIN!
    template = cv2.imread('Arm.jpg',1)
    #template = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
    w, h = template.shape[:-1]
    #template = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)

    #method = eval('cv2.TM_CCOEFF')
    #
    #     # Apply template Matching
    res = cv2.matchTemplate(frame,template,method) #note matching on FRAME not GRAY
    min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
    # top_left = max_loc
    # bottom_right = (top_left[0] + w, top_left[1] + h)
        #

    #print("Arm: ", max_val)
    if (max_val > 0.97 and lastVal < 0.97):
        print(asciiarm)
    #cv2.rectangle(gray,top_left, bottom_right, 255, 2)
    lastVal = max_val

#AGAIN!
    # template = cv2.imread('OpenHand_flipped.jpg',1)
    # #template = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
    # w, h = template.shape[:-1]
    # #template = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
    #
    # #method = eval('cv2.TM_CCOEFF')
    # #
    # #     # Apply template Matching
    # res = cv2.matchTemplate(frame,template,method) #note matching on FRAME not GRAY
    # min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
    # top_left = max_loc
    # bottom_right = (top_left[0] + w, top_left[1] + h)
        #

    #cv2.rectangle(gray,top_left, bottom_right, 255, 2)









    #HAD "FRAME INSTEAD OF GRAY
        #
    # plt.subplot(121),plt.imshow(res,cmap = 'gray')
    # plt.title('Matching Result'), plt.xticks([]), plt.yticks([])
    # plt.subplot(122),plt.imshow(frame,cmap = 'gray')
    # plt.title('Detected Point'), plt.xticks([]), plt.yticks([])
    # plt.suptitle("HEY")

    #plt.show()

    #END TEMPLATE MATCHING




    # Display the resulting frame
    cv2.imshow('frame',gray)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break












# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()


#FINDCONTOURS OF FRAMEDIFFERENCES MOTION ENERGY IMAGE
#CHECK THAT THE CONTOUR IS HORIUZONTAL





#TEMPLATE MATCHING:
# import cv2
# import numpy as np
# from matplotlib import pyplot as plt
#
# img = cv2.imread('dogwithball.jpg',0)
# img2 = img.copy()
# template = cv2.imread('tennis-ball.jpg',0)
# w, h = template.shape[::-1]
#
# # All the 6 methods for comparison in a list
# methods = ['cv2.TM_CCOEFF', 'cv2.TM_CCOEFF_NORMED', 'cv2.TM_CCORR',
#             'cv2.TM_CCORR_NORMED', 'cv2.TM_SQDIFF', 'cv2.TM_SQDIFF_NORMED']
#
# for meth in methods:
#     img = img2.copy()
#     method = eval(meth)
#
#     # Apply template Matching
#     res = cv2.matchTemplate(img,template,method)
#     min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
#
#     # If the method is TM_SQDIFF or TM_SQDIFF_NORMED, take minimum
#     if method in [cv2.TM_SQDIFF, cv2.TM_SQDIFF_NORMED]:
#         top_left = min_loc
#     else:
#         top_left = max_loc
#     bottom_right = (top_left[0] + w, top_left[1] + h)
#
#     cv2.rectangle(img,top_left, bottom_right, 255, 2)
#
#     plt.subplot(121),plt.imshow(res,cmap = 'gray')
#     plt.title('Matching Result'), plt.xticks([]), plt.yticks([])
#     plt.subplot(122),plt.imshow(img,cmap = 'gray')
#     plt.title('Detected Point'), plt.xticks([]), plt.yticks([])
#     plt.suptitle(meth)
#
#     plt.show()
