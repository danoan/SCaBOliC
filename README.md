## SCaBOliC
Squared Curvature Boundary Correction (SCaBOliC) is a 
collection of modules that optimizes the energy described
in [1]. This library is part of the 
[BTools](https://github.com/danoan/BTools) project.

 [1] Antunes, D., Lachaud, J.O., Talbot H.: Digital 
 curvature evolution model for image segmentation. In:
 Couprie, M., Cousty, J., Kenmochi, Y., Mustafa, N. (eds.) 
 Discrete Geometry for Computer Imagery. pp 15-26. Springer
 International Publishing, Cham (2019).
 
 ## Dependencies
 
 [libboost1.66.0-dev](https://www.boost.org/users/history/version_1_66_0.html)
 
 [DGtal0.9](https://dgtal.org/download/)
 
 [opencv-3.3.0](https://opencv.org/releases.html)
 
 ## Build and Install
 
 In order to build BTools, you are going to need cmake.
 
 ```
 cd [SCaBOliC_ROOT_FOLDER]
 mkdir build
 cd build
 cmake ..
 make install
 ```
 
 There are plenty of configuration parameters, but usually, if 
 all dependencies are installed in their standard
 locations, that should be enough. Otherwise, if errors occur, 
 it is likely that a manual configuration should be
 done. In this scenario, ccmake is quite handy.
 
 ## How to use
 
 Check the lab/example folder.
 
 
