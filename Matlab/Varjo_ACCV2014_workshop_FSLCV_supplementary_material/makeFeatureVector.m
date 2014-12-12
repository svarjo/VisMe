function featureVector = makeFeatureVector(I)
%function featureVector = makeFeatureVector(I)
%
%   Apply mutiScaleRetinex, take gradient magnitude and normalize

Ir = multiScaleRetinex(double(I));

v1 = sum(double(Ir),2);
gv1 = abs(v1(2:end)-v1(1:end-1)) ;
gv1 = gv1./max(gv1);

featureVector = gv1;

end