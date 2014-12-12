These files contain the supplementary material for the FLSCV workshop (
in conjunction with ACCV 2014) titled "Image Based Visibility Estimation 
During Day and Night", Sami Varjo, Jari Hannuksela, University of Oulu. 

The original image data consist of a large number of losslessly compressed tiff images
containing 14-bit data and each capture contains 14 images. It is therefore
impossible to offer access under review for the whole data. 

Examples of HDR images are provided with the code to extract feature vectors and
for training SVM classifiers. The training of SVMs is based on the Matlab trainsvm
which is part of the statistics toolbox.

For demo, please invoke:
>runMe

Please note that the training here is based on random sampling and the exact 
results may differ slightly from the reported (run serveral experiments if in doubt).

some file contents:

runMe.m                 demo file
autoSumCellImages.m     create a HDR image
makeFeatureVector.m     create a feature vector from an image


HDRvisMe-retinex-y-proj-grad.mat contains feature vectors extracted from the original image data. 
    featureVector  each row contains the feature

labelData.mat contains the annotation and image capture time data ++:    
    imTime contains the capture times of the image
    imFile contain HDR image file names
    label (and manualLabel) contain the manual labeling of the data
    labelVaisala contains the labels based on Vaisala 12D instrument
    im_visGT contains the MOR from Vaisala 12D instrument corresponding to 
             the image files (and the featureVector)
