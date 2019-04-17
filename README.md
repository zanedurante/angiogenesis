# angiogenesis
Windows Desktop Application for Angiogenesis Analysis.

### Background
This application was used to analyze blood vessel growth in microscope images.  This resulted in a paper that was published in the American Diabetes Association.  See http://diabetes.diabetesjournals.org/content/67/Supplement_1/481-P.  

### Instructions to run
This project was completed using Microsoft Visual Studio 2017.  To open the project, open the `Angionalysis.sln` file in `Angionalysis/`.  Then, debug the project within Visual Studio itself or export it as an exe file.

To do analysis on an image, first convert it to jpg.  Then, within the application window, select the image and press the `convert` button.  Within a minute, the analysis should finish with results the mesh count.

If you are having any trouble running the application or just want the working executable directly, you can email me at zanedurante@gmail.com.

### Sample inputs and outputs
I put some sample inputs and their outputs below.  In addition to the images, the window of the application displays the number of meshes (red Xs) that are in the output image.  As you can see from the samples below, microscope images with greater blood vessel growth have a significantly higher number of meshes. 

sample input 1:
![m 9](https://user-images.githubusercontent.com/17008638/56182945-2bd5ae00-5fc9-11e9-871c-24fc9b9f8757.jpg)
sample output 1:
![m 9_enhanced](https://user-images.githubusercontent.com/17008638/56182950-2e380800-5fc9-11e9-8d4a-d8b2d83909f3.jpg)
sample input 2:
![0 5](https://user-images.githubusercontent.com/17008638/56182953-342de900-5fc9-11e9-9691-6101cc7942a2.jpg)
sample output 2:
![0 5_enhanced](https://user-images.githubusercontent.com/17008638/56182954-355f1600-5fc9-11e9-95cc-d611b6c11af5.jpg)
