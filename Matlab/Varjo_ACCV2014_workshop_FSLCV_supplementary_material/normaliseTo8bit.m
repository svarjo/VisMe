function I = normaliseTo8bit(I,gamma,verbose)
%function I = normaliseTo8bit(I,gamma,verbose)
%
%   Scale values in I in range [0 255]
%   If gamma is given ( gamma ~= 1 ) the normalised values are raised
%   to power of gamma and renormalised.
if ~exist('verbose','var'), verbose = 0; end
if ~exist('gamma','var'), gamma = 1; end

if ~isa( class(I), 'double') 
   I = double(I);
end

% normalise to keep possible gamma in reasonable range
minV = min(I(:));
maxV = max(I(:));
range = maxV-minV;
I = I - minV./range;

if verbose
    fprintf(' min=%d max=%d range=%d : ', minV, maxV, range);
end

if gamma~=1
    I = I.^gamma;
    % renormalise for 16 bit image
    minV = min(I(:));
    maxV = max(I(:));
    range = maxV-minV;
end

max8b =  255;
I = uint8( max8b.*(I-minV)./range);

    
    
end
