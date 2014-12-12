function G = gradFun(img)
%function G = gradFun(img)
%
% return sum of gradient magnitudes in img (Sobel filtered)

% use eg for feature = blocproc(I, gradFun, [64 64]);
    H = [1 2 1; 0 0 0; -1 -2 -1];
    Gx = conv2(double(img),H,'same');
    Gy = conv2(double(img),H','same');    
    G  = sqrt(Gx.^2 + Gy.^2);

end