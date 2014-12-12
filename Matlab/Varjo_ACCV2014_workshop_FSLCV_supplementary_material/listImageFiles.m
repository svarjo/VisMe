function [imFile, imTime, imfolder] = listImageFiles(imfolder, suffix)
%   function [imFile imTime] = listImageFiles(imfolder, suffix)
%   
%   get imageFile names in given folder (imfolder) with format
%   yyyy-mm-dd_HHhMMmSSs.suffix and extract the time from the file name
%   
%   outputs:
%           imFile      - the filenames in .name stuct variable
%           imTime      - the time extracted from the file name
%                         as datenum (datestr to human readable form)
%           imfolder    - the folder containing the images

if ~exist('imfolder','var')    
    imfolder = './';
end

if ~exist('suffix','var')
    suffix = 'tif';
end

imFile = dir([imfolder '*.' suffix]);

N = length(imFile);
imTime = zeros(N,1);

%
for id = 1:N
    
    token = sscanf( imFile(id).name , '%u-%u-%u_%uh%um%us');    
    imTime(id) = datenum(token(1),token(2),token(3),token(4),token(5),token(6));
    
    %(fix also datetime in file struct) to "real creation time"
    imFile(id).datenum = imTime(id);
    
end


end