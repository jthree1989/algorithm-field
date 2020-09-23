#! /usr/bin/python3
from PIL import Image
import numpy as np
import scipy.ndimage as ndimage


def detect_corners(np_data : np.ndarray) -> list:
    # 1. Convert to double grayscale image
    np_data = np_data.astype(float)
    np_data_gray = rgb2gray(np_data)
    # 2. Compute image derivative
    sobel_mask = np.array([
        [1, 0, -1],
        [2, 0, -2],
        [1, 0, -1]
    ])
    sobel_mask_bi = np.array([
        [2, 2, 0],
        [2, 0, -2],
        [0, -2, -2]
    ])
    np_gray_du = ndimage.convolve(np_data_gray, sobel_mask)
    np_gray_dv = ndimage.convolve(np_data_gray, sobel_mask.transpose())
    np_gray_bi = ndimage.convolve(np_data_gray, sobel_mask_bi)

    im_du = Image.fromarray(np_gray_du)
    im_du.show()
    im_dv = Image.fromarray(np_gray_dv)
    im_dv.show()




def rgb2gray(rgb: np.ndarray) -> np.ndarray:
    r, g, b = rgb[:, :, 0], rgb[:, :, 1], rgb[:, :, 2]
    gray = 0.2989 * r + 0.5870 * g + 0.1140 * b
    return gray


if __name__ == "__main__":
    im = Image.open('data/pattern.png').convert('RGB')
    np_im = np.array(im)
    detect_corners(np_im)


