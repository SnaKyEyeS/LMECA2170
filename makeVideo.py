import cv2
import glob
import imageio
import os, sys

dir = os.listdir('./build/Video')

dir.sort()

img1 = cv2.imread('build/Video/' + dir[-1] +'/00001.ppm')
height, width, layers = img1.shape

# Set FOURCC code to '24BG' - '24BG' is used for creating uncompressed raw video
video1 = cv2.VideoWriter('V1.avi', cv2.VideoWriter_fourcc(*'XVID'), 30, (width, height))

filenames = glob.glob('build/Video/' + dir[-1] + '/*.ppm')
filenames.sort()


try:
    for filename in filenames:
        img = cv2.imread(filename)

        # Display input image for debugging
        #cv2.imshow('img', img)
        #cv2.waitKey(1000)

        video1.write(img)
except:
     print('An error occurred.')



cv2.destroyAllWindows()
video1.release()

def convertFile(inputpath, targetFormat):
    #Reference: http://imageio.readthedocs.io/en/latest/examples.html#convert-a-movie
    outputpath = os.path.splitext(inputpath)[0] + targetFormat
    print("converting\r\n\t{0}\r\nto\r\n\t{1}".format(inputpath, outputpath))

    reader = imageio.get_reader(inputpath)
    fps = reader.get_meta_data()['fps']

    writer = imageio.get_writer(outputpath, fps=fps)
    for i,im in enumerate(reader):
        sys.stdout.write("\rframe {0}".format(i))
        sys.stdout.flush()
        writer.append_data(im)
    print("\r\nFinalizing...")
    writer.close()
    print("Done.")

convertFile('V1.avi', '.gif')
