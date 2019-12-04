# opencv2_proj
for digital image procesing

=========================
 use following commands:
=========================

cd cpp

cmake .

make

./MyCvProject


=============================
	directory tree
=============================
.
├── cpp
│   ├── CMakeCache.txt
│   ├── CMakeFiles
│   │   ├── 3.5.1
│   │   │   ├── CMakeCCompiler.cmake
│   │   │   ├── CMakeCXXCompiler.cmake
│   │   │   ├── CMakeDetermineCompilerABI_C.bin
│   │   │   ├── CMakeDetermineCompilerABI_CXX.bin
│   │   │   ├── CMakeSystem.cmake
│   │   │   ├── CompilerIdC
│   │   │   │   ├── a.out
│   │   │   │   └── CMakeCCompilerId.c
│   │   │   └── CompilerIdCXX
│   │   │       ├── a.out
│   │   │       └── CMakeCXXCompilerId.cpp
│   │   ├── cmake.check_cache
│   │   ├── CMakeDirectoryInformation.cmake
│   │   ├── CMakeOutput.log
│   │   ├── CMakeTmp
│   │   ├── feature_tests.bin
│   │   ├── feature_tests.c
│   │   ├── feature_tests.cxx
│   │   ├── Makefile2
│   │   ├── Makefile.cmake
│   │   ├── MyCvProject.dir
│   │   │   ├── build.make
│   │   │   ├── cmake_clean.cmake
│   │   │   ├── CXX.includecache
│   │   │   ├── DependInfo.cmake
│   │   │   ├── depend.internal
│   │   │   ├── depend.make
│   │   │   ├── flags.make
│   │   │   ├── link.txt
│   │   │   ├── main.cpp.o
│   │   │   ├── progress.make
│   │   │   └── src
│   │   │       └── cv2ImageProcessing.cpp.o
│   │   ├── progress.marks
│   │   └── TargetDirectories.txt
│   ├── cmake_install.cmake
│   ├── CMakeLists.txt
│   ├── img
│   │   ├── cdf
│   │   │   ├── cdf_eq.jpg
│   │   │   ├── cdf_joker.jpeg
│   │   │   ├── cdf_match.jpg
│   │   │   ├── cdf_ref13.jpg
│   │   │   ├── cdf_ref14.jpg
│   │   │   ├── cdf_ref15_25_hsv_match.jpg
│   │   │   ├── cdf_ref15_25_rgb_all_match.jpg
│   │   │   ├── cdf_ref15_25_rgb_ind_match.jpg
│   │   │   ├── cdf_ref15_25_v_match.jpg
│   │   │   ├── cdf_ref15_25_y_match.jpg
│   │   │   ├── cdf_ref15_25_yuv_match.jpg
│   │   │   ├── cdf_ref15.jpg
│   │   │   ├── cdf_ref15_rgb_all_eq.jpg
│   │   │   ├── cdf_ref15_rgb_ind_eq.jpg
│   │   │   ├── cdf_ref15_v_eq.jpg
│   │   │   ├── cdf_ref15_y_eq.jpg
│   │   │   ├── cdf_ref16.jpg
│   │   │   ├── cdf_ref1.jpeg
│   │   │   ├── cdf_ref25.jpg
│   │   │   ├── cdf_ref2.jpeg
│   │   │   ├── cdf_ref3.jpg
│   │   │   ├── cdf_ref4.jpg
│   │   │   ├── cdf_ref6.jpg
│   │   │   └── cdf_ref7.jpg
│   │   ├── cdf.zip
│   │   ├── diff
│   │   │   ├── diff_all_v_eq.jpg
│   │   │   ├── diff_all_v_match.jpg
│   │   │   ├── diff_all_y_eq.jpg
│   │   │   ├── diff_all_y_match.jpg
│   │   │   ├── diff_ind_v_eq.jpg
│   │   │   ├── diff_ind_v_match.jpg
│   │   │   ├── diff_ind_y_eq.jpg
│   │   │   ├── diff_ind_y_match.jpg
│   │   │   ├── diff_ref15_25_hsv_match.jpg
│   │   │   ├── diff_ref15_25_rgb_all_match.jpg
│   │   │   ├── diff_ref15_25_rgb_ind_match.jpg
│   │   │   ├── diff_ref15_25_v_match.jpg
│   │   │   ├── diff_ref15_25_y_match.jpg
│   │   │   ├── diff_ref15_25_yuv_match.jpg
│   │   │   ├── diff_ref15_rgb_all_eq.jpg
│   │   │   ├── diff_ref15_rgb_ind_eq.jpg
│   │   │   ├── diff_ref15_v_eq.jpg
│   │   │   └── diff_ref15_y_eq.jpg
│   │   ├── diff_15_25.zip
│   │   ├── output
│   │   │   ├── eq.jpg
│   │   │   ├── match.jpg
│   │   │   ├── ref15_25_hsv_match.jpg
│   │   │   ├── ref15_25_rgb_all_match.jpg
│   │   │   ├── ref15_25_rgb_ind_match.jpg
│   │   │   ├── ref15_25_v_match.jpg
│   │   │   ├── ref15_25_y_match.jpg
│   │   │   ├── ref15_25_yuv_match.jpg
│   │   │   ├── ref15_rgb_all_eq.jpg
│   │   │   ├── ref15_rgb_ind_eq.jpg
│   │   │   ├── ref15_v_eq.jpg
│   │   │   └── ref15_y_eq.jpg
│   │   ├── output.zip
│   │   ├── pdf
│   │   │   ├── pdf_eq.jpg
│   │   │   ├── pdf_joker.jpeg
│   │   │   ├── pdf_match.jpg
│   │   │   ├── pdf_ref13.jpg
│   │   │   ├── pdf_ref14.jpg
│   │   │   ├── pdf_ref15_25_hsv_match.jpg
│   │   │   ├── pdf_ref15_25_rgb_all_match.jpg
│   │   │   ├── pdf_ref15_25_rgb_ind_match.jpg
│   │   │   ├── pdf_ref15_25_v_match.jpg
│   │   │   ├── pdf_ref15_25_y_match.jpg
│   │   │   ├── pdf_ref15_25_yuv_match.jpg
│   │   │   ├── pdf_ref15.jpg
│   │   │   ├── pdf_ref15_rgb_all_eq.jpg
│   │   │   ├── pdf_ref15_rgb_ind_eq.jpg
│   │   │   ├── pdf_ref15_v_eq.jpg
│   │   │   ├── pdf_ref15_y_eq.jpg
│   │   │   ├── pdf_ref16.jpg
│   │   │   ├── pdf_ref1.jpeg
│   │   │   ├── pdf_ref25.jpg
│   │   │   ├── pdf_ref2.jpeg
│   │   │   ├── pdf_ref2.jpg
│   │   │   ├── pdf_ref3.jpg
│   │   │   ├── pdf_ref4.jpg
│   │   │   ├── pdf_ref6.jpg
│   │   │   └── pdf_ref7.jpg
│   │   ├── pdf.zip
│   │   └── src
│   │       ├── blackhole.jpg
│   │       ├── dark.bmp
│   │       ├── elephant.png
│   │       ├── hsv_v 12match14.png
│   │       ├── hsv_v 21match18.png
│   │       ├── hsv_v jokermatch10.png
│   │       ├── joker.jpeg
│   │       ├── logo.png
│   │       ├── mario.png
│   │       ├── nice_work.jpg
│   │       ├── pac_man.png
│   │       ├── pepper.bmp
│   │       ├── pig.jpg
│   │       ├── ref10.jpg
│   │       ├── ref11.jpg
│   │       ├── ref12.jpg
│   │       ├── ref13.jpg
│   │       ├── ref14.jpg
│   │       ├── ref15.jpg
│   │       ├── ref16.jpg
│   │       ├── ref17.jpg
│   │       ├── ref18.jpg
│   │       ├── ref19.jpg
│   │       ├── ref1.jpeg
│   │       ├── ref20.jpg
│   │       ├── ref21.jpg
│   │       ├── ref22.jpg
│   │       ├── ref23.jpg
│   │       ├── ref24.jpg
│   │       ├── ref25.jpg
│   │       ├── ref2.jpeg
│   │       ├── ref3.jpg
│   │       ├── ref4.jpg
│   │       ├── ref5.jpg
│   │       ├── ref6.jpg
│   │       ├── ref7.jpg
│   │       ├── ref8.jpg
│   │       ├── ref9.jpg
│   │       ├── rgb 10match13.png
│   │       ├── rgb 7match3.png
│   │       └── yuv 23match15.png
│   ├── main.cpp
│   ├── main_terminal.cpp
│   ├── Makefile
│   ├── MyCvProject
│   └── src
│       ├── cv2ImageProcessing.cpp
│       └── cv2ImageProcessing.h
├── qt
│   ├── build-cv2proj-Desktop_Qt_5_8_0_GCC_64bit-Debug
│   │   ├── cv2ImageProcessing.o
│   │   ├── main.o
│   │   ├── mainwindow.o
│   │   ├── Makefile
│   │   ├── moc_mainwindow.cpp
│   │   ├── moc_mainwindow.o
│   │   ├── moc_predefs.h
│   │   └── ui_mainwindow.h
│   ├── cv2proj
│   │   ├── cv2proj.pro
│   │   ├── cv2proj.pro.user
│   │   ├── main.cpp
│   │   ├── mainwindow.cpp
│   │   ├── mainwindow.h
│   │   ├── mainwindow.ui
│   │   └── ref1.jpeg
│   └── README.md
└── README.md

18 directories, 180 files
