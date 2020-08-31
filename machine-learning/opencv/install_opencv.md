

Setting up OpenCV and C++ development environment in Mac

1. Install OpenCV

To install OpenCV using brew, open a terminal and paste:

```
brew install opencv
```

This should install OpenCV 4.4

2. Install pkg-config

pkg-config is a helper tool used when compiling applications and libraries. This will be helpful for finding the correct linker flags for OpenCV.

```
brew install pkg-config
```

3. View OpenCV linker flags

To view the linker flags for OpenCV, run:

```
pkg-config --cflags --libs /usr/local/Cellar/opencv/4.4.0/lib/pkgconfig/opencv.pc
```

The output looks like:

```
-I/usr/local/Cellar/opencv/4.4.0/include/opencv4 -L/usr/local/Cellar/opencv/4.4.0/lib -lopencv_gapi -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_highgui -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_sfm -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_xfeatures2d -lopencv_shape -lopencv_ml -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core
```

4. Running your code on the terminal

Paste the below line which compiles the OpenCV code with the appropriate linker flags.

```
g++ $(pkg-config --cflags --libs /usr/local/Cellar/opencv/4.4.0/lib/pkgconfig/opencv4.pc) -std=c++11  yourFile.cpp -o yourFileProgram
```

Run the binary,

```
./yourFileProgram
```



