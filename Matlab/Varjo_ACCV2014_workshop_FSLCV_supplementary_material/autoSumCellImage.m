function  [sumI, nImages] = autoSumCellImage( Ic, expTime, op, gamma, force)
%[sumI h nImages] = autoSumCellImage( Ic, threshold, op )
%
% Create a HDR image from a stack of images
%
% Input:    Ic          - a cell array of input images
%           expTime     - the exposure times of images in Ic
%           threshold   - the fraction of histogram values that should be
%                         located before the mid point of histogram
%                         (default 0.45). I.E. larger the value, darker the
%                         image. The case fulfilling the threshold value
%                         stops the summation.
%           op          - operation for image wighting
%                         'linear' - sum( expTime.*Ic ) , (default)
%           gamma       - apply sumI.^gamma (def 1.0 - i.e. no gamma)
%           force       - require at least N images


if ~exist( 'expTime','var'), expTime = [25 50 100 300 900 2700 8100 24300 72900 218700 656100 1968300 5904900 11809800 23619600 47239200 47239200 70858800 70858800]; end
if ~exist( 'op', 'var'), op = 'linear'; end
if ~exist( 'gamma', 'var'), gamma = 1.0; end
if ~exist( 'force', 'var'), force = 1; end

sumI = zeros(size(Ic{1}),'double');
nImg = min(length(Ic), length(expTime));
val = zeros(1,nImg);

for imId = 1:nImg
    
    %protect from over saturated images (see from histogram) (over exp)
    h = imhist( normaliseTo8bit( Ic{imId} ) );
    csh = cumsum(h)./sum(h);
    if csh(1) > 0.999 || csh(end-1) < 0.001 %under | over exp
        continue;
    end
    
    %if (csh(end-1) < 0.5 && csh(end) > 0.5) %over exp csh(end) ==1 always!
    if (csh(end-1) < 0.5 ) %over exp csh(end) ==1 always!        
        val(imId) = nan;
        continue;
    end
    
    if strcmp( op, 'linear' )
        sumI = sumI + expTime(imId).*double( Ic{imId} );
    else
        error('Unknown op mode: %s',op);
    end
    
    sumI = normaliseToDouble(imfilter(sumI,fspecial('gaussian',3)) );
    conI = gradFun(sumI);    
    
    if var(h(1:end-1)-(h(2:end))) < 1E7 %heur for noise not giving good resp 
        val(imId)=0;
    else    
        val(imId) = sum(conI(:));
    end
    
end

[vals,idxN] = sort(val,'descend');


if idxN(1)<force
    idxN(1) = force; %require at least this many images
end

idxN(1)

%Redo with given number of images...
sumI = zeros(size(Ic{1}),'double');
for imId = 1:idxN(1)
    
    if strcmp( op, 'linear' )
        sumI = sumI + expTime(imId).*double( Ic{imId} );        
    end
    
end

nImages = idxN(1);

if gamma ~= 1.0
    sumI = sumI.^gamma;
end

end