import imageio
import glob
import cv2
import numpy as np
from PIL import ImageFont, ImageDraw, Image


def read_images(image_name_list):
    image_list = []
    size = Image.open(image_name_list[0]).size
    print ('size', size)
    for image_name in image_name_list:
        image = Image.open(image_name).resize(size, Image.ANTIALIAS)

        # 转为UMat，以使用addWeighted方法
        image = cv2.cvtColor(np.asarray(image), cv2.COLOR_RGB2BGR)
        image_list.append(image)
    print("图片读取完成")
    return image_list


def create_gif(image_list, gif_name, duration=2, k=9):
    """生成gif动图,
    image_list:图片矩阵列表；
    git_name：生成的gif；
    duration：两张影像之间的间隔时间，单位秒。
    """
    frames = []
    
    for index in range(len(image_list)-1):
        buff = gif_gradually(image_list[index+1], image_list[index], k)
        frames.extend(buff)
        
    # 生成gif,frames是图片列表，duration是间隔时间
    imageio.mimsave(gif_name, frames, 'GIF', duration=duration/k)
    print("gif图制作完成")


def gif_gradually(img1, img2, k=10):
    """生成两张影像之间的过度影像，渐变"""
    buff = []
    for i in range(k):
        alpha = i*1/k
        # 增加权重，显示两张图片之间的过渡图片
        img = cv2.addWeighted(img1, alpha, img2, (1-alpha), gamma=0)

        # 转为PIL
        img = cv2.cvtColor(np.asarray(img), cv2.COLOR_BGR2RGB)
        buff.append(img)

    return buff


class Compress_img:

    def __init__(self, img_path):
        self.img_path = img_path
        self.img_name = img_path.split('/')[-1]

    def compress_img_CV(self, compress_rate=0.2, show=False):
        img = cv2.imread(self.img_path)
        heigh, width = img.shape[:2]
        print (heigh, width)
        if  heigh*width*compress_rate < 200000:
            compress_rate = 200000./(heigh*width)
        # 双三次插值
        img_resize = cv2.resize(img, (int(width*compress_rate), int(heigh*compress_rate)),
                                interpolation=cv2.INTER_AREA)
        compress_path = 'compress/result_cv_' + self.img_name
        cv2.imwrite(compress_path, img_resize)
        print("%s 已压缩，" % (self.img_name), "压缩率：", compress_rate)
        if show:
            cv2.imshow(self.img_name, img_resize)
            cv2.waitKey(0)
        return compress_path

def compress(img_path):
    compress = Compress_img(img_path)
    return compress.compress_img_CV()

# gif生成
#filename_list = glob.glob(r'*.jpeg')

for i in range(1, 15):
#for i in range(4, 5):
    path = './resource/%d/*.jpg' % i
    filename_list = glob.glob(path)
    new_filename_list = []
    for name in filename_list:
        img_path = compress(name)
        new_filename_list.append(img_path)
    print (new_filename_list)
    final_list = [new_filename_list[0], new_filename_list[0], new_filename_list[1], new_filename_list[1], new_filename_list[0]]
    image_list = read_images(final_list)
    create_gif(image_list, "output/%d.gif" % i)

# 将jpg压缩之后生成gif
# 参考 
# https://blog.csdn.net/huanyue6660/article/details/79423326
# https://blog.csdn.net/weixin_41010198/article/details/106544789