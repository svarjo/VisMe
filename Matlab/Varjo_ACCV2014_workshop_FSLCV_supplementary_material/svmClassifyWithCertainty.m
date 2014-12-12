function [group, confidence] = svmClassifyWithCertainty(svm_struct, Sample, label)
%function [group confidence] = svmClassify(svm_struct, Sample, label)
%
%   use svm_struct to classiby a sample so that also a confidence 
%   measure is received as distance from the decision boundary. 
%
if ~exist('label','var')
    label = [-1,1];
end

sv = svm_struct.SupportVectors;
alphaHat = svm_struct.Alpha;
bias = svm_struct.Bias;
kfun = svm_struct.KernelFunction;
kfunargs = svm_struct.KernelFunctionArgs;
f = (kfun(sv,Sample,kfunargs{:})'*alphaHat(:) + bias)-1;

confidence = abs(f)-1;

group(f>=0) = label(1);
group(f<0)  = label(2);
