function [out, infId] = multiScaleRetinex(I,N,cList,weightList)
%function [out, infId] = multiscaleRetinex(I)
%
%   Do multi scale retinex filtering on image I
%   input:
%       I - input image
%       N - number of scales
%       cList - Gaussian parameter (corrsponds to sigam) - not very sensitive
%       weightList - weighting of each subResult from cList (equal by def)
%
%   output:
%       out - filtered image (only valid part)
%       infId - if infinite values are found infId gives logical map
%               if not empty matrix is returned
%
%ref:"A Multiscale Retinex for Bridging the Gap Between Color Images and 
%     the Human Observation of Scenes" Daniel J. Jobson, Zia-ur Rahman, and Glenn A. Woodell
%     IEEE TRANSACTIONS ON IMAGE PROCESSING, VOL. 6, NO. 7, JULY 1997


if ~exist('N','var'), N = 3; end                    % pub 3
if ~exist('cList','var'),  cList = [15 80 250]; end % pub [c_i]

if length(cList) < N, 
    warning('sigmaList too short using generated'); 
    cList = 10:40:N*40;
end    

if ~exist('weightList','var'), weightList = repmat(1/N,N,1); end

out = zeros(size(I));
%logI = log(double(I));
logI = log(double(I)+1);

w = 4;
for id = 1:N            
    [X,Y] = meshgrid(-w:w,-w:w);    
    F = exp(-(X.^2+Y.^2)/(2*cList(id)^2));          %obs sqrt(X.^2+Y.^2)...!!!
    %F = exp(-sqrt(X.^2+Y.^2)/(2*cList(id)^2));    %obs sqrt(X.^2+Y.^2)...!!!
    F = F./sum(F(:));        
    Fi = conv2( I, F, 'same');
    %Ri = logI - log(Fi);
    Ri = logI - log(Fi+1);    
    
    out = out + (weightList(id).*Ri);
end

out = out(w+1:end-w,w+1:end-w,:); %take valid part

%Following should be now taken care of +1 in log
% % infId = (abs(out)==inf);
% % if sum(infId(:)) > 0 
% %     out(infId)=0;
% %     %warning('Infinite value(s) in result');
% % else
% %     infId = [];
% % end